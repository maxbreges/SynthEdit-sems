#include "./IntEntryGui.h"
#include "unicode_conversion.h"
#include "it_enum_list.h"
#include "../se_sdk3/MpString.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <cstdlib> // For std::strtof
#include <cmath> // For std::fabs

using namespace gmpi;
using namespace gmpi_gui;
using namespace gmpi_sdk;
using namespace JmUnicodeConversions;
using namespace GmpiDrawing;
using namespace GmpiDrawing_API;
using namespace gmpi_gui_api;

GMPI_REGISTER_GUI(MP_SUB_TYPE_GUI2, IntEntryGui, L"IntEntry");
SE_DECLARE_INIT_STATIC_FILE(IntEntry_Gui);

IntEntryGui::IntEntryGui()
{
    // Initialise pins.
    initializePin(pinText, static_cast<MpGuiBaseMemberPtr2>(&IntEntryGui::redraw));
    initializePin(pinStyle, static_cast<MpGuiBaseMemberPtr2>(&IntEntryGui::onSetStyle));
    initializePin(pinMultiline, static_cast<MpGuiBaseMemberPtr2>(&IntEntryGui::onSetStyle));
    initializePin(pinWriteable);
    initializePin(pinGreyed, static_cast<MpGuiBaseMemberPtr2>(&IntEntryGui::redraw));
    initializePin(pinHint);
    initializePin(pinMouseDown_LEGACY);
    initializePin(pinMouseDown);
    initializePin(pinIntIn, static_cast<MpGuiBaseMemberPtr2>(&IntEntryGui::redraw));
    initializePin(pinIntOut, static_cast<MpGuiBaseMemberPtr2>(&IntEntryGui::redraw));
    initializePin(pinInt, static_cast<MpGuiBaseMemberPtr2>(&IntEntryGui::onSetText));
    initializePin(pinString, static_cast<MpGuiBaseMemberPtr2>(&IntEntryGui::onSetText));
}

void IntEntryGui::onSetText()
{
    pinText = pinString;
    pinIntIn = pinInt;
}

void IntEntryGui::redraw()
{
    pinIntOut = pinIntIn;
    pinInt = pinIntIn;
    pinString = pinText;
}

void IntEntryGui::onSetStyle()
{
    std::string customStyleName{ "IntEntryGui:text:" };
    customStyleName += WStringToUtf8(pinStyle.getValue());

    if (pinMultiline.getValue())
    {
        customStyleName += ":multiline";
    }

    fontmetadata = nullptr;
    textFormat = FontCache::instance()->TextFormatExists(getHost(), getGuiHost(), customStyleName, &fontmetadata);
    if (!textFormat)
    {
        // Get the specified font style.
        GetTextFormat(getHost(), getGuiHost(), pinStyle, &fontmetadata);

        auto textFormatMutable = AddCustomTextFormat(getHost(), getGuiHost(), customStyleName, fontmetadata);

        // Apply customization.
        if (pinMultiline.getValue())
        {
            textFormatMutable.SetWordWrapping(WordWrapping::Wrap);
        }
        else
        {
            textFormatMutable.SetWordWrapping(WordWrapping::NoWrap);
        }
        textFormat = textFormatMutable.Get();
    }

    TextSubcontrol::onSetStyle();
}

// Overridden to support multiline text.
int32_t IntEntryGui::OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext)
{
    GmpiDrawing::Graphics g(drawingContext);

    auto r = getRect();
    Rect textRect(r);
    textRect.Deflate((float)border, (float)border);

    // Background Fill
    auto brush = g.CreateSolidColorBrush(fontmetadata->getBackgroundColor());
    g.FillRectangle(textRect, brush);

    if (pinGreyed == true)
        brush.SetColor(Color::Gray);
    else
        brush.SetColor(fontmetadata->getColor());

    if (fontmetadata->verticalSnapBackwardCompatibilityMode)
    {
        auto directXOffset = fontmetadata->getLegacyVerticalOffset();
        textRect.top += directXOffset;
        textRect.bottom += directXOffset;
    }

    g.DrawTextU(getDisplayText(), textFormat, textRect, brush, (int32_t)DrawTextOptions::Clip);

    return gmpi::MP_OK;
}

int32_t IntEntryGui::onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point)
{
    // Let host handle right-clicks
    if ((flags & GG_POINTER_FLAG_FIRSTBUTTON) == 0)
    {
        return gmpi::MP_OK; // Indicate successful hit
    }

    setCapture();

    pinMouseDown = true;
    pinMouseDown_LEGACY = true;

    return gmpi::MP_OK;
}

int32_t IntEntryGui::onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point)
{
    if (!getCapture())
    {
        return gmpi::MP_UNHANDLED;
    }

    releaseCapture();
    pinMouseDown = false;
    pinMouseDown_LEGACY = false;

    if (pinWriteable == false)
        return gmpi::MP_OK;

    GmpiGui::GraphicsHost host(getGuiHost());
    nativeEdit = host.createPlatformTextEdit(getRect());
    nativeEdit.SetAlignment(fontmetadata->getTextAlignment(), pinMultiline.getValue() ? WordWrapping::Wrap : WordWrapping::NoWrap);
    nativeEdit.SetTextSize(static_cast<float>(fontmetadata->size_));
    nativeEdit.SetText(WStringToUtf8(pinText).c_str());

    // Show the edit control
    nativeEdit.ShowAsync([this](int32_t result) { this->OnTextEnteredComplete(result); });

    return gmpi::MP_OK;
}

std::string IntEntryGui::getDisplayText()
{
    return WStringToUtf8(pinText.getValue());
}

void IntEntryGui::OnTextEnteredComplete(int32_t result)
{
    if (result == gmpi::MP_OK)
    {
        // Update pinText with the current content of the text edit
        pinText = Utf8ToWstring(nativeEdit.GetText()); // Get the latest text after editing
        invalidateRect(); // Request UI redraw if necessary

        std::string inputText = WStringToUtf8(pinText.getValue());

        // Try to parse inputText as an integer
        try {
            long int intValue = std::stol(inputText);
            pinIntIn = static_cast<int32_t>(intValue); // Assign to pinIntOut
        }
        catch (const std::invalid_argument&) {
            // Not a valid integer, do nothing or set to 0
            pinIntIn = 0; // Optional: reset if invalid
        }
        catch (const std::out_of_range&) {
            // The number is out of range for int32_t, handle accordingly
            pinIntIn = (inputText.front() == '-') ? INT32_MIN : INT32_MAX; // Optional: assign min or max
        }

        pinIntOut = pinIntIn;
    }

    nativeEdit.setNull(); // Release the native edit instance
}

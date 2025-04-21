#include "./FloatEntryGui.h"
#include "unicode_conversion.h"
#include "it_enum_list.h"
#include "../se_sdk3/MpString.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <cstdlib> // For std::strtof
#include <cmath> // For std::fabs
#include "Drawing_API.h"

using namespace gmpi;
using namespace gmpi_gui;
using namespace gmpi_sdk;
using namespace JmUnicodeConversions;
using namespace GmpiDrawing;
using namespace GmpiDrawing_API;
using namespace gmpi_gui_api;

GMPI_REGISTER_GUI(MP_SUB_TYPE_GUI2, FloatEntryGui, L"FloatEntry");
SE_DECLARE_INIT_STATIC_FILE(FloatEntry_Gui);

const int32_t MIN_WIDTH = 40;
const int32_t MIN_HEIGHT = 20;

FloatEntryGui::FloatEntryGui()
{
    // Initialise pins.
    initializePin(pinText, static_cast<MpGuiBaseMemberPtr2>(&FloatEntryGui::redraw));
    initializePin(pinStyle, static_cast<MpGuiBaseMemberPtr2>(&FloatEntryGui::onSetStyle));
    initializePin(pinMultiline, static_cast<MpGuiBaseMemberPtr2>(&FloatEntryGui::onSetStyle));
    initializePin(pinWriteable);
    initializePin(pinGreyed, static_cast<MpGuiBaseMemberPtr2>(&FloatEntryGui::redraw));
    initializePin(pinHint);
    initializePin(pinMouseDown_LEGACY);
    initializePin(pinMouseDown);
    initializePin(pinFloatIn, static_cast<MpGuiBaseMemberPtr2>(&FloatEntryGui::redraw));
    initializePin(pinFloatOut, static_cast<MpGuiBaseMemberPtr2>(&FloatEntryGui::redraw));
    initializePin(pinFloat, static_cast<MpGuiBaseMemberPtr2>(&FloatEntryGui::onSetText));
    initializePin(pinString, static_cast<MpGuiBaseMemberPtr2>(&FloatEntryGui::onSetText));
    initializePin(pinWidth, static_cast<MpGuiBaseMemberPtr2>(&FloatEntryGui::onSetWidth));

/*    // Initializing cursor position variables
    lastDragPoint.x = 0;
    lastDragPoint.y = 0;*/
}
void FloatEntryGui::onSetWidth()
{
    invalidateRect();
}

void FloatEntryGui::onSetText()
{
    pinText = pinString;
    pinFloatIn = pinFloat;
}

void FloatEntryGui::redraw()
{
    pinFloatOut = pinFloatIn;
    pinFloat = pinFloatIn;
    pinString = pinText;
}

void FloatEntryGui::onSetStyle()
{
    std::string customStyleName{ "FloatEntryGui:text:" };
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

// Render function to draw text
int32_t FloatEntryGui::OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext)
{
    GmpiDrawing::Graphics g(drawingContext);

    auto r = getRect();
    Rect textRect(r);
    textRect.Deflate((float)border, (float)border);

    // Background fill
    auto brush = g.CreateSolidColorBrush(fontmetadata->getBackgroundColor());
    g.FillRectangle(textRect, brush);

    if (pinGreyed == true)
        brush.SetColor(Color::Gray);
    else
        brush.SetColor(fontmetadata->getColor());

    // Vertical snap (optional)
    if (fontmetadata->verticalSnapBackwardCompatibilityMode)
    {
        auto directXOffset = fontmetadata->getLegacyVerticalOffset();
        textRect.top += directXOffset;
        textRect.bottom += directXOffset;
    }

    // Calculate yOffset dynamically based on the height of the control
    float yOffset = (textRect.bottom - textRect.top - fontmetadata->pixelHeight_) / 2;

    // Ensure textRect accounts for the yOffset
    Rect adjustedTextRect(textRect.left, textRect.top + yOffset, textRect.right, textRect.bottom);
    g.DrawTextU(getDisplayText(), textFormat, adjustedTextRect, brush, (int32_t)DrawTextOptions::Clip);

    return gmpi::MP_OK;
}

int32_t FloatEntryGui::onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point)
{
    // Let host handle right-clicks
    if ((flags & GG_POINTER_FLAG_FIRSTBUTTON) == 0)
    {
        return gmpi::MP_OK; // Indicate successful hit
    }

    setCapture(); // Capture the pointer

    pinMouseDown = true; // Start tracking mouse down
    pinMouseDown_LEGACY = true;

/*    // Store the last drag position
    lastDragPoint.x = point.x;
    lastDragPoint.y = point.y;*/

    return gmpi::MP_OK;
}

/*// Implement `onPointerMove` to handle resizing based on drag
int32_t FloatEntryGui::onPointerMove(int32_t flags, GmpiDrawing_API::MP1_POINT point)
{
    if (!pinMouseDown)
    {
        return gmpi::MP_UNHANDLED; // Do nothing if the mouse is not pressed
    }

    // Calculate the mouse movement delta
    int32_t deltaX = point.x - lastDragPoint.x;
    int32_t deltaY = point.y - lastDragPoint.y;

    // Update control's size based on drag delta
    auto currentRect = getRect();

    int32_t newWidth = std::max<int32_t>(currentRect.right - currentRect.left + deltaX, MIN_WIDTH);
    int32_t newHeight = std::max<int32_t>(currentRect.bottom - currentRect.top + deltaY, MIN_HEIGHT);

    // Set the new dimensions
    int setSize();

    // Update last drag point
    lastDragPoint.x = point.x;
    lastDragPoint.y = point.y;

    return gmpi::MP_OK;
}*/

int32_t FloatEntryGui::onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point)
{
    if (!getCapture())
    {
        return gmpi::MP_UNHANDLED; // If not captured, do nothing
    }

    releaseCapture();
    pinMouseDown = false;
    pinMouseDown_LEGACY = false;

    if (pinWriteable == false)
    {
        return gmpi::MP_OK; // If not writable, skip edit control creation
    }

    // Handle text editing as before
    GmpiGui::GraphicsHost host(getGuiHost());
    nativeEdit = host.createPlatformTextEdit(getRect());
    nativeEdit.SetAlignment(fontmetadata->getTextAlignment(), pinMultiline.getValue() ? WordWrapping::Wrap : WordWrapping::NoWrap);
    nativeEdit.SetTextSize(static_cast<float>(fontmetadata->size_));
    nativeEdit.SetText(WStringToUtf8(pinText).c_str());

    // Show the edit control
    nativeEdit.ShowAsync([this](int32_t result) { this->OnTextEnteredComplete(result); });

    return gmpi::MP_OK;
}

std::string FloatEntryGui::getDisplayText()
{
    return WStringToUtf8(pinText.getValue());
}

void FloatEntryGui::OnTextEnteredComplete(int32_t result)
{
    if (result == gmpi::MP_OK)
    {
        // Update pinText with the current content of the text edit
        pinText = Utf8ToWstring(nativeEdit.GetText()); // Get the latest text after editing
        invalidateRect(); // Request UI redraw if necessary

        std::string inputText = WStringToUtf8(pinText.getValue());

        // Try to parse inputText as a float
        try {
            float floatValue = std::stof(inputText);
            pinFloatIn = floatValue; // Assign to pinFloatOut
        }
        catch (const std::invalid_argument&) {
            // Not a valid float, do nothing or set to 0.0f
            pinFloatIn = 0.0f; // Optional: reset if invalid
        }
        catch (const std::out_of_range&) {
            // The number is out of range, handle accordingly
            pinFloatIn = (inputText.front() == '-') ? -FLT_MAX : FLT_MAX; // Optional: assign max float value
        }

        pinFloatOut = pinFloatIn;
    }

    nativeEdit.setNull(); // Release the native edit instance
}

// Measure function to determine the size of the FloatEntryGui
int32_t FloatEntryGui::measure(GmpiDrawing_API::MP1_SIZE availableSize, GmpiDrawing_API::MP1_SIZE* returnDesiredSize)
{
    // Initialize minimum dimensions
    returnDesiredSize->width = MIN_WIDTH; // Ensure minimum width is always respected
    returnDesiredSize->height = MIN_HEIGHT; // Ensure minimum height is always respected

    // Default width for testing; replace with calculated values later
    const int32_t defaultTextWidth = pinWidth; // A smaller default width

    // Check if font metadata is available and update the width using a sensible default
    if (fontmetadata)
    {
        // Sample size calculation, which could be adjusted later for actual text width later when MeasureString becomes available
        returnDesiredSize->width = std::max<int32_t>(MIN_WIDTH, defaultTextWidth); // Default width of box
        returnDesiredSize->height = std::max<int32_t>(returnDesiredSize->height, fontmetadata->pixelHeight_ + 8); // Maintain height based on font metrics plus padding
    }

    return gmpi::MP_OK; // Indicate success
}

// Arrange function to layout the FloatEntryGui
int32_t FloatEntryGui::arrange(GmpiDrawing_API::MP1_RECT finalRect_s)
{
    Rect finalRect(finalRect_s);
    // Call base class arrange if necessary
    MpGuiGfxBase::arrange(finalRect);

    // Adjust text section drawing to fit inside the new dimensions while allowing for padding
    if (fontmetadata)
    {
        float yOffset = 5; // Top offset for text
        float xOffset = 5; // Left and right padding
        Rect textRect(finalRect.left + xOffset, finalRect.top + yOffset, finalRect.right - xOffset, finalRect.bottom - yOffset);

        // Store or use the calculated text rectangle directly for rendering
    }

    return gmpi::MP_OK;
}
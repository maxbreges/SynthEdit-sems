#include "./TextEntryGui.h"
#include "unicode_conversion.h"
#include "it_enum_list.h"
#include "../se_sdk3/MpString.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <cstdlib> // For std::strtof
#include <cmath> // For std::fabs
#define _USE_MATH_DEFINES
#include <math.h>

using namespace gmpi;
using namespace gmpi_gui;
using namespace gmpi_sdk;
using namespace JmUnicodeConversions;
using namespace GmpiDrawing;
using namespace GmpiDrawing_API;
using namespace gmpi_gui_api;

GMPI_REGISTER_GUI(MP_SUB_TYPE_GUI2, TextXGui, L"TextX");
SE_DECLARE_INIT_STATIC_FILE(TextX_Gui);

TextXGui::TextXGui()
{
    // Initialise pins.
    initializePin(pinText, static_cast<MpGuiBaseMemberPtr2>(&TextXGui::redraw));
    initializePin(pinTextColor, static_cast<MpGuiBaseMemberPtr2>(&TextXGui::onSetTextColor));
    initializePin(pinTopColor, static_cast<MpGuiBaseMemberPtr2>(&TextXGui::onSetTopColor));
    initializePin(pinBgColor, static_cast<MpGuiBaseMemberPtr2>(&TextXGui::onSetBgColor));
    initializePin(pinStyle, static_cast<MpGuiBaseMemberPtr2>(&TextXGui::onSetStyle));
    initializePin(pinWrapping, static_cast<MpGuiBaseMemberPtr2>(&TextXGui::onSetStyle));
    initializePin(pinWriteable);
    initializePin(pinGreyed, static_cast<MpGuiBaseMemberPtr2>(&TextXGui::redraw));
    initializePin(pinHint);
    initializePin(pinMouseDown);
/*    initializePin(pinFontSize, static_cast<MpGuiBaseMemberPtr2>(&TextXGui::redraw));
    initializePin(pinFont, static_cast<MpGuiBaseMemberPtr2>(&TextXGui::redraw));
    initializePin(pinAlignV, static_cast<MpGuiBaseMemberPtr2>(&TextXGui::redraw));*/
}

void TextXGui::redraw()
{
    invalidateRect();
}
void TextXGui::onSetTextColor()
{
    invalidateRect();
}
void TextXGui::onSetTopColor()
{
    invalidateRect();
}
void TextXGui::onSetBgColor()
{
    invalidateRect();
}

void TextXGui::onSetStyle()
{
    std::string customStyleName{ "TextXGui:text:" };
    customStyleName += WStringToUtf8(pinStyle.getValue());

    if (pinWrapping.getValue())
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
        if (pinWrapping.getValue())
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
int32_t TextXGui::OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext)
{
    GmpiDrawing::Graphics g(drawingContext);

    auto r = getRect();
    Rect textRect(r);
    textRect.Deflate((float)border, (float)border);
    //==============
    int width = r.right - r.left;
    int height = r.bottom - r.top;

    int radius = 3;

    radius = (std::min)(radius, width / 2);
    radius = (std::min)(radius, height / 2);

    auto geometry = g.GetFactory().CreatePathGeometry();
    auto sink = geometry.Open();

    // define a corner 
    const float rightAngle = M_PI * 0.5f;
    // top left
    if (1)
    {
        sink.BeginFigure(Point(0, radius), FigureBegin::Filled);
        ArcSegment as(Point(radius, 0), Size(radius, radius), rightAngle);
        sink.AddArc(as);
    }
    else
    {
        sink.BeginFigure(Point(0, 0), FigureBegin::Filled);
    }

    // top right
    if (1)
    {
        sink.AddLine(Point(width - radius, 0));
        //		sink.AddArc(Corner, 270, 90);
        ArcSegment as(Point(width, radius), Size(radius, radius), rightAngle);
        sink.AddArc(as);
    }
    else
    {
        sink.AddLine(Point(width, 0));
    }

    // bottom right
    if (1)
    {
        sink.AddLine(Point(width, height - radius));
        //		sink.AddArc(Corner, 0, 90);
        ArcSegment as(Point(width - radius, height), Size(radius, radius), rightAngle);
        sink.AddArc(as);
    }
    else
    {
        sink.AddLine(Point(width, height));
    }

    // bottom left
    if (1)
    {
        sink.AddLine(Point(radius, height));
        ArcSegment as(Point(0, height - radius), Size(radius, radius), rightAngle);
        sink.AddArc(as);
    }
    else
    {
        sink.AddLine(Point(0, height));
    }

    // end path
    sink.EndFigure();
    sink.Close();

    // Gradient.
    Point point1(1, 0);
    Point point2(1, height);

    GradientStop gradientStops[]
    {
        { 0.0f, GmpiDrawing::Color::FromHexString(pinTopColor) },
        { 1.0f, GmpiDrawing::Color::FromHexString(pinBgColor) },
    };

    auto gradientBrush = g.CreateLinearGradientBrush(gradientStops, point1, point2);

    g.FillGeometry(geometry, gradientBrush);

    auto brush = g.CreateSolidColorBrush(GmpiDrawing::Color::FromHexString(pinBgColor));

    if (pinGreyed == true)
        brush.SetColor(Color::Gray);
    else
        brush.SetColor(GmpiDrawing::Color::FromHexString(pinTextColor));

    if (fontmetadata->verticalSnapBackwardCompatibilityMode)
    {
        auto directXOffset = fontmetadata->getLegacyVerticalOffset();
        textRect.top += directXOffset;
        textRect.bottom += directXOffset;
    }

    g.DrawTextU(getDisplayText(), textFormat, textRect, brush, (int32_t)DrawTextOptions::Clip);

    return gmpi::MP_OK;
}

int32_t TextXGui::onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point)
{
    // Let host handle right-clicks
    if ((flags & GG_POINTER_FLAG_FIRSTBUTTON) == 0)
    {
        return gmpi::MP_OK; // Indicate successful hit
    }

    setCapture();

    pinMouseDown = true;

    return gmpi::MP_OK;
}

int32_t TextXGui::onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point)
{
    if (!getCapture())
    {
        return gmpi::MP_UNHANDLED;
    }

    releaseCapture();
    pinMouseDown = false;

    if (pinWriteable == false)
        return gmpi::MP_OK;

    GmpiGui::GraphicsHost host(getGuiHost());
    nativeEdit = host.createPlatformTextEdit(getRect());
    nativeEdit.SetAlignment(fontmetadata->getTextAlignment(), pinWrapping.getValue() ? WordWrapping::Wrap : WordWrapping::NoWrap);
    nativeEdit.SetTextSize(static_cast<float>(fontmetadata->size_));
    nativeEdit.SetText(WStringToUtf8(pinText).c_str());

    // Show the edit control
    nativeEdit.ShowAsync([this](int32_t result) { this->OnTextEnteredComplete(result); });

    return gmpi::MP_OK;
}

std::string TextXGui::getDisplayText()
{
    return WStringToUtf8(pinText.getValue());
}

void TextXGui::OnTextEnteredComplete(int32_t result)
{
    if (result == gmpi::MP_OK)
    {
        // Update pinText with the current content of the text edit
        pinText = Utf8ToWstring(nativeEdit.GetText()); // Get the latest text after editing
        invalidateRect(); // Request UI redraw if necessary

        std::string inputText = WStringToUtf8(pinText.getValue());

        nativeEdit.setNull(); // Release the native edit instance
    }
}

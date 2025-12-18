#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include "mp_gui.h"
#include <sstream>
#include <iomanip>

using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;
using namespace JmUnicodeConversions;

namespace {
    constexpr float MouseWheelCoarseness = 12000.0f;
    constexpr int MinFontSize = 8;
    constexpr int MaxFontSize = 32;
}

// Utility function outside class for reusability
Color FromHexStringBackwardCompatible(const std::wstring& s)
{
    constexpr float oneOver255 = 1.0f / 255.0f;
    wchar_t* stopString = nullptr;
    uint32_t hex = wcstoul(s.c_str(), &stopString, 16);
    if (stopString == s.c_str() || *stopString != L'\0') {
        // Invalid parsing
        return Color(0, 0, 0, 0);
    }
    float alpha = ((hex >> 24) & 0xFF) * oneOver255;
    return Color(
        se_sdk::FastGamma::sRGB_to_float((hex >> 16) & 0xFF),
        se_sdk::FastGamma::sRGB_to_float((hex >> 8) & 0xFF),
        se_sdk::FastGamma::sRGB_to_float(hex & 0xFF),
        alpha
    );
}

class DisplayText final : public gmpi_gui::MpGuiGfxBase
{
public:
    // Constructor
    DisplayText()
    {
        initializePin(pinText, static_cast<MpGuiBaseMemberPtr2>(&DisplayText::onSetText));
        initializePin(pinHint);
        initializePin(pinBgColor);
        initializePin(pinTopColor, static_cast<MpGuiBaseMemberPtr2>(&DisplayText::onSetTopColor));
        initializePin(pinTextColor, static_cast<MpGuiBaseMemberPtr2>(&DisplayText::onSetTextColor));
        initializePin(pinFont, static_cast<MpGuiBaseMemberPtr2>(&DisplayText::onSetTextFont));
        initializePin(pinFontSize, static_cast<MpGuiBaseMemberPtr2>(&DisplayText::onSetFontSize));
        initializePin(pinAnimPosShift);
        initializePin(pinAnimPosAlt);
        initializePin(pinMouseDown);
        initializePin(pinCornerRadius, static_cast<MpGuiBaseMemberPtr2>(&DisplayText::onSetCornerRadius));
        initializePin(pinCtrlClick);
        initializePin(pinAlignV, static_cast<MpGuiBaseMemberPtr2>(&DisplayText::onSetAlignV));
        initializePin(pinAlignY, static_cast<MpGuiBaseMemberPtr2>(&DisplayText::onSetAlignY));
    }

    // --- Pin handlers
    void onSetTopColor() { invalidateRect(); }
    void onSetText() { invalidateRect(); }
    void onSetTextColor() { invalidateRect(); }
    void onSetTextFont() { invalidateRect(); }
    void onSetFontSize() { invalidateRect(); }
    void onSetCornerRadius() { invalidateRect(); }
    void onSetAlignV() { invalidateRect(); }
    void onSetAlignY() 
    {
        padding = static_cast<int>(pinAlignY);
        invalidateRect();
    }

    // --- Pointer events
    int32_t MP_STDCALL getToolTip(GmpiDrawing_API::MP1_POINT point, gmpi::IString* returnString) override
    {
        auto utf8String = (std::string)pinHint;
        returnString->setData(utf8String.data(), static_cast<int32_t>(utf8String.size()));
        return gmpi::MP_OK;
    }

    int32_t MP_STDCALL onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
    {
        if ((flags & GG_POINTER_FLAG_FIRSTBUTTON) == 0)
            return gmpi::MP_OK; // Right-click handling

        if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL)
            pinCtrlClick = true;

        pointPrevious = point;
        pinMouseDown = true;
        setCapture();

        return gmpi::MP_OK;
    }

    int32_t MP_STDCALL onMouseWheel(int32_t flags, int32_t delta, MP1_POINT point) override
    {
        float newPos = pinAnimPosAlt + delta / MouseWheelCoarseness;
        newPos = std::clamp(newPos, 0.f, 1.f);
        pinAnimPosAlt = newPos;
        invalidateRect();
        return gmpi::MP_OK;
    }

    int32_t MP_STDCALL onPointerMove(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
    {
        if (!getCapture()) return gmpi::MP_UNHANDLED;

        if (flags & gmpi_gui_api::GG_POINTER_KEY_ALT)
            return gmpi::MP_UNHANDLED;

        if (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT)
        {
            Point offset(point.x - pointPrevious.x, point.y - pointPrevious.y);
            float coarseness = 0.004f;

            if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL)
                coarseness = 0.001f; // optional: if you want magnification on ctrl

            float newPos = pinAnimPosShift - coarseness * static_cast<float>(offset.y);
            newPos = std::clamp(newPos, 0.f, 1.f);
            pinAnimPosShift = newPos;
        }

        pointPrevious = point;
        invalidateRect();

        return gmpi::MP_OK;
    }

    int32_t onPointerUp(int32_t flags, struct GmpiDrawing_API::MP1_POINT point)
    {
        pinMouseDown = false;
        pinCtrlClick = false;
        releaseCapture();

        return gmpi::MP_OK;
    }

    // --- Rendering
    int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override
    {
        Graphics g(drawingContext);
        ClipDrawingToBounds clip(g, getRect());

        auto rect = getRect();
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;

        auto topColor = FromHexStringBackwardCompatible(pinTopColor);
        auto bottomColor = !pinBgColor.getValue().empty() ? FromHexStringBackwardCompatible(pinBgColor) : topColor;

        int radius = static_cast<int>(pinCornerRadius);
        radius = std::min<float>(radius, width / 2);
        radius = std::min<float>(radius, height / 2);

        auto geometry = g.GetFactory().CreatePathGeometry();
        auto sink = geometry.Open();

        const float rightAngle = 3.14159265358979323846f * 0.5f;

        // Draw the rounded rectangle with arcs
        sink.BeginFigure(Point(0, radius), FigureBegin::Filled);
        sink.AddArc(ArcSegment(Point(radius, 0), Size(radius, radius), rightAngle));

        sink.AddLine(Point(width - radius, 0));
        sink.AddArc(ArcSegment(Point(width, radius), Size(radius, radius), rightAngle));

        sink.AddLine(Point(width, height - radius));
        sink.AddArc(ArcSegment(Point(width - radius, height), Size(radius, radius), rightAngle));

        sink.AddLine(Point(radius, height));
        sink.AddArc(ArcSegment(Point(0, height - radius), Size(radius, radius), rightAngle));

        sink.EndFigure();
        sink.Close();

        // Create gradient fill
        Point startPoint(1, 0);
        Point endPoint(1, height);
        GradientStop gradientStops[]
        {
            { 0.0f, topColor },
            { 1.0f, bottomColor }
        };
        auto gradientBrush = g.CreateLinearGradientBrush(gradientStops, startPoint, endPoint);
        g.FillGeometry(geometry, gradientBrush);

        // --- Draw text
        int fontSize = determineFontSize(rect);
        auto textFormat = createTextFormat(g, fontSize);
        textFormat.SetTextAlignment(TextAlignment::Center);
        auto textRect = getTextRect(rect, padding);

        auto textBrush = g.CreateSolidColorBrush(Color::White);
        
        // Platform-specific text drawing
#ifdef _WIN32
        g.DrawTextU(getDisplayText(), textFormat, getRect(), textBrush, 1);
#else
        g.DrawTextU(pinText, textFormat, getRect(), textBrush, 1);
#endif

        return gmpi::MP_OK;
    }

private:
    // --- Helper methods
    int determineFontSize(const Rect& rect)
    {
        int fontSize = 0;
        if (pinFontSize != 0)
        {
            fontSize = std::clamp<int>(pinFontSize, MinFontSize, MaxFontSize);
        }
        else
        {
            int height = rect.bottom - rect.top;
            fontSize = static_cast<int>(height * 0.5f);
            fontSize = std::clamp<int>(fontSize, MinFontSize, MaxFontSize);
        }
        return fontSize;
    }

    TextFormat createTextFormat(Graphics& g, int fontSize)
    {
        std::string fontFace = std::string(pinFont);
        auto tf = g.GetFactory().CreateTextFormat(fontSize, fontFace.c_str());
        tf.SetParagraphAlignment(static_cast<ParagraphAlignment>(pinAlignV.getValue()));
        tf.SetTextAlignment(TextAlignment::Center);
        return tf;
    }

    Rect getTextRect(const Rect& rect, int padding)
    {
        return Rect(rect.left, rect.top + padding, rect.right, rect.bottom - padding);
    }

    std::string getDisplayText()
    {
        return WStringToUtf8(pinText.getValue());
    }

    // --- Pins
    StringGuiPin pinText;
    StringGuiPin pinHint;
    StringGuiPin pinBgColor;
    StringGuiPin pinTopColor;
    StringGuiPin pinTextColor;
    StringGuiPin pinFont;
    IntGuiPin pinFontSize;
    FloatGuiPin pinAnimPosShift;
    FloatGuiPin pinAnimPosAlt;
    BoolGuiPin pinMouseDown;
    IntGuiPin pinCornerRadius;
    BoolGuiPin pinCtrlClick;
    IntGuiPin pinAlignV;
    FloatGuiPin pinAlignY;

    // --- Internal state
    int padding = 0;
    Point pointPrevious;
};
namespace
{
    auto r = Register<DisplayText>::withId(L"DisplayText");
}
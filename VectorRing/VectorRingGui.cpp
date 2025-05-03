#include "mp_sdk_gui2.h"
#include "Drawing.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "../shared/fast_gamma.h"

using namespace gmpi;
using namespace GmpiDrawing;

class VectorRingGui final : public gmpi_gui::MpGuiGfxBase
{
    void onSetColor()
    {
        invalidateRect();
    }

    void onSetThickness()
    {
        invalidateRect();
    }

    void onSetGradientStop()
    {
        invalidateRect();
    }

    void onSetGrayScale()
    {
        invalidateRect();
    }

    StringGuiPin pinOuterColor;
    StringGuiPin pinColor;
    BoolGuiPin pinGrayScale;
    FloatGuiPin pinThickness;
    FloatGuiPin pinGradientStop;

public:
    VectorRingGui()
    {
        initializePin(pinOuterColor, static_cast<MpGuiBaseMemberPtr2>(&VectorRingGui::onSetColor));
        initializePin(pinColor, static_cast<MpGuiBaseMemberPtr2>(&VectorRingGui::onSetColor));
        initializePin(pinGrayScale, static_cast<MpGuiBaseMemberPtr2>(&VectorRingGui::onSetGrayScale));
        initializePin(pinThickness, static_cast<MpGuiBaseMemberPtr2>(&VectorRingGui::onSetThickness));
        initializePin(pinGradientStop, static_cast<MpGuiBaseMemberPtr2>(&VectorRingGui::onSetGradientStop));
    }

    void calcDimensions(Point& center, float& radius, float& thickness)
    {
        auto r = getRect();
        center = Point(0.5f * r.right, 0.5f * r.bottom);
        radius = std::min<>(r.getWidth(), r.getHeight()) * 0.25f;
        thickness = pinThickness * radius;
    }

    Color FromHexStringBackwardCompatible(const std::wstring& s)
    {
        uint32_t hex = wcstoul(s.c_str(), nullptr, 16);
        float alpha = ((hex >> 24) & 0xff) / 255.0f;
        return Color(se_sdk::FastGamma::sRGB_to_float((hex >> 16) & 0xff),
            se_sdk::FastGamma::sRGB_to_float((hex >> 8) & 0xff),
            se_sdk::FastGamma::sRGB_to_float(hex & 0xff),
            alpha);
    }

    // Function to convert a color to grayscale
    Color toGrayscale(const Color& color)
    {
        // Standard grayscale conversion using luminosity method
        float gray = 0.2126f * color.r + 0.7152f * color.g + 0.0722f * color.b;
        return Color(gray, gray, gray, color.a);
    }

    int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override
    {
        Graphics g(drawingContext);
        auto r = getRect();

        // Calculate center, radius, thickness
        Point center;
        float radius;
        float thickness;
        calcDimensions(center, radius, thickness);

        Color PinColor = FromHexStringBackwardCompatible(pinColor);
        Color outerColor = FromHexStringBackwardCompatible(pinOuterColor);

        // Apply grayscale if pinGrayScale is true
        if (pinGrayScale)
        {
            PinColor = toGrayscale(PinColor);
            outerColor = toGrayscale(outerColor);
        }

        float PinGradientStop = pinGradientStop / 2 + 0.5f;

        // Gradient stops setup
        GradientStop gradientStops[]
        {
            { 0.f, outerColor },
            { PinGradientStop, PinColor }
        };

        // Calculate the adjusted radius to make stroke appear outside
        float adjustedRadius = radius + (thickness / 2.0f);
        float adjustedRadius1 = (radius + (thickness / 2.0f)) * 1.4f;

        auto gradientStopCollection = g.CreateGradientStopCollection(gradientStops);
        RadialGradientBrushProperties radialGradientProps(center, adjustedRadius1);
        auto Brush = g.CreateRadialGradientBrush(radialGradientProps, BrushProperties(), gradientStopCollection);

        GmpiDrawing::Ellipse ellipse(center, adjustedRadius, adjustedRadius);
        g.DrawEllipse(ellipse, Brush, thickness);

        return gmpi::MP_OK;
    }
};

namespace
{
    auto r = Register<VectorRingGui>::withId(L"VectorRing");
}
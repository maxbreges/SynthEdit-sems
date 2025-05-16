#include "mp_sdk_gui2.h"
#include "mp_sdk_gui.h"
#include "Drawing.h"
#include <iomanip>
#include <sstream>
#include <vector>         // For std::vector
#include <string.h>      // For memcpy
#include <chrono>
#include <thread>

using namespace std;
using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;

// Previous mouse point for tracking movement
GmpiDrawing_API::MP1_POINT pointPrevious;

// Constants for better readability
const float WHEEL_SCROLL_FACTOR = 1.0f / 12000.0f;
const float CIRCLE_SIZE_MULTIPLIER = 0.4f;
const float THICKNESS_MULTIPLIER = 0.2f;
const int HEX_WIDTH = 2; // Width for hex color string

class BlobKnobGui final : public gmpi_gui::MpGuiGfxBase
{
    void onSetAnimationIn() {
        pinAnimationPosition = pinAnimationIn;
    }

    void onSetAnimationPosition() {
        pinAnimationIn = pinAnimationPosition;
pinAnimationPositionOut = pinAnimationPosition;
        onSetBlob();

        if (pinColorAdj) {
            AnimPosToHex();
            pinHex = pinHexIn;
            pinBgColor = pinHexIn;
        }
        else {
            pinHexIn = pinHex;
            pinBgColor = pinHex;
        }
        
        invalidateRect();
    }

    void onSetName() {
        // Placeholder for name setting
    }

    void onSetBlob()
    {
        // Serialize hint and animation position into a string
        std::string category = pinCategory;
        std::string hint = pinHint;  // Current hint
        float animationPos = pinAnimationPosition * 100.f; // Current animation position

        std::ostringstream oss;

        // Set precision to 2 decimal places
        oss << std::fixed << std::setprecision(1) << animationPos;

        // Get the formatted animation position string
        std::string formattedAnimationPos = oss.str();

        // Create a string that contains both values
        std::string blobString = category + "\n" + hint + "\n" + formattedAnimationPos;

        pinDebug = blobString;
        pinHexOut = pinHex;
        invalidateRect();
    }
private:
    int count = 0;
    bool processing = false;
public:
    void onSetMouseDown() 
    {
        if (pinMouseDown)
        {
            ++count;

           

            int countDown = 250;
            // Start countdown in a separate thread
            std::thread([this, countDown]()
                {                    
                    std::this_thread::sleep_for(std::chrono::milliseconds(countDown));                   

                     // Reset count when countdown is finished
                    {
                        count = 0;                 
                    }

                }
            ).detach();
        }

        {
            if (count == 2)
            {
                pinAnimationPosition = pinResetValue;
               
                // Check if we are already processing
                if (processing)
                {
                    return; // Ignore further clicks while processing
                }

                processing = true; // Set flag to indicate processing has started
            }
            // Reset the processing flag to allow future clicks
            processing = false;
        }
    }

    void onSetMouseOver() 
    {
        onSetBlob();
        
    }

    void onSetBgColor() {
        invalidateRect();
    }

    void onSetCircleSize() {
        invalidateRect();
    }

    void onSetLineThickness() {
        invalidateRect();
    }

    void onSetColorAdj() {
        invalidateRect();
    }

    void onSetHexIn() {
        invalidateRect();
    }

    void onSetStyle() {
        invalidateRect();
    }

    void onSetLineColor() {
        invalidateRect();
    }

    void onSetHexOut() 
    {
        
    }

    FloatGuiPin pinAnimationIn;
    FloatGuiPin pinAnimationPosition;
    StringGuiPin pinCategory;
    StringGuiPin pinHint;
    BoolGuiPin pinMouseDown;
    BoolGuiPin pinMouseOver;
    StringGuiPin pinBgColor;
    FloatGuiPin pinCircleSize;
    FloatGuiPin pinLineThickness;
    BoolGuiPin pinColorAdj;
    StringGuiPin pinHex;
    StringGuiPin pinHexIn;
    IntGuiPin pinStyle;
    StringGuiPin pinLineColor;
    StringGuiPin pinDebug;
    StringGuiPin pinHexOut;
    FloatGuiPin pinResetValue;
FloatGuiPin pinAnimationPositionOut;

public:
    BlobKnobGui() {
        // Initialize GUI pins with event handlers
        initializePin(pinAnimationIn, static_cast<MpGuiBaseMemberPtr2>(&BlobKnobGui::onSetAnimationIn));
        initializePin(pinAnimationPosition, static_cast<MpGuiBaseMemberPtr2>(&BlobKnobGui::onSetAnimationPosition));
        initializePin(pinCategory, static_cast<MpGuiBaseMemberPtr2>(&BlobKnobGui::onSetName));
        initializePin(pinHint, static_cast<MpGuiBaseMemberPtr2>(&BlobKnobGui::onSetName));
        initializePin(pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&BlobKnobGui::onSetMouseDown));
        initializePin(pinMouseOver, static_cast<MpGuiBaseMemberPtr2>(&BlobKnobGui::onSetMouseOver));
        initializePin(pinBgColor, static_cast<MpGuiBaseMemberPtr2>(&BlobKnobGui::onSetBgColor));
        initializePin(pinCircleSize, static_cast<MpGuiBaseMemberPtr2>(&BlobKnobGui::onSetCircleSize));
        initializePin(pinLineThickness, static_cast<MpGuiBaseMemberPtr2>(&BlobKnobGui::onSetLineThickness));
        initializePin(pinColorAdj, static_cast<MpGuiBaseMemberPtr2>(&BlobKnobGui::onSetColorAdj));
        initializePin(pinHex, static_cast<MpGuiBaseMemberPtr2>(&BlobKnobGui::onSetAnimationPosition));
        initializePin(pinHexIn, static_cast<MpGuiBaseMemberPtr2>(&BlobKnobGui::onSetHexIn));
        initializePin(pinStyle, static_cast<MpGuiBaseMemberPtr2>(&BlobKnobGui::onSetStyle));
        initializePin(pinLineColor, static_cast<MpGuiBaseMemberPtr2>(&BlobKnobGui::onSetLineColor));
        initializePin(pinDebug, static_cast<MpGuiBaseMemberPtr2>(&BlobKnobGui::onSetBlob));
        initializePin(pinHexOut, static_cast<MpGuiBaseMemberPtr2>(&BlobKnobGui::onSetBlob));
        initializePin(pinResetValue, static_cast<MpGuiBaseMemberPtr2>(&BlobKnobGui::onSetMouseDown));
initializePin(pinAnimationPositionOut, static_cast<MpGuiBaseMemberPtr2>(&BlobKnobGui::onSetAnimationPosition));
    }

    int32_t MP_STDCALL setHover(bool isMouseOverMe) override {
        pinMouseOver = isMouseOverMe;
        return gmpi::MP_OK;
    }

    int32_t MP_STDCALL getToolTip(GmpiDrawing_API::MP1_POINT point, gmpi::IString* returnString) override {
        auto utf8String = (std::string)pinHint;
        returnString->setData(utf8String.data(), (int32_t)utf8String.size());
        return gmpi::MP_OK;
    }

    int32_t MP_STDCALL onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point) override {
        // Let host handle right-clicks
        if ((flags & GG_POINTER_FLAG_FIRSTBUTTON) == 0) {
            return gmpi::MP_OK; // Indicate successful hit for right-click menu
        }
        /*if ((flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL) != 0) {
            pinAnimationPosition = 0.5f; // Reset the animation position on control press
        }*/        

        pointPrevious = point; // note first point
        pinMouseDown = true;
        setCapture();

        return gmpi::MP_OK;
    }

    int32_t MP_STDCALL onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point) override {
        if (!getCapture()) {
            return gmpi::MP_UNHANDLED;
        }

        if ((flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT) == 0) {
            pinColorAdj = false; // Reset the color adjustment flag
        }

        pinMouseDown = false;
        releaseCapture();

        return gmpi::MP_OK;
    }

    int32_t MP_STDCALL onMouseWheel(int32_t flags, int32_t delta, MP1_POINT point) override {
        float new_pos = pinAnimationPosition + delta * WHEEL_SCROLL_FACTOR; // Updated scrolling logic
        new_pos = max(0.f, min(new_pos, 1.f)); // Clamp value between 0 and 1

        pinColorAdj = (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT) != 0; // Set color adjustment flag based on shift state

        pinAnimationPosition = new_pos;

        return gmpi::MP_OK;
    }

    int32_t MP_STDCALL onPointerMove(int32_t flags, GmpiDrawing_API::MP1_POINT point) override {
        if (!getCapture()) {
            return gmpi::MP_UNHANDLED;
        }

        // Ignore horizontal scrolling
        if (flags & gmpi_gui_api::GG_POINTER_KEY_ALT) {
            return gmpi::MP_UNHANDLED;
        }

        pinColorAdj = (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT) != 0; // Set color adjustment flag based on shift state

        Point offset(point.x - pointPrevious.x, point.y - pointPrevious.y); // Calculate offset

        float coarseness = (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL) ? 0.001f : 0.005f; // Adjust coarseness based on control key

        float new_pos = pinAnimationPosition -coarseness * static_cast<float>(offset.y);
        new_pos = max(0.f, min(new_pos, 1.f)); // Clamp value between 0 and 1

        pinAnimationPosition = new_pos;
        pointPrevious = point;

        invalidateRect();

        return gmpi::MP_OK;
    }

    int32_t AnimPosToHex() {
        int x = pinAnimationPosition * 1535; // Scale animation position to range
        int R = 0;
        int G = 0;
        int B = 0;

        // Determine RGB values based on x
        if ((x >= 255) && (x < 768)) G = 255;
        if (x > 1023) G = 0;
        if ((x >= 1279) || (x < 255)) R = 255;
        if ((x > 510) && (x <= 1024)) R = 0;
        if ((x >= 767) && (x < 1280)) B = 255;
        if (x <= 511) B = 0;

        // Additional color transitions
        if ((x >= 0) && (x <= 255)) G = x;
        if ((x >= 256) && (x <= 511)) R = 255 - (x - 256);
        if ((x >= 512) && (x <= 767)) B = x - 512;
        if ((x >= 768) && (x <= 1023)) G = 255 - (x - 768);
        if ((x >= 1024) && (x <= 1279)) R = x - 1024;
        if ((x >= 1280) && (x <= 1535)) B = 255 - (x - 1280);

        // Format to hex string with 'ff' (for full opacity) and clamped RGB values
        std::stringstream ssR, ssG, ssB;
        ssR << std::setfill('0') << std::setw(HEX_WIDTH) << std::hex << R;
        ssG << std::setfill('0') << std::setw(HEX_WIDTH) << std::hex << G;
        ssB << std::setfill('0') << std::setw(HEX_WIDTH) << std::hex << B;

        pinHexIn = "ff" + ssR.str() + ssG.str() + ssB.str(); // Combining into a final hex color string

        return gmpi::MP_OK;
    }

    void calcDimensionsBg(Point& centerBg, float& radiusBg, float& thicknessBg) {
        auto r = getRect();
        centerBg = Point((r.left + r.right) * 0.5f, (r.top + r.bottom) * 0.5f);
        radiusBg = min(r.getWidth(), r.getHeight()) * 0.4f; // Background radius
        thicknessBg = radiusBg * THICKNESS_MULTIPLIER; // Background thickness
    }

    void calcDimensions(Point& center, float& radius, float& thickness) {
        auto r = getRect();
        float mult = pinCircleSize * CIRCLE_SIZE_MULTIPLIER; // Scaling by pinCircleSize
        center = Point((r.left + r.right) * 0.5f, (r.top + r.bottom) * 0.5f);
        radius = min(r.getWidth(), r.getHeight()) * mult; // Circle radius
        thickness = radius * THICKNESS_MULTIPLIER; // Circle thickness
    }

    int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override {
        Graphics g(drawingContext);
        Point center;
        float radiusBg, thicknessBg;
        calcDimensionsBg(center, radiusBg, thicknessBg);

        float radius, thickness;
        calcDimensions(center, radius, thickness);

        auto brushLine = g.CreateSolidColorBrush(Color::FromHexString(pinLineColor));
        auto gradientBrushBg = g.CreateRadialGradientBrush(Color::FromHexString(pinBgColor), Color::Black, center, radiusBg);
        auto gradientBrush = g.CreateRadialGradientBrush(Color::FromHexString(pinHexIn), Color::Black, center, radius);

        const float startAngle = 30.0f; // Angle between "straight-down" and start of arc
        const float startAngleRadians = startAngle * (3.14159265358979323846f / 180.f); // Convert to radians
        const float quarterTurnClockwise = 3.14159265358979323846f * 0.5f;

        // Create stroke style with cap style
        StrokeStyleProperties strokeStyleProperties;
        CapStyle arr[] = { CapStyle::Flat, CapStyle::Square, CapStyle::Round, CapStyle::Triangle };
        strokeStyleProperties.setCapStyle(arr[pinStyle.getValue()]);

        auto strokeStyle = g.GetFactory().CreateStrokeStyle(strokeStyleProperties);

        Point startPoint(center.x + radius * cosf(quarterTurnClockwise + startAngleRadians), center.y + radius * sinf(quarterTurnClockwise + startAngleRadians));
        Point midPoint(center.x + radius, center.y - radius);

        float nominalized = pinAnimationPosition;
        float sweepAngle = nominalized * (static_cast<float>(3.14159265358979323846) * 2.0f - startAngleRadians * 2.0f);
        Point movingPoint(center.x + radius * cosf(quarterTurnClockwise + startAngleRadians + sweepAngle), center.y + radius * sinf(quarterTurnClockwise + startAngleRadians + sweepAngle));

        // Draw background circle
        g.FillCircle(center, radiusBg + thicknessBg * .5f, gradientBrushBg);
        // Draw the top circle
        g.FillCircle(center, radius + thickness * .5f, gradientBrush);
        // Draw the line
        g.DrawLine(center, movingPoint, brushLine, pinLineThickness, strokeStyle);

        return gmpi::MP_OK;
    }
};

namespace {
    auto r = Register<BlobKnobGui>::withId(L"BlobKnob");
}

#pragma once 
#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include <iomanip>
#include <sstream>
#include "mp_gui.h"
#include <chrono>
#include <thread>

using namespace gmpi;
using namespace gmpi_gui; //for enum GG_POINTER_FLAGS
using namespace GmpiGui;
using namespace GmpiDrawing;

class ValueBoxGui final : public gmpi_gui::MpGuiGfxBase
{
    FloatGuiPin pinAnimPos;
    FloatGuiPin pinAnimPosColor;
    FloatGuiPin pinBrightness;
    FloatGuiPin pinBrightnessBot;

    FloatGuiPin pinMin;
    FloatGuiPin pinMax;

    StringGuiPin pinHelp;
    StringGuiPin pinColorHex;

    FloatGuiPin pinFontSize;
    StringGuiPin pinFontFace;
    StringGuiPin pinTextColor;

    IntGuiPin pinPrecision;
    IntGuiPin pinCorner;
    BoolGuiPin pinEntryOpen;
    BoolGuiPin pinMouseDown;

    FloatGuiPin pinFloat; 
    StringGuiPin pinHint;

    StringGuiPin pinMenuItems;
    IntGuiPin pinMenuSelection;

    // Member variables 
    GmpiDrawing_API::MP1_POINT pointPrevious;
    GmpiDrawing_API::MP1_POINT pointPreviousColor;
    GmpiDrawing_API::MP1_POINT pointPreviousBrightness;
    GmpiDrawing_API::MP1_POINT pointPreviousBrightnessBot;
    GmpiDrawing_API::MP1_POINT pointOnMouseDown;
    GmpiGui::TextEdit nativeEdit;
    float previousValue = 0.0f;
    int precision = 2;
    int corner = 4;
    bool dblClickState = false;
       
public:
    ValueBoxGui()
        : pointPrevious({ 0, 0 }), //initializing variables
        pointPreviousColor({ 0, 0 }), pointOnMouseDown({ 0, 0 }), pointPreviousBrightness({ 0, 0 }), pointPreviousBrightnessBot({ 0, 0 })
    {
        initializePin(pinAnimPos, static_cast<MpGuiBaseMemberPtr2>(&ValueBoxGui::onSetAnimPos));
        initializePin(pinAnimPosColor, static_cast<MpGuiBaseMemberPtr2>(&ValueBoxGui::onSetAnimPosColor));
        initializePin(pinBrightness);
        initializePin(pinBrightnessBot);

        initializePin(pinMin);
        initializePin(pinMax);

        initializePin(pinHelp);
        initializePin(pinColorHex);

        initializePin(pinFontSize, static_cast<MpGuiBaseMemberPtr2>(&ValueBoxGui::onSetFontSize));
        initializePin(pinFontFace, static_cast<MpGuiBaseMemberPtr2>(&ValueBoxGui::onSetFontFace));
        initializePin(pinTextColor, static_cast<MpGuiBaseMemberPtr2>(&ValueBoxGui::onSetTextColor));


        initializePin(pinPrecision, static_cast<MpGuiBaseMemberPtr2>(&ValueBoxGui::onSetPrecision));
        initializePin(pinCorner, static_cast<MpGuiBaseMemberPtr2>(&ValueBoxGui::onSetCorner));

        initializePin(pinEntryOpen);
        initializePin(pinMouseDown);

        initializePin(pinFloat);
        initializePin(pinHint);

        initializePin(pinMenuItems);
        initializePin(pinMenuSelection);

    }

    virtual int32_t MP_STDCALL populateContextMenu(float x, float y, gmpi::IMpUnknown* contextMenuItemsSink) override
    {
        gmpi::IMpContextItemSink* sink = nullptr;
        contextMenuItemsSink->queryInterface(gmpi::MP_IID_CONTEXT_ITEMS_SINK, reinterpret_cast<void**>(&sink));

        it_enum_list itr(pinMenuItems);

        for (itr.First(); !itr.IsDone(); ++itr)
        {
            int32_t flags = 0;

            // Special commands (sub-menus)
            switch (itr.CurrentItem()->getType())
            {
            case enum_entry_type::Separator:
            case enum_entry_type::SubMenu:
                flags |= gmpi_gui::MP_PLATFORM_MENU_SEPARATOR;
                break;

            case enum_entry_type::SubMenuEnd:
            case enum_entry_type::Break:
                continue;

            default:
                break;
            }

            sink->AddItem(JmUnicodeConversions::WStringToUtf8(itr.CurrentItem()->text).c_str(), itr.CurrentItem()->value, flags);
        }
        sink->release();
        return gmpi::MP_OK;
    }

    virtual int32_t MP_STDCALL onContextMenu(int32_t selection) override
    {
        pinMenuSelection = selection; // send menu momentarily, then reset.
        pinMenuSelection = -1;

        return gmpi::MP_OK;
    }

    void onSetAnimPos()
    {
        onSetFloat();
    }
    void onSetAnimPosColor()
    {
        AnimPosToHex();
    }
    void onSetFontSize()
    {
        //pinFontSize.getValue();
    }
    void onSetFontFace()
    {
       //pinFontFace.getValue();
    }

    void onSetTextColor()
    {
        invalidateRect();
    }

    void onSetPrecision()
    {
        precision = pinPrecision.getValue();
    }
    void onSetCorner()
    {
        corner = pinCorner.getValue();
    }

    void onSetBrightness()
    {
    }

    void onSetMouseDown()
    {
    }

    int32_t MP_STDCALL getToolTip(GmpiDrawing_API::MP1_POINT point, gmpi::IString* returnString) override
    {
        auto utf8String = (std::string)pinHint;
        returnString->setData(utf8String.data(), (int32_t)utf8String.size());
        return gmpi::MP_OK;
    }
private:
    int count = 0;
    bool processing = false;
public:
    int32_t MP_STDCALL onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
    {
        if (dblClickState)
        {
            dblClickState = false;
        }

        if ((flags & GG_POINTER_FLAG_FIRSTBUTTON) == 0)
        {
            return gmpi::MP_OK; // Indicate successful hit, so right-click menu can show.
        }

        setCapture();
        pinMouseDown = true;
        pointPrevious = point;
        pointOnMouseDown = point;
        if (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT)
        {
            if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL)
            {
                pointPreviousBrightness = point;
                pointPreviousBrightnessBot = point;
            }
            else{ pointPreviousColor = point; }            
        }
        pinHelp = "MouseDown (setCapture())";
        if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL)
        {
            if (pinMouseDown)
            {
                ++count;
                int countDown = 350;
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
                    dblClickState = true;
                }
                    if (count == 3)
                {
                    pinAnimPos = 0.f;
                    onSetFloat();
                    // Check if we are already processing
                    if (processing)
                    {
                        return MP_UNHANDLED; // Ignore further clicks while processing
                    }

                    processing = true; // Set flag to indicate processing has started
                }
                // Reset the processing flag to allow future clicks
                processing = false;
            }
        }

        return gmpi::MP_OK;
    }

    int32_t onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point)
    {
        GmpiDrawing_API::MP1_POINT pointOnMouseUp;
        pointOnMouseUp = point;
        
        if (!getCapture())
        {
            return gmpi::MP_UNHANDLED;
        }

        releaseCapture();
        pinMouseDown = false;
        pinHelp = "MouseUp (releaseCapture())";

        if ((pointOnMouseUp.x == pointOnMouseDown.x && pointOnMouseUp.y == pointOnMouseDown.y) && !((flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT) || (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL)) )
        {
            GmpiGui::GraphicsHost host(getGuiHost());
            nativeEdit = host.createPlatformTextEdit(getRect());
            nativeEdit.SetText(textValue);
            pinEntryOpen = true;
            pinHelp = "Entry is open";
            previousValue = pinAnimPos;
            // Show the edit control
            nativeEdit.ShowAsync([this](int32_t result) { this->OnTextEnteredComplete(result); });
        }                
        
        return gmpi::MP_OK;
    }

    void OnTextEnteredComplete(int32_t result)
    {
        if (result == gmpi::MP_OK)
        {
            // Update pinText with the current content of the text edit
            textValue = (nativeEdit.GetText()); // Get the latest text after editing                      
            invalidateRect(); // Request UI redraw if necessary
            pinEntryOpen = false;
            // Release native edit instance
            nativeEdit.setNull();
            pinHelp = "Entry is closed";
        }

        try {
            float floatValue = std::stof(textValue);
            pinAnimPos = floatValue;           
            // After successful conversion, update the display text to reflect the new value
            onSetFloat();
        }
        catch (const std::invalid_argument&) 
        {            
        }

        //clamp
        if (pinAnimPos < pinMin.getValue())
        {
            pinAnimPos = previousValue;
            pinHelp = "Out of range. Reverted to the previous value";
        }
        if (pinAnimPos > pinMax.getValue())
        {
            pinAnimPos = previousValue;
            pinHelp = "Out of range. Reverted to the previous value";
        }

        onSetFloat(); //to update the text box value
        
        invalidateRect();
    }

    int32_t MP_STDCALL onMouseWheel(int32_t flags, int32_t delta, MP1_POINT point) override
    {        
        float coarseness = 1200.0f;

        if (precision == 0 && !(flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT))
        {
            coarseness = 120.f;
            pinHelp = "The value changes in integer steps";
        }

        if (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT)
        {  
            if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL)
            {
                if (dblClickState)
                {
                    float new_pos = pinBrightnessBot;
                    new_pos = std::clamp(new_pos + delta / 12000.0f, 0.0f, 10.0f);
                    pinBrightnessBot = new_pos;
                    pinHelp = "Fine adjusting of the bottom brightness with the mouse wheel (dlkClk+Shift+Ctrl+Mouse Wheel)";
                }
                else
                {
                    float new_pos = pinBrightness;
                    new_pos = std::clamp(new_pos + delta / 12000.0f, 0.0f, 10.0f);
                    pinBrightness = new_pos;
                    pinHelp = "Fine adjusting of the top brightness with the mouse wheel (Shift+Ctrl+Mouse Wheel)";
                }               
            }
            else
            {
                float new_pos = pinAnimPosColor;
                new_pos = std::clamp(new_pos + delta / 18420.f, 0.0f, 10.0f);
                pinAnimPosColor = new_pos;
                pinHelp = "Fine adjusting of the color with the mouse wheel (Shift+Mouse Wheel)"; 
                AnimPosToHex();
            }                        
        }
        else 
        { 
            if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL) // <cntr> key magnifies
            {
                coarseness = 12000.0f;
                pinHelp = "Fine tuning the value with the mouse wheel (Ctrl+Mouse Wheel)";
            }
            else{ pinHelp = "Adjusting the value with the mouse wheel (Mouse Wheel)"; }

            float new_pos = pinAnimPos;
            new_pos = std::clamp(new_pos + delta / coarseness, pinMin.getValue(), pinMax.getValue());
            pinAnimPos = new_pos; 
        }
        onSetFloat();
        return gmpi::MP_OK;
    }

    int32_t MP_STDCALL onPointerMove(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
    {
        if (!getCapture())
        {
            return gmpi::MP_UNHANDLED;
        }

        float coarseness = (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL) ? 0.005f : 0.02f;
        constexpr float brightnessMin = 0.f;
        constexpr float brightnessMax = 10.f;

        constexpr float colorMin = 0.f;
        constexpr float colorMax = 10.f;

        // Lambda to handle position update
        auto handlePointerMove = [&](GmpiDrawing_API::MP1_POINT currentPoint, FloatGuiPin& pinTarget, float minVal, float maxVal, GmpiDrawing_API::MP1_POINT& prevPoint)
            {
                Point offset(currentPoint.x - prevPoint.x, currentPoint.y - prevPoint.y);
                float new_pos = pinTarget;
                new_pos = std::clamp(new_pos - coarseness * (float)offset.y, minVal, maxVal);
                pinTarget = new_pos;
                prevPoint = currentPoint;
            };

        if (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT)
        {
            if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL)
            {
                if (dblClickState)
                {
                    coarseness = 0.02f;
                    handlePointerMove(point, pinBrightnessBot, brightnessMin, brightnessMax, pointPreviousBrightnessBot);
                    pinHelp = "Adjusting the bottom brightness (dblClick+Shift+Ctrl+Drag)";
                }
                else
                {
                    // Adjust top brightness with its range
                    coarseness = 0.02f;
                    handlePointerMove(point, pinBrightness, brightnessMin, brightnessMax, pointPreviousBrightness);
                    pinHelp = "Adjusting the top brightness (Shift+Ctrl+Drag)";
                }
            }
            else
            {
                // Adjust color with its range
                handlePointerMove(point, pinAnimPosColor, colorMin, colorMax, pointPreviousColor);
                pinHelp = "Adjusting the color (Shift+Drag)";
            }
            AnimPosToHex();
        }
        else
        {
            if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL)
            {
                pinHelp = "Fine tuning the value (Ctrl+Drag)";
            }
            else
            {
                pinHelp = "Adjusting the value (Drag)";
            }
            // For pinAnimPos, use its min/max
            handlePointerMove(point, pinAnimPos, pinMin.getValue(), pinMax.getValue(), pointPrevious);
        }

        onSetFloat();

        return gmpi::MP_OK;
    }

    int32_t AnimPosToHex()
    {
        float x = pinAnimPosColor * 153.5f;
        int R = 0; int G = 0; int B = 0;
        //--------------------------------
        if ((x >= 255) && (x < 768))
            G = 255;
        if (x > 1023)
            G = 0;

        if ((x >= 1279) || (x < 255))
            R = 255;
        if ((x > 510) && (x <= 1024))
            R = 0;

        if ((x >= 767) && (x < 1280))
            B = 255;
        if (x <= 511)
            B = 0;

        //-----------------------------
        if ((x >= 0) && (x <= 255))
        { G = x; }
        //-------------------
        if ((x >= 256) && (x <= 511))
        { R = 255 - (x - 256); }
        //-------------------
        if ((x >= 512) && (x <= 767))
        { B = x - 512; }
        //-------------------
        if ((x >= 768) && (x <= 1023))
        { G = 255 - (x - 768); }
        //-------------------
        if ((x >= 1024) && (x <= 1279))
        { R = x - 1024; }
        //-------------------
        if ((x >= 1280) && (x <= 1535))
        { B = 255 - (x - 1280); }

        // Convert to hex string
        std::stringstream ssR, ssG, ssB;
        ssR << std::setfill('0') << std::setw(2) << std::hex << (int)R;
        ssG << std::setfill('0') << std::setw(2) << std::hex << (int)G;
        ssB << std::setfill('0') << std::setw(2) << std::hex << (int)B;

        std::string resA = "ff";
        pinColorHex = resA + ssR.str() + ssG.str() + ssB.str();

        return gmpi::MP_OK;
    }

    void onSetFloat() //delivering float value to the text box (requires <iomanip> and <sstream> headers)
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(precision) << pinAnimPos;
        textValue = ss.str();
        pinFloat = pinAnimPos;
        invalidateRect();//added for 1.6 (in 1.5 it does not seem to be necessary)
    }

    std::string textValue;

    Color FromHexStringBackwardCompatible(const std::wstring& s)
    {
        constexpr float oneOver255 = 1.0f / 255.0f;

        wchar_t* stopString;
        uint32_t hex = wcstoul(s.c_str(), &stopString, 16);
        float alpha = (hex >> 24) * oneOver255;

        return Color(se_sdk::FastGamma::sRGB_to_float((hex >> 16) & 0xff), se_sdk::FastGamma::sRGB_to_float((hex >> 8) & 0xff), se_sdk::FastGamma::sRGB_to_float(hex & 0xff), alpha);
    }

    int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override
    {        
        Graphics g(drawingContext);
        ClipDrawingToBounds x(g, getRect());

        //a simple monocolored rectangle 
        //auto brushRect = g.CreateSolidColorBrush(Color::FromHexString(pinColorHex));
       // g.FillRectangle(getRect(), brushRect);

        //an advanced rectangle with the gradient       
                //======================================
        auto r = getRect();
        int width = r.right - r.left;
        int height = r.bottom - r.top;        

        auto topCol = FromHexStringBackwardCompatible(pinColorHex);
        auto botCol = topCol;

        auto adjustBrightness = [&](Color color, float brightness) -> Color {
            float r = std::clamp(color.r * brightness, 0.0f, 1.f);
            float g = std::clamp(color.g * brightness, 0.0f, 1.f);
            float b = std::clamp(color.b * brightness, 0.0f, 1.f);
            float a = color.a; // Keep alpha unchanged
            return Color(r, g, b, a);
            };

        auto topColorBright = adjustBrightness(topCol, pinBrightness / 3.333f);
        auto bottomColorBright = adjustBrightness(botCol, pinBrightnessBot / 3.333f);

        int radius = corner;

        radius = (std::min)(radius, width / 2);
        radius = (std::min)(radius, height / 2);

        auto geometry = g.GetFactory().CreatePathGeometry();
        auto sink = geometry.Open();

        // define a corner 
        const float rightAngle = 3.14159265358979323846 * 0.5f;
        // top left
        if (5)
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
        if (5)
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
        if (5)
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
        if (5)
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

        Point point1(1, 0);
        Point point2(1, height);


        GradientStop gradientStops[]
        {
        { 0.0f, topColorBright },
        { 1.0f, bottomColorBright },
        };


        auto gradientBrush = g.CreateLinearGradientBrush(gradientStops, point1, point2);

        g.FillGeometry(geometry, gradientBrush);


        //=============================================================
        
        // 
        std::string str = { pinFontFace };
        const char* fontFace = str.c_str();
        TextFormat tf = g.GetFactory().CreateTextFormat(pinFontSize, fontFace);
        tf.SetParagraphAlignment(ParagraphAlignment::Center);
        tf.SetTextAlignment(TextAlignment::Center);
        auto brush = g.CreateSolidColorBrush(Color::FromHexString(pinTextColor));
        g.DrawTextU(textValue, tf, getRect(), brush);
        invalidateRect();
        return gmpi::MP_OK;
    }
};

namespace
{
    auto r = Register<ValueBoxGui>::withId(L"My ValueBox");
}
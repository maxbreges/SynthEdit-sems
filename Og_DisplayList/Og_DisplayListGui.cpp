#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include "it_enum_list.h"
#include "mp_gui.h"
#include <sstream>
#include <iomanip>

using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;

GmpiGui::PopupMenu nativeMenu;
GmpiDrawing_API::MP1_POINT pointPrevious;

class Og_DisplayListGui final : public gmpi_gui::MpGuiGfxBase
{   
 	void onSetBgColor()
	{invalidateRect();}
 	void onSetColor()
	{
        updateColor(); invalidateRect();}

 	void onSetTextColor()
	{invalidateRect();}
 	void onSetTextFont()
	{invalidateRect();}
 	void onSetFontSize()
	{invalidateRect();}

 	void onSetChoice()
	{
		it_enum_list it(pinItemList);
		it.FindValue(pinChoice);
        pinSelection = it.CurrentItem()->text;
		onSetText();
		invalidateRect();
	}

    void onSetSelection(){}

    std::string macText;
    void onSetText()
    {
        macText = pinSelection;        
        invalidateRect();
    }

 	void onSetItemList()
	{invalidateRect();}

    float corner = 5;
    void onSetCornerRadius()
    {
        corner = pinCornerRadius;
        invalidateRect();
    }

    void onSetCornerOn()
    {invalidateRect();}

 	StringGuiPin pinBgColor;
 	StringGuiPin pinColor;
 	StringGuiPin pinTextColor;
 	StringGuiPin pinTextFont;
 	FloatGuiPin pinFontSize;
 	IntGuiPin pinChoice;
 	StringGuiPin pinItemList;
	StringGuiPin pinSelection; //internal pin
 	BoolGuiPin pinPopUpopen;
 	FloatGuiPin pinCornerRadius;
    BoolGuiPin pinCornerOn;

    BoolGuiPin pinUpdate;

    BoolGuiPin pinShiftClk;
    BoolGuiPin pinCtrlClk;

    StringGuiPin pinColorGlow;
    StringGuiPin pinColorOut;
    
public:
	Og_DisplayListGui()
	{
		initializePin( pinBgColor, static_cast<MpGuiBaseMemberPtr2>(&Og_DisplayListGui::onSetBgColor) );
		initializePin( pinColor, static_cast<MpGuiBaseMemberPtr2>(&Og_DisplayListGui::onSetColor) );

		initializePin( pinTextColor, static_cast<MpGuiBaseMemberPtr2>(&Og_DisplayListGui::onSetTextColor) );
		initializePin( pinTextFont, static_cast<MpGuiBaseMemberPtr2>(&Og_DisplayListGui::onSetTextFont) );
		initializePin( pinFontSize, static_cast<MpGuiBaseMemberPtr2>(&Og_DisplayListGui::onSetFontSize) );

		initializePin( pinChoice, static_cast<MpGuiBaseMemberPtr2>(&Og_DisplayListGui::onSetChoice) );
		initializePin( pinItemList, static_cast<MpGuiBaseMemberPtr2>(&Og_DisplayListGui::onSetItemList) );
		initializePin(pinSelection, static_cast<MpGuiBaseMemberPtr2>(&Og_DisplayListGui::onSetSelection));

		initializePin( pinPopUpopen );
		initializePin( pinCornerRadius, static_cast<MpGuiBaseMemberPtr2>(&Og_DisplayListGui::onSetCornerRadius) );
        initializePin(pinCornerOn, static_cast<MpGuiBaseMemberPtr2>(&Og_DisplayListGui::onSetCornerOn));

        initializePin(pinUpdate, static_cast<MpGuiBaseMemberPtr2>(&Og_DisplayListGui::onSetUpdate));

        initializePin(pinShiftClk);
        initializePin(pinCtrlClk);

        initializePin(pinColorGlow);
        initializePin(pinColorOut);
    }

    void onSetUpdate()
    {onSetChoice();}

    int32_t MP_STDCALL onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
    {
        // Let host handle right-clicks.
        if ((flags & GG_POINTER_FLAG_FIRSTBUTTON) == 0)
        {
            return gmpi::MP_OK; // Indicate successful hit, so right-click menu can show.
        }
        
        if ((flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL) && !(flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT))
        {
            pinCtrlClk = true;
            goto bypass;
        }
        if ((flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT) && !(flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL))
        {
            pinShiftClk = true;
            goto bypass;
        }

        if ((flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT) && (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL) || !(flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT) && !(flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL))
        {
            setCapture();
        }

    bypass:
        pinCtrlClk = false;
        pinShiftClk = false;

        return gmpi::MP_OK;
    }

    int32_t MP_STDCALL onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point)
    {
        if (getCapture() && !(flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL) && !(flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT))
        {
            releaseCapture();

            GmpiGui::GraphicsHost host(getGuiHost());
            nativeMenu = host.createPlatformMenu(Point(0, 0));
            nativeMenu.SetAlignment(TextAlignment::Leading);

            it_enum_list itr(pinItemList);
            const int popupMenuWrapRowCount = 32;
            int vertical_size = 0; // for collumns on tall menus.
            for (itr.First(); !itr.IsDone(); itr.Next())
            {
                int32_t flags = itr.CurrentItem()->value == pinChoice ? gmpi_gui::MP_PLATFORM_MENU_TICKED : 0;
                if (vertical_size++ == popupMenuWrapRowCount)
                {
                    flags |= gmpi_gui::MP_PLATFORM_MENU_BREAK;
                    vertical_size = 1;
                }

                auto& txt = itr.CurrentItem()->text;

                switch (itr.CurrentItem()->getType())
                {
                case enum_entry_type::Separator:
                    flags |= gmpi_gui::MP_PLATFORM_MENU_SEPARATOR;
                    break;

                case enum_entry_type::Break:
                    flags |= gmpi_gui::MP_PLATFORM_MENU_BREAK;
                    vertical_size = 1;
                    break;

                case enum_entry_type::SubMenu:
                    flags = gmpi_gui::MP_PLATFORM_SUB_MENU_BEGIN; // ignore ticked flag.
                    txt = txt.substr(4);
                    vertical_size = 0; // not quite right, loses count on parent menu.
                    break;

                case enum_entry_type::SubMenuEnd:
                    flags |= gmpi_gui::MP_PLATFORM_SUB_MENU_END;
                    break;

                case enum_entry_type::Normal:
                    break;
                }

                nativeMenu.AddItem(txt, itr.CurrentItem()->value, flags);
            }

            nativeMenu.ShowAsync([this](int32_t result) -> void { this->OnPopupComplete(result); });
        }

        releaseCapture();

        return gmpi::MP_OK;
    }

    void OnPopupComplete(int32_t result)
    {
        if (result == gmpi::MP_OK)
        {
            pinChoice = nativeMenu.GetSelectedId();
        }
        nativeMenu.setNull(); // release it.
    }

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
        
        //an advanced rectangle with the gradient       
                //======================================
        auto r = getRect();
        float width = r.right - r.left;
        float height = r.bottom - r.top;

        float radius = corner;

        radius = (std::min)(radius, width / 2);
        radius = (std::min)(radius, height / 2);

        auto geometry = g.GetFactory().CreatePathGeometry();
        auto sink = geometry.Open();

        // define a corner 
        const float rightAngle = 3.14159265358979323846 * 0.5f;
        // top left
        if (pinCornerOn)
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
        if (pinCornerOn)
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
        if (pinCornerOn)
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
        if (pinCornerOn)
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

        // glow added to gradient brush
        auto topCol = FromHexStringBackwardCompatible(pinColorOut);
        auto botCol = FromHexStringBackwardCompatible(pinBgColor);
        auto glowCol = GmpiDrawing::Color::FromHexString(pinColorGlow);
        GradientStop gradientStops[]
        {
            { 0.0f, topCol },
            { 0.33f, glowCol },
            { 0.5f, topCol },
            { 1.0f, botCol },
        };

        auto gradientStopCollection = g.CreateGradientStopCollection(gradientStops);
        auto Brush = g.CreateLinearGradientBrush(gradientStopCollection, point1, point2);
        auto outlineBrush = g.CreateSolidColorBrush(botCol);
        float thickness = 1.f;
        g.FillGeometry(geometry, Brush);
        g.DrawGeometry(geometry, outlineBrush, thickness);

/*        GradientStop gradientStops[] //basic gradient without glow
        {
        { 0.0f, topCol }, //topColorBright },
        { 1.0f, botCol },//bottomColorBright },
        };

        auto gradientBrush = g.CreateLinearGradientBrush(gradientStops, point1, point2);

        g.FillGeometry(geometry, gradientBrush);*/

        //=============================================================

        std::string str = { pinTextFont };
        const char* fontFace = str.c_str();
        TextFormat tf = g.GetFactory().CreateTextFormat(pinFontSize, fontFace);
        tf.SetParagraphAlignment(ParagraphAlignment::Center),
            tf.SetTextAlignment(TextAlignment::Center);
        auto brush = g.CreateSolidColorBrush(Color::FromHexString(pinTextColor));

#ifdef _WIN32
        g.DrawTextU(pinSelection, tf, getRect(), brush);
#else
        g.DrawTextU(macText, tf, getRect(), brush);
#endif

        return gmpi::MP_OK;
    }

    //======================================================================

    //Brightness class

        // Helper to convert hex string to uint32_t
    uint32_t hexStringToUint32(const std::string& hexStr)
    {
        uint32_t value = 0;
        std::stringstream ss;
        ss << std::hex << hexStr;
        ss >> value;
        return value;
    }

    // Helper to convert uint32_t to hex string
    std::string uint32ToHexString(uint32_t value, size_t width = 6)
    {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(width) << std::hex << value;
        return ss.str();
    }

    // Extract R, G, B components
    void extractRGB(uint32_t color, uint8_t& r, uint8_t& g, uint8_t& b)
    {
        r = (color >> 16) & 0xFF;
        g = (color >> 8) & 0xFF;
        b = color & 0xFF;
    }

    // Combine R, G, B into uint32
    uint32_t combineRGB(uint8_t r, uint8_t g, uint8_t b)
    {
        return (r << 16) | (g << 8) | b;
    }

    //copied from Button

// Inside Og_DisplayListGui class

// Helper to brighten a color (similar to ButtonGui)
    uint32_t brightenColor(uint32_t color, float factor)
    {
        uint8_t r, g, b;
        extractRGB(color, r, g, b);
        r = std::min<>(255, static_cast<int>(r * factor));
        g = std::min<>(255, static_cast<int>(g * factor));
        b = std::min<>(255, static_cast<int>(b * factor));
        return combineRGB(r, g, b);
    }

    // Adjust color brightness (darken or brighten)
    uint32_t adjustBrightness(uint32_t color, float brightness)
    {
        uint8_t r, g, b;
        extractRGB(color, r, g, b);
        r = static_cast<uint8_t>(std::min<>(255.0f, r * brightness));
        g = static_cast<uint8_t>(std::min<>(255.0f, g * brightness));
        b = static_cast<uint8_t>(std::min<>(255.0f, b * brightness));
        return combineRGB(r, g, b);
    }

    void updateColor() //can be cleaned up a bit to use just pure numbers
    {
        // Read input color
        std::string inputHex = pinColor;
        if (inputHex.size() >= 2 && inputHex[0] == '0' && (inputHex[1] == 'x' || inputHex[1] == 'X'))
            inputHex = inputHex.substr(2);

        uint32_t color = hexStringToUint32(inputHex);

        // Define brightness factors similar to ButtonGui
        float baseBrightness = 1.25f; // you can adjust this as needed
        float glowBrightnessFactor = 0.7f * 1.44f; // for a brighter glow

        // Determine brightness based on your logic (if any)
        float brightness, brightnessGlow;
 
           brightness = 0.55f * baseBrightness;
           brightnessGlow = 0.75f * baseBrightness;

        // Generate main color
        uint32_t adjustedColor = adjustBrightness(color, brightness);
        // Generate glow color by brightening the original color
        uint32_t glowColor = brightenColor(color, glowBrightnessFactor);

        // Convert colors to hex strings with opacity
        std::string resultHex = uint32ToHexString(adjustedColor, 8);
        std::string glowHex = uint32ToHexString(glowColor, 8);

        // Apply opacity
        uint8_t opacityHex = static_cast<uint8_t>(1.f * 255.0f);
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(opacityHex);
        std::string opacityStr = ss.str();

        if (resultHex.size() >= 8)
            resultHex.replace(0, 2, opacityStr);
        if (glowHex.size() >= 8)
            glowHex.replace(0, 2, opacityStr);

        // Set output pins
        pinColorOut = resultHex;
        pinColorGlow = glowHex;
    }
};

namespace
{
	auto r = Register<Og_DisplayListGui>::withId(L"My Og_DisplayList");
}
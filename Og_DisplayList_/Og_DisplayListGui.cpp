#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include "it_enum_list.h"
#include "mp_gui.h"

using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;

GmpiGui::PopupMenu nativeMenu;

class Og_DisplayListGui final : public gmpi_gui::MpGuiGfxBase
{

 	void onSetBgColor()
	{
        invalidateRect();
	}

 	void onSetColor()
	{
        invalidateRect();
	}

 	void onSetTextColor()
	{
        invalidateRect();
	}

 	void onSetTextFont()
	{
        invalidateRect();
	}

 	void onSetFontSize()
	{
        invalidateRect();
	}

 	void onSetChoice()
	{
				it_enum_list it(pinItemList);
		it.FindValue(pinChoice);
		if (it.IsDone())
		{
			pinSelection = std::wstring();
		}
		else
		{
			pinSelection = it.CurrentItem()->text;
            
		}

		onSetText();

		invalidateRect();
	}

    std::string macText;
    void onSetText()
    {
        macText = pinSelection;
        invalidateRect();
    }

 	void onSetItemList()
	{
        invalidateRect();
	}

    float corner = 5;
    void onSetCornerRadius()
    {
        corner = pinCornerRadius;
        invalidateRect();
    }

    void onSetCornerOn()
    {
        invalidateRect();
    }

 	StringGuiPin pinBgColor;
 	StringGuiPin pinColor;
 	StringGuiPin pinTextColor;
 	StringGuiPin pinTextFont;
 	FloatGuiPin pinFontSize;
 	IntGuiPin pinChoice;
 	StringGuiPin pinItemList;
	StringGuiPin pinSelection;
 	BoolGuiPin pinPopUpopen;
 	FloatGuiPin pinCornerRadius;
    BoolGuiPin pinCornerOn;

    BoolGuiPin pinReset;
    BoolGuiPin pinUpdate;

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
		initializePin(pinSelection);

		initializePin( pinPopUpopen );
		initializePin( pinCornerRadius, static_cast<MpGuiBaseMemberPtr2>(&Og_DisplayListGui::onSetCornerRadius) );
        initializePin(pinCornerOn, static_cast<MpGuiBaseMemberPtr2>(&Og_DisplayListGui::onSetCornerOn));

        initializePin(pinReset, static_cast<MpGuiBaseMemberPtr2>(&Og_DisplayListGui::onSetReset));
        initializePin(pinUpdate, static_cast<MpGuiBaseMemberPtr2>(&Og_DisplayListGui::onSetUpdate));
    }

    void onSetReset()
    {
        pinChoice = 0;
        onSetChoice();
    }
    void onSetUpdate()
    {
        onSetChoice();
    }

    int32_t MP_STDCALL initialize() override
    {
        onSetChoice();
        return gmpi::MP_OK;
    }

    int32_t MP_STDCALL onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
    {
        // Let host handle right-clicks.
        if ((flags & GG_POINTER_FLAG_FIRSTBUTTON) == 0)
        {
            return gmpi::MP_OK; // Indicate successful hit, so right-click menu can show.
        }

        setCapture();

        return gmpi::MP_OK;
    }

    int32_t MP_STDCALL onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point)
    {
        if (getCapture())
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
/*#if 1
            if (nativeMenu.GetSelectedId() == pinChoice)
            {
                pinChoice = -1;
            }
#endif*/
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

        auto topCol = FromHexStringBackwardCompatible(pinColor);
        auto botCol = FromHexStringBackwardCompatible(pinBgColor);

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

        GradientStop gradientStops[]
        {
        { 0.0f, topCol }, //topColorBright },
        { 1.0f, botCol },//bottomColorBright },
        };

        auto gradientBrush = g.CreateLinearGradientBrush(gradientStops, point1, point2);

        g.FillGeometry(geometry, gradientBrush);

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
};

namespace
{
	auto r = Register<Og_DisplayListGui>::withId(L"My Og_DisplayList");
}
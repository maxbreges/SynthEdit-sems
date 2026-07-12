#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include "mp_gui.h"

using namespace gmpi;
using namespace gmpi_gui; //for enum GG_POINTER_FLAGS
using namespace GmpiDrawing;
using namespace GmpiGui;

GmpiGui::TextEdit nativeEdit;

class TextEntry15Gui final : public gmpi_gui::MpGuiGfxBase
{
 	void onSetText()
	{
		invalidateRect();
	}

    void onSetFontFace()
    { 
    }

    int fontSize = 16;
    void onSetFontSize()
    {
        fontSize = pinFontSize;
    }

 	void onSetTextColor()
	{
        pinTextColor.getValue();
		invalidateRect();
	}

 	void onSetTopColor()
	{
		invalidateRect();
	}

 	void onSetBgColor()
	{
		invalidateRect();
	}

	int corner = 5;
	void onSetCorner()
	{
		corner = pinCorner.getValue();
		invalidateRect();
	}

 	void onSetHint()
	{
		// pinHint changed
	}

    void onSetDisableHint()
    {
        // pinHint changed
    }

 	void onSetMouseDown()
	{
		// pinMouseDown changed
	}

 	StringGuiPin pinText;
	StringGuiPin pinFontFace;
	IntGuiPin pinFontSize;
 	StringGuiPin pinTextColor;
 	StringGuiPin pinTopColor;
 	StringGuiPin pinBgColor;
	IntGuiPin pinCorner;
 	StringGuiPin pinHint;
    BoolGuiPin pinDisableHint;
 	BoolGuiPin pinMouseDown;
	BoolGuiPin pinHover;   
    BoolGuiPin pinEntryOpen;

public:
	TextEntry15Gui()
	{
		initializePin( pinText, static_cast<MpGuiBaseMemberPtr2>(&TextEntry15Gui::onSetText) );
		initializePin(pinFontFace, static_cast<MpGuiBaseMemberPtr2>(&TextEntry15Gui::onSetFontFace));
		initializePin(pinFontSize, static_cast<MpGuiBaseMemberPtr2>(&TextEntry15Gui::onSetFontSize));
		initializePin( pinTextColor, static_cast<MpGuiBaseMemberPtr2>(&TextEntry15Gui::onSetTextColor) );
		initializePin( pinTopColor, static_cast<MpGuiBaseMemberPtr2>(&TextEntry15Gui::onSetTopColor) );
		initializePin( pinBgColor, static_cast<MpGuiBaseMemberPtr2>(&TextEntry15Gui::onSetBgColor) );
		initializePin(pinCorner, static_cast<MpGuiBaseMemberPtr2>(&TextEntry15Gui::onSetCorner));
		initializePin( pinHint, static_cast<MpGuiBaseMemberPtr2>(&TextEntry15Gui::onSetHint) );
        initializePin(pinDisableHint, static_cast<MpGuiBaseMemberPtr2>(&TextEntry15Gui::onSetDisableHint));
		initializePin( pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&TextEntry15Gui::onSetMouseDown) );
		initializePin(pinHover);
        initializePin(pinEntryOpen);
	}

    int32_t setHover(bool isMouseOverMe) override
    {
        pinHover = isMouseOverMe;
        return gmpi::MP_UNHANDLED;
    }

    int32_t MP_STDCALL getToolTip(GmpiDrawing_API::MP1_POINT point, gmpi::IString* returnString) override
    {
        if (!pinDisableHint)
        {
            auto hintUser = pinHint.getValue();
            if (hintUser.empty())
            {
               // pinHint = pinHintAuto;//<Pin name="HintAuto" datatype="string" parameterId="0" parameterField="ShortName"/> — if audio
            }
            else
            {
                pinHint = hintUser;
            }
            auto utf8String = (std::string)pinHint;
            returnString->setData(utf8String.data(), (int32_t)utf8String.size());
            return gmpi::MP_OK;
        }
        else {}
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

        pinMouseDown = true;
        return gmpi::MP_OK;
    }

    int32_t onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point)
    {
        if (!getCapture())
        {
            return gmpi::MP_UNHANDLED;
        }

        releaseCapture();
        pinMouseDown = false;

            GmpiGui::GraphicsHost host(getGuiHost());
            nativeEdit = host.createPlatformTextEdit(getRect());
            nativeEdit.SetText(pinText);
            pinEntryOpen = true;
            // Show the edit control
            nativeEdit.ShowAsync([this](int32_t result) { this->OnTextEnteredComplete(result); });        

        return gmpi::MP_OK;
    }

    void OnTextEnteredComplete(int32_t result)
    {
        if (result == gmpi::MP_OK)
        {
            // Update pinText with the current content of the text edit
            pinText = (nativeEdit.GetText()); // Get the latest text after editing                      
            invalidateRect(); // Request UI redraw if necessary
        }
        // Release native edit instance
        nativeEdit.setNull();
        pinEntryOpen = false;
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

        //a simple monocolored rectangle 
        //auto brushRect = g.CreateSolidColorBrush(Color::FromHexString(pinColorHex));
       // g.FillRectangle(getRect(), brushRect);

        //an advanced rectangle with the gradient       
                //======================================
        auto r = getRect();
        int width = r.right - r.left;
        int height = r.bottom - r.top;

        auto topCol = FromHexStringBackwardCompatible(pinTopColor);
        auto botCol = FromHexStringBackwardCompatible(pinBgColor);

        auto adjustBrightness = [&](Color color, float brightness) -> Color {
            float r = std::clamp(color.r * brightness, 0.0f, 1.f);
            float g = std::clamp(color.g * brightness, 0.0f, 1.f);
            float b = std::clamp(color.b * brightness, 0.0f, 1.f);
            float a = color.a; // Keep alpha unchanged
            return Color(r, g, b, a);
            };

       // auto topColorBright = adjustBrightness(topCol, pinBrightness / 3.333f);
       // auto bottomColorBright = adjustBrightness(botCol, pinBrightnessBot / 3.333f);

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
        { 0.0f, topCol }, //topColorBright },
        { 1.0f, botCol },//bottomColorBright },
        };


        auto gradientBrush = g.CreateLinearGradientBrush(gradientStops, point1, point2);

        g.FillGeometry(geometry, gradientBrush);


        //=============================================================

        // 
        std::string str = { pinFontFace };
        const char* fontFace = str.c_str();
    TextFormat tf = g.GetFactory().CreateTextFormat(fontSize, fontFace);
    tf.SetParagraphAlignment(ParagraphAlignment::Center),
        tf.SetTextAlignment(TextAlignment::Center);
    auto brush = g.CreateSolidColorBrush(Color::FromHexString(pinTextColor));

    g.DrawTextU(pinText, tf, getRect(), brush);

    return gmpi::MP_OK;
    }

};

namespace
{
	auto r = Register<TextEntry15Gui>::withId(L"TextEntry15");
}

#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include <cmath>
#include <string>

using namespace std;
using namespace gmpi;
using namespace GmpiDrawing;

class mxXYTextGui final : public gmpi_gui::MpGuiGfxBase
{
 	void onSetText()
	{
        std::string pattern ( "" );
        std::string text = pinText;

        if (text == pattern)
        {
            pinShapeARGB = "00000000";
            pinLineARGB = "00000000";
        }
        if (text != pattern)
        {
            pinShapeARGB = "FFFFFFFF";
            pinLineARGB = "FF000000";
        }
        invalidateRect();
	}

 	void onSetPositionX()
	{
        invalidateRect();
	}

    void onSetPositionY()
    {
        invalidateRect();
    }

 	void onSetFont()
	{
        invalidateRect();
	}

 	void onSetFontSize()
	{
        invalidateRect();
	}

    void onSetLineSize()
    {
        invalidateRect();
    }

    void onSetHeight()
    {
        invalidateRect();
    }

    void onSetWidth()
    {
        invalidateRect();
    }

    void onSetFontYAdjust()
    {
        invalidateRect();
    }

    void onSetShape()
    {
        invalidateRect();
    }

    void onSetLineARGB()
    {
        invalidateRect();
    }

    void onSetShapeARGB()
    {
        invalidateRect();
    }

    void onSetFontARGB()
    {
        invalidateRect();
    }

    void onSetMouseDown()
    {
        //invalidateRect();
    }

 	StringGuiPin pinText;
 	FloatGuiPin pinPositionX;
    FloatGuiPin pinPositionY;
 	StringGuiPin pinFont;
 	FloatGuiPin pinFontSize;
    FloatGuiPin pinLineSize;
    FloatGuiPin pinHeight;
    FloatGuiPin pinWidth;
    FloatGuiPin pinFontYAdjust;
    BoolGuiPin pinShape;
    StringGuiPin pinLineARGB;
    StringGuiPin pinShapeARGB;
    StringGuiPin pinFontARGB;
    BoolGuiPin pinMouseDown;

public:
	mxXYTextGui()
	{
		initializePin( pinText, static_cast<MpGuiBaseMemberPtr2>(&mxXYTextGui::onSetText) );
		initializePin( pinPositionX, static_cast<MpGuiBaseMemberPtr2>(&mxXYTextGui::onSetPositionX) );
        initializePin(pinPositionY, static_cast<MpGuiBaseMemberPtr2>(&mxXYTextGui::onSetPositionY));
		initializePin( pinFont, static_cast<MpGuiBaseMemberPtr2>(&mxXYTextGui::onSetFont) );
		initializePin( pinFontSize, static_cast<MpGuiBaseMemberPtr2>(&mxXYTextGui::onSetFontSize) );
        initializePin(pinLineSize, static_cast<MpGuiBaseMemberPtr2>(&mxXYTextGui::onSetLineSize));
        initializePin(pinHeight, static_cast<MpGuiBaseMemberPtr2>(&mxXYTextGui::onSetHeight));
        initializePin(pinWidth, static_cast<MpGuiBaseMemberPtr2>(&mxXYTextGui::onSetWidth));
        initializePin(pinFontYAdjust, static_cast<MpGuiBaseMemberPtr2>(&mxXYTextGui::onSetFontYAdjust));
        initializePin(pinShape, static_cast<MpGuiBaseMemberPtr2>(&mxXYTextGui::onSetShape));
        initializePin(pinLineARGB, static_cast<MpGuiBaseMemberPtr2>(&mxXYTextGui::onSetLineARGB));
        initializePin(pinShapeARGB, static_cast<MpGuiBaseMemberPtr2>(&mxXYTextGui::onSetShapeARGB));
        initializePin(pinFontARGB, static_cast<MpGuiBaseMemberPtr2>(&mxXYTextGui::onSetFontARGB));
        initializePin(pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&mxXYTextGui::onSetMouseDown));
	}

    int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override
    {
        Graphics g(drawingContext);
        Rect r = getRect();
        float lineSize = pinLineSize.getValue();
        float textboxwidth = pinWidth.getValue() - lineSize;
        float textboxheight = pinHeight.getValue() - lineSize;
        float halfLineSize = lineSize * 0.5f;
        float normalisedX = (max)(0.0f, (min)(1.0f, pinPositionX.getValue()));
        float normalisedY = 1.0f - (max)(0.0f, (min)(1.0f, pinPositionY.getValue()));
        float adjustedWidth = r.getWidth() - (textboxwidth + halfLineSize * 2.0f);
        float adjustedHeight = r.getHeight() - (textboxheight + halfLineSize * 2.0f);

        float x = halfLineSize + normalisedX * adjustedWidth;
        float y = halfLineSize + normalisedY * adjustedHeight;

        GmpiDrawing::Rect text_rect(x, pinFontYAdjust + y, x + textboxwidth, pinFontYAdjust + y + textboxheight);

        auto brushrectline = g.CreateSolidColorBrush(Color::FromHexString(pinLineARGB));
        auto brushshape = g.CreateSolidColorBrush(Color::FromHexString(pinShapeARGB));
        float tboxwidthhlf = textboxwidth * .5f;
        float tboxheighthlf = textboxheight * .5f;

        {
            Point center = { x + tboxwidthhlf,y + tboxheighthlf };
            GmpiDrawing::Ellipse circle(center, tboxwidthhlf, tboxheighthlf);
            g.FillEllipse(circle, brushshape);
            g.DrawEllipse(circle, brushrectline, pinLineSize);
        }
        float font_size = pinFontSize;
        std::string str = { pinFont };
        const char* fontFace = str.c_str();
        GmpiDrawing_API::MP1_FONT_WEIGHT fontWeight = GmpiDrawing_API::MP1_FONT_WEIGHT_HEAVY;
        auto textFormat = GetGraphicsFactory().CreateTextFormat(font_size, fontFace, fontWeight);
        textFormat.SetTextAlignment(TextAlignment::Center);
        textFormat.SetParagraphAlignment(ParagraphAlignment::Center);
        auto brushfont = g.CreateSolidColorBrush(Color::FromHexString(pinFontARGB));
        g.DrawTextW(pinText, textFormat, text_rect, brushfont, (int32_t)DrawTextOptions::Clip);

        return gmpi::MP_OK;
    }
};

namespace
{
	auto r = Register<mxXYTextGui>::withId(L"mxXYText");
}

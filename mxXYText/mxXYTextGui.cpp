#include "mp_sdk_gui2.h"
#include "../shared/xplatform_modifier_keys.h"
//#include "../shared/it_enum_list.h"
#include "../shared/unicode_conversion.h"
#include <cmath>

using namespace std;
using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;
using namespace JmUnicodeConversions;

class mxXYTextGui final : public gmpi_gui::MpGuiGfxBase
{
 	void onSetText()
	{
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

	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext ) override
    {
        Graphics g(drawingContext);
        ClipDrawingToBounds z(g, getRect());
        Rect r = getRect();

        // Get control dimensions
        float lineSize = pinLineSize.getValue();
        float halfLineSize = lineSize * 0.5f;
        float textboxwidth = pinWidth.getValue() - lineSize;
        float textboxheight = pinHeight.getValue() - lineSize;
        float tboxwidthhlf = textboxwidth * .5f;
        float tboxheighthlf = textboxheight * .5f;
        // Ensure normalized positions
        float normalisedX = (max)(0.0f, (min)(1.0f, pinPositionX.getValue()));
        float normalisedY = 1.0f - (max)(0.0f, (min)(1.0f, pinPositionY.getValue()));
        // Adjust for padding and border size
        float adjustedWidth = r.getWidth() - (textboxwidth + halfLineSize * 2.0f);
        float adjustedHeight = r.getHeight() - (textboxheight + halfLineSize * 2.0f);
        float x = halfLineSize + normalisedX * adjustedWidth;
        float y = halfLineSize + normalisedY * adjustedHeight;
        // Rectangle for the text box
        GmpiDrawing::Rect dest_rect(x, y, x + textboxwidth, y + textboxheight);
        // Rectangle for text (with vertical adjustment if needed)
        GmpiDrawing::Rect text_rect(x, pinFontYAdjust + y, x + textboxwidth, pinFontYAdjust + y + textboxheight);
        // Draw rectangle with correct line size
        auto brushrectline = g.CreateSolidColorBrush(Color::FromHexString(pinLineARGB));
        auto brushshape = g.CreateSolidColorBrush(Color::FromHexString(pinShapeARGB));

        if (!pinShape)
        {
            Point center = { x + tboxwidthhlf,y + tboxheighthlf };
            GmpiDrawing::Ellipse circle(center, tboxwidthhlf, tboxheighthlf);
            g.FillEllipse(circle, brushshape);
            g.DrawEllipse(circle, brushrectline, pinLineSize);
        }
        else
        {

            g.FillRectangle(dest_rect, brushshape);
            g.DrawRectangle(dest_rect, brushrectline, pinLineSize);
        }

        // Start Font Draw: setting QUICK AND DIRTY!!! YOU CAN USE STYLES FROM META DATA!!
        std::string str = pinFont;
        int font_size_ = pinFontSize.getValue();
        GmpiDrawing_API::MP1_FONT_WEIGHT fontWeight = GmpiDrawing_API::MP1_FONT_WEIGHT_HEAVY;
        const char* fontFace = str.c_str();
        float dipFontSize = (max(font_size_, 1.f));
        TextFormat textFormat = g.GetFactory().CreateTextFormat(dipFontSize, fontFace, fontWeight);
        textFormat.SetTextAlignment(TextAlignment::Center);
        textFormat.SetParagraphAlignment(ParagraphAlignment::Center);
        textFormat.SetImprovedVerticalBaselineSnapping();
        textFormat.SetWordWrapping(WordWrapping::Wrap);
        auto brushfont = g.CreateSolidColorBrush(Color::FromHexString(pinFontARGB));
        g.DrawTextW(pinText, textFormat, text_rect, brushfont, (int32_t)DrawTextOptions::Clip);
        // End FONT Draw //


        return MP_OK;
    }
};

namespace
{
	auto r = Register<mxXYTextGui>::withId(L"mxXYText");
}

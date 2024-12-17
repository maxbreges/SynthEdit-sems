#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include <cmath>
#include <string>

using namespace std;
using namespace gmpi;
using namespace GmpiDrawing;

class MacTestGui final : public gmpi_gui::MpGuiGfxBase
{
 	void onSetText()
	{
		// pinText changed
	}

 	void onSetPositionX()
	{
		// pinPositionX changed
	}

 	void onSetPositionY()
	{
		// pinPositionY changed
	}

 	void onSetFont()
	{
		// pinFont changed
	}

 	void onSetFontSize()
	{
		// pinFontSize changed
	}

 	void onSetLineSize()
	{
		// pinLineSize changed
	}

 	void onSetHeight()
	{
		// pinHeight changed
	}

 	void onSetWidth()
	{
		// pinWidth changed
	}

 	void onSetFontYAdjust()
	{
		// pinFontYAdjust changed
	}

 	void onSetShape()
	{
		// pinShape changed
	}

 	void onSetLineARGB()
	{
		// pinLineARGB changed
	}

 	void onSetShapeARGB()
	{
		// pinShapeARGB changed
	}

 	void onSetFontARGB()
	{
		// pinFontARGB changed
	}

 	void onSetMouseDown()
	{
		// pinMouseDown changed
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
	MacTestGui()
	{
		initializePin( pinText, static_cast<MpGuiBaseMemberPtr2>(&MacTestGui::onSetText) );
		initializePin( pinPositionX, static_cast<MpGuiBaseMemberPtr2>(&MacTestGui::onSetPositionX) );
		initializePin( pinPositionY, static_cast<MpGuiBaseMemberPtr2>(&MacTestGui::onSetPositionY) );
		initializePin( pinFont, static_cast<MpGuiBaseMemberPtr2>(&MacTestGui::onSetFont) );
		initializePin( pinFontSize, static_cast<MpGuiBaseMemberPtr2>(&MacTestGui::onSetFontSize) );
		initializePin( pinLineSize, static_cast<MpGuiBaseMemberPtr2>(&MacTestGui::onSetLineSize) );
		initializePin( pinHeight, static_cast<MpGuiBaseMemberPtr2>(&MacTestGui::onSetHeight) );
		initializePin( pinWidth, static_cast<MpGuiBaseMemberPtr2>(&MacTestGui::onSetWidth) );
		initializePin( pinFontYAdjust, static_cast<MpGuiBaseMemberPtr2>(&MacTestGui::onSetFontYAdjust) );
		initializePin( pinShape, static_cast<MpGuiBaseMemberPtr2>(&MacTestGui::onSetShape) );
		initializePin( pinLineARGB, static_cast<MpGuiBaseMemberPtr2>(&MacTestGui::onSetLineARGB) );
		initializePin( pinShapeARGB, static_cast<MpGuiBaseMemberPtr2>(&MacTestGui::onSetShapeARGB) );
		initializePin( pinFontARGB, static_cast<MpGuiBaseMemberPtr2>(&MacTestGui::onSetFontARGB) );
		initializePin( pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&MacTestGui::onSetMouseDown) );
	}

	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext ) override
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

		auto textFormat = GetGraphicsFactory().CreateTextFormat();
		auto brush = g.CreateSolidColorBrush(Color::Black);
		auto brushfont = g.CreateSolidColorBrush(Color::FromHexString(pinFontARGB));
		g.DrawTextW(pinText, textFormat, text_rect, brushfont, (int32_t)DrawTextOptions::Clip);


		return gmpi::MP_OK;
	}
};

namespace
{
	auto r = Register<MacTestGui>::withId(L"MacTest");
}

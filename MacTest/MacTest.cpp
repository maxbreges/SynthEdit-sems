#include "mp_sdk_gui2.h"
#include "Drawing.h"

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

		auto textFormat = GetGraphicsFactory().CreateTextFormat();
		auto brush = g.CreateSolidColorBrush(Color::Red);

		g.DrawTextU("Hello World!", textFormat, 0.0f, 0.0f, brush);

		return gmpi::MP_OK;
	}
};

namespace
{
	auto r = Register<MacTestGui>::withId(L"MacTest");
}

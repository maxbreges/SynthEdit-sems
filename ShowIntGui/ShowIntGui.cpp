#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include <iomanip>
#include <sstream>

using namespace gmpi;
using namespace std;
using namespace GmpiDrawing;


class ShowFloatGui final : public gmpi_gui::MpGuiGfxBase
{
 	void onSetFloat()
	{
		std::stringstream ss;
		ss << std::fixed << pinFloat;
		std::string res(ss.str());

		pinText = res;
		pinFloatR = pinFloat;
		invalidateRect();
	}

	void onSetFloatR()
	{
		std::stringstream ss;
		ss << std::fixed << pinFloatR;
		std::string res(ss.str());

		pinText = res;
		pinFloat = pinFloatR;
		invalidateRect();
	}
 	void onSetTextColor()
	{
		// pinTextColor changed
	}

 	void onSetFont()
	{
		// pinFont changed
	}

	void onSetText()
	{	
		onSetFloat();
	}

 	IntGuiPin pinFloat;
	IntGuiPin pinFloatR;
 	StringGuiPin pinTextColor;
 	StringGuiPin pinFont;
	StringGuiPin pinText;

public:
	ShowFloatGui()
	{
		initializePin( pinFloat, static_cast<MpGuiBaseMemberPtr2>(&ShowFloatGui::onSetFloat));
		initializePin( pinFloatR, static_cast<MpGuiBaseMemberPtr2>(&ShowFloatGui::onSetFloatR));
		initializePin( pinTextColor, static_cast<MpGuiBaseMemberPtr2>(&ShowFloatGui::onSetTextColor));
		initializePin( pinFont, static_cast<MpGuiBaseMemberPtr2>(&ShowFloatGui::onSetFont));
		initializePin(pinText, static_cast<MpGuiBaseMemberPtr2>(&ShowFloatGui::onSetText));
	}

	int32_t  measure(GmpiDrawing_API::MP1_SIZE availableSize, GmpiDrawing_API::MP1_SIZE* returnDesiredSize) override
	{
		*returnDesiredSize = availableSize;
		const float minSizeH = 32.f;
		const float minSizeW = 64.f;
		returnDesiredSize->height = (std::min)(returnDesiredSize->height, minSizeH);
		returnDesiredSize->width = (std::min)(returnDesiredSize->width, minSizeW);
		return gmpi::MP_OK;
	}
	int32_t MP_STDCALL arrange(GmpiDrawing_API::MP1_RECT finalRect) override
	{
		auto r = MpGuiGfxBase::arrange(finalRect);
		return r;
	}

	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override
	{
		Graphics g(drawingContext);

		std::string str = { pinFont };
		const char* fontFace = str.c_str();
		float font_size = 16;
		TextFormat textFormat = g.GetFactory().CreateTextFormat(font_size, fontFace);

		textFormat.SetParagraphAlignment(ParagraphAlignment::Center),

			textFormat.SetTextAlignment(TextAlignment::Center);

		auto brush = g.CreateSolidColorBrush(Color::FromHexString(pinTextColor));

		g.DrawTextU(pinText, textFormat, getRect(), brush, 0);

		return gmpi::MP_OK;
	}

};

namespace
{
	auto r = Register<ShowFloatGui>::withId(L"ShowIntGui");
}

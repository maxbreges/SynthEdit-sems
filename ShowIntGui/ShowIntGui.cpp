#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include <iomanip>
#include <sstream>

using namespace gmpi;
using namespace std;
using namespace GmpiDrawing;

int pinInt;

class ShowIntGui final : public gmpi_gui::MpGuiGfxBase
{
	void onSetInt()
	{
		std::stringstream ss;
		ss << std::fixed << pinInt;
		std::string res(ss.str());
		pinText = res;	
		invalidateRect();
	}
	void onSetIntL()
	{
		pinInt = pinIntL;
		onSetInt();
	}

	void onSetIntR()
	{
		pinInt = pinIntR;
		onSetInt();
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
		onSetInt();
	}
	
	IntGuiPin pinIntL;
	IntGuiPin pinIntR;
	StringGuiPin pinTextColor;
	StringGuiPin pinFont;
	StringGuiPin pinText;

public:
	ShowIntGui()
	{
		initializePin(pinIntL, static_cast<MpGuiBaseMemberPtr2>(&ShowIntGui::onSetIntL));
		initializePin(pinIntR, static_cast<MpGuiBaseMemberPtr2>(&ShowIntGui::onSetIntR));
		initializePin(pinTextColor, static_cast<MpGuiBaseMemberPtr2>(&ShowIntGui::onSetTextColor));
		initializePin(pinFont, static_cast<MpGuiBaseMemberPtr2>(&ShowIntGui::onSetFont));
		initializePin(pinText, static_cast<MpGuiBaseMemberPtr2>(&ShowIntGui::onSetText));
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
	auto r = Register<ShowIntGui>::withId(L"ShowIntGui");
}

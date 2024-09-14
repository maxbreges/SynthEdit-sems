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
		ss << std::fixed << std::setprecision(pinDecimals) << pinFloat;
		std::string res(ss.str());

		pinText = res;
		invalidateRect();

	}
	void onSetFloatR()
	{
		std::stringstream ssR;
		ssR << std::fixed << std::setprecision(pinDecimals) << pinFloatR;
		std::string resR(ssR.str());

		pinText = resR;
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
		//pinFloatR = pinFloat;
		invalidateRect();
	}

	void onSetDecimals()
	{
	}

	FloatGuiPin pinFloat;
	FloatGuiPin pinFloatR;
	StringGuiPin pinTextColor;
	StringGuiPin pinFont;
	StringGuiPin pinText;
	IntGuiPin pinDecimals;

public:
	ShowFloatGui()
	{
		initializePin(pinFloat, static_cast<MpGuiBaseMemberPtr2>(&ShowFloatGui::onSetFloat));
		initializePin(pinFloatR, static_cast<MpGuiBaseMemberPtr2>(&ShowFloatGui::onSetFloatR));
		initializePin(pinTextColor, static_cast<MpGuiBaseMemberPtr2>(&ShowFloatGui::onSetTextColor));
		initializePin(pinFont, static_cast<MpGuiBaseMemberPtr2>(&ShowFloatGui::onSetFont));
		initializePin(pinText, static_cast<MpGuiBaseMemberPtr2>(&ShowFloatGui::onSetText));
		initializePin(pinDecimals, static_cast<MpGuiBaseMemberPtr2>(&ShowFloatGui::onSetDecimals));
	}

	int32_t  measure(GmpiDrawing_API::MP1_SIZE availableSize, GmpiDrawing_API::MP1_SIZE* returnDesiredSize) override
	{
		*returnDesiredSize = availableSize;
		const float minSizeH = 24.f;
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
	auto r = Register<ShowFloatGui>::withId(L"Show Float");
}
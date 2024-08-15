#include "mp_sdk_gui2.h"
#include "Drawing.h"


using namespace gmpi;
using namespace GmpiDrawing;


class ShowTextGui final : public gmpi_gui::MpGuiGfxBase
{
 	void onSetText()
	{
		invalidateRect();
	}


 	StringGuiPin pinText;
	StringGuiPin pinTextColor;
	StringGuiPin pinFont;


public:
	ShowTextGui()
	{
		initializePin( pinText, static_cast<MpGuiBaseMemberPtr2>(&ShowTextGui::onSetText) );
		initializePin(pinTextColor, static_cast<MpGuiBaseMemberPtr2>(&ShowTextGui::onSetText));
		initializePin(pinFont, static_cast<MpGuiBaseMemberPtr2>(&ShowTextGui::onSetText));
	}

	int32_t  measure(GmpiDrawing_API::MP1_SIZE availableSize, GmpiDrawing_API::MP1_SIZE* returnDesiredSize) override
	{
		*returnDesiredSize = availableSize;
		const float minSizeH = 24.f;
		const float minSizeW = 80.f;
		returnDesiredSize->height = (std::min)(returnDesiredSize->height, minSizeH);
		returnDesiredSize->width = (std::min)(returnDesiredSize->width, minSizeW);
		return gmpi::MP_OK;
	}
	int32_t MP_STDCALL arrange(GmpiDrawing_API::MP1_RECT finalRect) override
	{
		auto r = MpGuiGfxBase::arrange(finalRect);
		return r;
	}

	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext ) override
	{
		Graphics g(drawingContext);
		
		auto r = getRect();

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
	auto r = Register<ShowTextGui>::withId(L"Show Text");
}

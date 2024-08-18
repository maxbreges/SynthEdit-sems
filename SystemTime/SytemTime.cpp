#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include "time.h"
#include <iomanip>
#include <sstream>


using namespace std;
using namespace gmpi;
using namespace GmpiDrawing;

class ClockGui final : public gmpi_gui::MpGuiGfxBase
{
 	void onSetText()
	{
		time_t _tm = time(NULL);
		struct tm* curtime = localtime(&_tm);
		std::stringstream ss;
		ss << std::fixed << asctime(curtime);
		std::string res(ss.str());
		pinText = res;
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


 	StringGuiPin pinBgColor;
 	StringGuiPin pinFont;
	StringGuiPin pinFontColor;
	StringGuiPin pinText;

public:
	ClockGui()
	{	
		initializePin( pinBgColor, static_cast<MpGuiBaseMemberPtr2>(&ClockGui::onSetTextColor) );
		initializePin( pinFont, static_cast<MpGuiBaseMemberPtr2>(&ClockGui::onSetFont) );
		initializePin(pinFontColor, static_cast<MpGuiBaseMemberPtr2>(&ClockGui::onSetFont));
		initializePin(pinText, static_cast<MpGuiBaseMemberPtr2>(&ClockGui::onSetText));
	}

	int32_t  measure(GmpiDrawing_API::MP1_SIZE availableSize, GmpiDrawing_API::MP1_SIZE* returnDesiredSize) override
	{
		*returnDesiredSize = availableSize;
		const float minSizeH = 24.f;
		const float minSizeW = 200.f;
		const float maxSizeH = 24.f;
		const float maxSizeW = 200.f;
		returnDesiredSize->height = (std::min)(returnDesiredSize->height, minSizeH);
		returnDesiredSize->width = (std::min)(returnDesiredSize->width, minSizeW);
		returnDesiredSize->height = (std::max)(returnDesiredSize->height, maxSizeH);
		returnDesiredSize->width = (std::max)(returnDesiredSize->width, maxSizeW);

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

		//textFormat.SetParagraphAlignment(ParagraphAlignment::Center),

			textFormat.SetTextAlignment(TextAlignment::Center);

		auto brush = g.CreateSolidColorBrush(Color::FromHexString(pinFontColor));

		g.DrawTextU(pinText, textFormat, getRect(), brush, 0);
		onSetText();
		return gmpi::MP_OK;
	}
};

namespace
{
	auto r = Register<ClockGui>::withId(L"Clock");
}

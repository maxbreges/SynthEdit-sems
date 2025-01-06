#include "mp_sdk_gui2.h"
#include "Drawing.h"

using namespace gmpi;
using namespace GmpiDrawing;

class FretLedGui final : public gmpi_gui::MpGuiGfxBase
{
	void onSetGate()
	{
		switch (pinGate)
		{
		case 0: pinLineColor = "00000000"; pinColor = "00000000"; break;
		case 1: pinLineColor = "FF000000"; pinColor = "FFFFFFFF"; break;
		}
	}

 	void onSetColor()
	{
		invalidateRect();
	}
	void onSetLineColor()
	{
		invalidateRect();
	}
	void onSetLineSize()
	{
		invalidateRect();
	}

	BoolGuiPin pinGate;
 	StringGuiPin pinColor;
	StringGuiPin pinLineColor;
	FloatGuiPin pinLineSize;

public:
	FretLedGui()
	{
		initializePin(pinGate, static_cast<MpGuiBaseMemberPtr2>(&FretLedGui::onSetGate));
		initializePin(pinColor, static_cast<MpGuiBaseMemberPtr2>(&FretLedGui::onSetColor));
		initializePin(pinLineColor, static_cast<MpGuiBaseMemberPtr2>(&FretLedGui::onSetLineColor));
		initializePin(pinLineSize, static_cast<MpGuiBaseMemberPtr2>(&FretLedGui::onSetLineSize));
	}


	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext ) override
	{
		Graphics g(drawingContext);
		auto r = getRect();
		auto brushshape = g.CreateSolidColorBrush(Color::FromHexString(pinColor));
		auto brushrectline = g.CreateSolidColorBrush(Color::FromHexString(pinLineColor));

		{
			Point center = { (r.left + r.right) * 0.5f, (r.top + r.bottom) * 0.5f };
			float radius = (std::min)(r.getWidth(), r.getHeight()) * 0.4f;
			GmpiDrawing::Ellipse circle(center, radius, radius);
			g.FillEllipse(circle, brushshape);
			g.DrawEllipse(circle, brushrectline, pinLineSize);
		}

		return gmpi::MP_OK;
	}
};

namespace
{
	auto r = Register<FretLedGui>::withId(L"FretLed");
}

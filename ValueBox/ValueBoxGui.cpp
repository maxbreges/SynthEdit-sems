#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include <iomanip>
#include <sstream>

using namespace gmpi;
using namespace gmpi_gui; //for enum GG_POINTER_FLAGS
using namespace GmpiDrawing;

GmpiDrawing_API::MP1_POINT pointPrevious; // x, y structed float object

class ValueBoxGui final : public gmpi_gui::MpGuiGfxBase
{ 
 	void onSetAnimationPosition()
	{
		// pinAnimationPosition changed
	}

 	FloatGuiPin pinAnimPos;

 	FloatGuiPin pinDebug;
	FloatGuiPin pinDebug2;

 	StringGuiPin pinHelp;

public:
	ValueBoxGui()
	{
		initializePin( pinAnimPos, static_cast<MpGuiBaseMemberPtr2>(&ValueBoxGui::onSetAnimationPosition) );
		
		initializePin( pinDebug);
		initializePin(pinDebug2);
		
		initializePin( pinHelp);
	}
	
	int32_t MP_STDCALL onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{
		// Let host handle right-clicks.
		if ((flags & GG_POINTER_FLAG_FIRSTBUTTON) == 0)
		{
			return gmpi::MP_OK; // Indicate successful hit, so right-click menu can show.
		}

		setCapture();

		pointPrevious = point; // note the first point so that if released and then dragged again it starts from the value it was released at
		
		pinHelp = "MouseDown — setCapture()";

		return gmpi::MP_OK;
	}

	int32_t onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point)
	{
		if (!getCapture())
		{
			return gmpi::MP_UNHANDLED;
		}

		releaseCapture();

		pinHelp = "MouseUp — releaseCapture()";

		return gmpi::MP_OK;
	}

	int32_t MP_STDCALL onPointerMove(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{
			if (!getCapture())
			{return gmpi::MP_UNHANDLED;}

			Point offset(point.x - pointPrevious.x, point.y - pointPrevious.y); 
			
			float new_pos = pinAnimPos;
			new_pos = std::clamp(new_pos - 0.005f * (float)offset.y, 0.0f, 1.0f);
			pinAnimPos = new_pos;
			pointPrevious = point;

			if (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT)
			{				
				pinDebug2 = pinAnimPos;
				pinHelp = "Shift+onPointerMove";				
			}
			else
			{
			pinDebug = pinAnimPos;
			pinHelp = "onPointerMove";
			}

			onSetFloat();

		return gmpi::MP_OK;
	}

	void onSetFloat()
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << pinAnimPos;
		std::string res(ss.str());
		value = res;
		invalidateRect();
	}

	std::string value;

	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext ) override
	{
		Graphics g(drawingContext);
		ClipDrawingToBounds x(g, getRect());

		TextFormat tf = g.GetFactory().CreateTextFormat(16, "Times New Roman");
		tf.SetParagraphAlignment(ParagraphAlignment::Center),
			tf.SetTextAlignment(TextAlignment::Center);
		auto brush = g.CreateSolidColorBrush(Color::Black);		
		g.DrawTextU(value, tf, getRect(), brush);
		
		return gmpi::MP_OK;
	}
};

namespace
{
	auto r = Register<ValueBoxGui>::withId(L"My ValueBox");
}

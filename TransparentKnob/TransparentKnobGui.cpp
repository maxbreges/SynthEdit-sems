#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include <iomanip>
#include <sstream>

using namespace gmpi;
using namespace gmpi_gui; //for enum GG_POINTER_FLAGS
using namespace GmpiDrawing;

GmpiDrawing_API::MP1_POINT pointPrevious;

class TransparentKnobGui final : public gmpi_gui::MpGuiGfxBase
{
	int precision = 2;
 	void onSetAnimationPosition()
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(precision) << pinAnimationPosition;
		std::string res(ss.str());
		if(!pinDisplayEnable)
		{
			//do nothing
		}
		else { value = res; }

		invalidateRect();
	}

 	void onSetDisplayEnable()
	{
	}
	void onSetRectangleColorEnable()
	{
		invalidateRect();
	}
 	void onSetMouseDown()
	{
	}
	void onSetColor()
	{
		pinColor = pinColor.getValue();
	}

 	FloatGuiPin pinAnimationPosition;
 	BoolGuiPin pinDisplayEnable;
	BoolGuiPin pinRectangleColorEnable;
	StringGuiPin pinColor;
 	BoolGuiPin pinMouseDown;
	StringGuiPin pinHelp;

public:
	TransparentKnobGui()
	{
		initializePin( pinAnimationPosition, static_cast<MpGuiBaseMemberPtr2>(&TransparentKnobGui::onSetAnimationPosition) );
		initializePin( pinDisplayEnable, static_cast<MpGuiBaseMemberPtr2>(&TransparentKnobGui::onSetDisplayEnable) );
		initializePin(pinRectangleColorEnable, static_cast<MpGuiBaseMemberPtr2>(&TransparentKnobGui::onSetRectangleColorEnable));
		initializePin(pinColor, static_cast<MpGuiBaseMemberPtr2>(&TransparentKnobGui::onSetColor));
		initializePin( pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&TransparentKnobGui::onSetMouseDown) );
		initializePin(pinHelp);
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
		pinMouseDown = true;
		return gmpi::MP_OK;
	}

	int32_t onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point)
	{
		if (!getCapture())
		{
			return gmpi::MP_UNHANDLED;
		}

		releaseCapture();
		pinMouseDown = false;
		return gmpi::MP_OK;
	}

	int32_t MP_STDCALL onMouseWheel(int32_t flags, int32_t delta, MP1_POINT point) override
	{
		float coarseness = 1200.0f ;
		pinHelp = "Rotating the mouse wheel";
		//		if (modifier_keys::isHeldCtrl()) // <cntr> key magnifies
		if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL) // <cntr> key magnifies
		{
			coarseness = 12000.0f;	
			pinHelp = "Ctrl key is held down";
		}
		if ((flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL) && (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT))// <cntr> key magnifies
		{
			coarseness = 120000.0f;
			precision = 3;
			pinHelp = "Ctrl and Shift keys are held down";
		}
		else { precision = 2; }

		float new_pos = pinAnimationPosition;

		new_pos = std::clamp(new_pos + delta / coarseness, 0.0f, 1.0f);

		pinAnimationPosition = new_pos;

		return gmpi::MP_OK;
	}

	int32_t MP_STDCALL onPointerMove(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{
		if (!getCapture())
		{
			return gmpi::MP_UNHANDLED;
		}
		float coarseness = 0.005f;
		pinHelp = "Click and drag";
		if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL) // <cntr> key magnifies
		{
			coarseness = 0.001f;
			pinHelp = "Ctrl key is held down";
		}
		if ((flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL) && (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT))// <cntr> key magnifies
		{
			coarseness = 0.0001f;
			precision = 3;
			pinHelp = "Ctrl and Shift keys are held down";
		}
		else { precision = 2; }

		Point offset(point.x - pointPrevious.x, point.y - pointPrevious.y);

		float new_pos = pinAnimationPosition;
		new_pos = std::clamp(new_pos - coarseness * (float)offset.y, 0.0f, 1.0f);
		pinAnimationPosition = new_pos;
		pointPrevious = point;

		return gmpi::MP_OK;
	}

	std::string value;

	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override
	{
		Graphics g(drawingContext);
		ClipDrawingToBounds x(g, getRect());
		auto brushRect = g.CreateSolidColorBrush(Color::FromHexString(pinColor));
		if (!pinRectangleColorEnable)
		{
			//do nothing
		}
		else {
			g.FillRectangle(getRect(), brushRect);
		}		

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
	auto r = Register<TransparentKnobGui>::withId(L"My Transparent Knob");
}

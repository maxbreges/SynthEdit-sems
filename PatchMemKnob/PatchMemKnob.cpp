#include "mp_sdk_gui2.h"
#include "Drawing.h"
//#define _USE_MATH_DEFINES
//#include "C:\Program Files (x86)\Windows Kits\10\Include\10.0.10150.0\ucrt\math.h"

using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;

GmpiDrawing_API::MP1_POINT pointPrevious;

class PatchMemKnobGui final : public gmpi_gui::MpGuiGfxBase
{
	void onSetScaleIn()
	{
		pinScaled = pinScaleIn;
	}
	void onSetAnimationIn()
	{
		pinAnimationPosition = pinAnimationIn;
	}
	void onSetScaled()
	{
		pinScaleIn = pinScaled;
	}
	void onSetAnimationPosition()
	{
		pinAnimationIn = pinAnimationPosition;
	}
	void onSetHexIn()
	{
	}
	void onSetHex()
	{
	}

public:
 	FloatGuiPin pinScaleIn;
 	FloatGuiPin pinAnimationIn;
 	FloatGuiPin pinScaled;
 	FloatGuiPin pinAnimationPosition;
	StringGuiPin pinHexIn;
	StringGuiPin pinHex;


	PatchMemKnobGui()
	{
		initializePin( pinScaleIn, static_cast<MpGuiBaseMemberPtr2>(&PatchMemKnobGui::onSetScaleIn) );
		initializePin( pinAnimationIn, static_cast<MpGuiBaseMemberPtr2>(&PatchMemKnobGui::onSetAnimationIn) );
		initializePin( pinScaled, static_cast<MpGuiBaseMemberPtr2>(&PatchMemKnobGui::onSetScaled) );
		initializePin( pinAnimationPosition, static_cast<MpGuiBaseMemberPtr2>(&PatchMemKnobGui::onSetAnimationPosition) );
		initializePin(pinHexIn, static_cast<MpGuiBaseMemberPtr2>(&PatchMemKnobGui::onSetHexIn));
		initializePin(pinHex, static_cast<MpGuiBaseMemberPtr2>(&PatchMemKnobGui::onSetHex));
	}

	int32_t MP_STDCALL onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{
		// Let host handle right-clicks.
		if ((flags & GG_POINTER_FLAG_FIRSTBUTTON) == 0)
		{
			return gmpi::MP_OK; // Indicate successful hit, so right-click menu can show.
		}

		pointPrevious = point;	// note first point.
	
		setCapture();

		return gmpi::MP_OK;
	}

	int32_t onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point)
	{
		if (!getCapture())
		{
			return gmpi::MP_UNHANDLED;
		}

		releaseCapture();

		return gmpi::MP_OK;
	}

	int32_t MP_STDCALL onPointerMove(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{
		if (!getCapture())
		{
			return gmpi::MP_UNHANDLED;
		}

		Point offset(point.x - pointPrevious.x, point.y - pointPrevious.y); // TODO overload subtraction.

		float coarseness = 0.005f;

		//		if (modifier_keys::isHeldCtrl()) // <cntr> key magnifies
		if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL) // <cntr> key magnifies
			coarseness = 0.001f;

		float new_pos = pinAnimationPosition;
		new_pos = new_pos - coarseness * (float)offset.y;

		if (new_pos < 0.f)
			new_pos = 0.f;

		if (new_pos > 1.f)
			new_pos = 1.f;

		pinAnimationPosition = new_pos;

		pointPrevious = point;

		invalidateRect();

		return gmpi::MP_OK;
	}

	void calcDimensions(Point& center, float& radius, float& thickness)
	{
		auto r = getRect();

		center = Point((r.left + r.right) * 0.5f, (r.top + r.bottom) * 0.5f);
		radius = (std::min)(r.getWidth(), r.getHeight()) * 0.4f;
		thickness = radius * 0.1f;
	}

	int32_t MP_STDCALL hitTest(MP1_POINT point) override
	{
		Point center;
		float radius;
		float thickness;
		calcDimensions(center, radius, thickness);

		Point v(point.x - center.x, point.y - center.y);
		float distSquared = v.x * v.x + v.y * v.y;
		float outerSquared = (radius + thickness * 0.1f);
		outerSquared *= outerSquared;

		return distSquared <= outerSquared ? MP_OK : MP_FAIL;
	}

	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override
	{
		Graphics g(drawingContext);

		Point center;
		float radius;
		float thickness;
		calcDimensions(center, radius, thickness);

		auto brushForeground = g.CreateSolidColorBrush(Color::White);
		auto brushBackground = g.CreateSolidColorBrush(Color::FromHexString(pinHexIn));

		const float startAngle = 30.0f; // angle between "straight-down" and start of arc. In degrees.
		const float startAngleRadians = (startAngle * (3.14159265358979323846) / 180.f); // angle between "straight-down" and start of arc. In degrees.
		const float quarterTurnClockwise = (3.14159265358979323846) * 0.5f;

		StrokeStyleProperties strokeStyleProperties;
		strokeStyleProperties.setCapStyle(CapStyle::Round);
		strokeStyleProperties.setLineJoin(LineJoin::Round);
		auto strokeStyle = g.GetFactory().CreateStrokeStyle(strokeStyleProperties);

		Point startPoint(center.x + radius * cosf(quarterTurnClockwise + startAngleRadians), center.y + radius * sinf(quarterTurnClockwise + startAngleRadians));
		Point midPoint(center.x, center.y - radius);
		float sweepAngle = ((3.14159265358979323846) * 2.0f - startAngleRadians * 2.0f);
		Point endPoint(center.x + radius * cosf(quarterTurnClockwise + startAngleRadians + sweepAngle), center.y + radius * sinf(quarterTurnClockwise + startAngleRadians + sweepAngle));

		float nomalised = pinAnimationPosition;
		sweepAngle = nomalised * (static_cast<float>(3.14159265358979323846) * 2.0f - startAngleRadians * 2.0f);
		Point movingPoint(center.x + radius * cosf(quarterTurnClockwise + startAngleRadians + sweepAngle), center.y + radius * sinf(quarterTurnClockwise + startAngleRadians + sweepAngle));

		Size circleSize(radius, radius);

		// Background circle.
		{
			g.FillCircle(center, radius + thickness * 0.5f, brushBackground);
		}

		// Line.
		{
			g.DrawLine(center, movingPoint, brushForeground, thickness, strokeStyle);
		}

		return gmpi::MP_OK;
	}
};

namespace
{
	auto r = Register<PatchMemKnobGui>::withId(L"PatchMemKnob");
}

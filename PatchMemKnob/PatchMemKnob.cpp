#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include <iomanip>
#include <sstream>
#define _USE_MATH_DEFINES


using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;

GmpiDrawing_API::MP1_POINT pointPrevious;

class WirelessPatchMemGui final : public gmpi_gui::MpGuiGfxBase
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
		if (pinColorAdj)
		{
			AnimPosToHex();
			pinHex = pinHexIn;
		}
		invalidateRect();
		if (!pinColorAdj)
			pinHexIn = pinHex;
	}

	void onSetHexIn()
	{
		//pinHex = pinHexIn;
	}
	void onSetHex()
	{
		//pinHexIn = pinHex;
	}


public:
 	FloatGuiPin pinScaleIn;
 	FloatGuiPin pinAnimationIn;
 	FloatGuiPin pinScaled;
 	FloatGuiPin pinAnimationPosition;
	StringGuiPin pinHexIn;
	BoolGuiPin pinColorAdj;
	StringGuiPin pinHex;


	WirelessPatchMemGui()
	{
		initializePin( pinScaleIn, static_cast<MpGuiBaseMemberPtr2>(&WirelessPatchMemGui::onSetScaleIn) );
		initializePin( pinAnimationIn, static_cast<MpGuiBaseMemberPtr2>(&WirelessPatchMemGui::onSetAnimationIn) );
		initializePin( pinScaled, static_cast<MpGuiBaseMemberPtr2>(&WirelessPatchMemGui::onSetScaled) );
		initializePin( pinAnimationPosition, static_cast<MpGuiBaseMemberPtr2>(&WirelessPatchMemGui::onSetAnimationPosition) );
		initializePin(pinHexIn, static_cast<MpGuiBaseMemberPtr2>(&WirelessPatchMemGui::onSetHexIn));
		initializePin(pinColorAdj);
		initializePin(pinHex, static_cast<MpGuiBaseMemberPtr2>(&WirelessPatchMemGui::onSetHex));
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

	int32_t AnimPosToHex()
	{
		int x = pinAnimationPosition * 1535;
		int R = 0;
		int G = 0;
		int B = 0;
		//--------------------------------

		if ((x >= 255) && (x < 768))
			G = 255;
		if (x > 1023)
			G = 0;

		if ((x >= 1279) || (x < 255))
			R = 255;
		if ((x > 510) && (x <= 1024))
			R = 0;

		if ((x >= 767) && (x < 1280))
			B = 255;
		if (x <= 511)
			B = 0;

		//-----------------------------

		if ((x >= 0) && (x <= 255))
		{
			G = x;
		}
		//-------------------

		if ((x >= 256) && (x <= 511))
		{
			R = 255 - (x - 256);
		}

		//-------------------
		if ((x >= 512) && (x <= 767))
		{
			B = x - 512;
		}

		//-------------------
		if ((x >= 768) && (x <= 1023))
		{
			G = 255 - (x - 768);
		}

		//-------------------

		if ((x >= 1024) && (x <= 1279))
		{
			R = x - 1024;
		}

		//-------------------
		if ((x >= 1280) && (x <= 1535))
		{
			B = 255 - (x - 1280);
		}

		std::stringstream ssR;
		ssR << std::setfill('0') << std::setw(sizeof(int) - 2) << std::hex << R;
		std::string resR(ssR.str());

		std::stringstream ssG;
		ssG << std::setfill('0') << std::setw(sizeof(int) - 2) << std::hex << G;
		std::string resG(ssG.str());

		std::stringstream ssB;
		ssB << std::setfill('0') << std::setw(sizeof(int) - 2) << std::hex << B;
		std::string resB(ssB.str());

		pinHexIn = resR + resG + resB;

	}

	void calcDimensions(Point& center, float& radius, float& thickness)
	{
		auto r = getRect();

		center = Point((r.left + r.right) * 0.5f, (r.top + r.bottom) * 0.5f);
		radius = (std::min)(r.getWidth(), r.getHeight()) * 0.4f;
		thickness = radius * 0.2f;
	}

	int32_t MP_STDCALL hitTest(MP1_POINT point) override
	{
		Point center;
		float radius;
		float thickness;
		calcDimensions(center, radius, thickness);

		Point v(point.x - center.x, point.y - center.y);
		float distSquared = v.x * v.x + v.y * v.y;
		float outerSquared = (radius + thickness * 0.5f);
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

		const float startAngle = 35.0f; // angle between "straight-down" and start of arc. In degrees.
		const float startAngleRadians = startAngle * (M_PI) / 180.f; // angle between "straight-down" and start of arc. In degrees.
		const float quarterTurnClockwise = (M_PI) * 0.5f;

		StrokeStyleProperties strokeStyleProperties;
		strokeStyleProperties.setCapStyle(CapStyle::Round);
		strokeStyleProperties.setLineJoin(LineJoin::Round);
		auto strokeStyle = g.GetFactory().CreateStrokeStyle(strokeStyleProperties);

		Point startPoint(center.x + radius * cosf(quarterTurnClockwise + startAngleRadians), center.y + radius * sinf(quarterTurnClockwise + startAngleRadians));
		Point midPoint(center.x, center.y - radius);
		float sweepAngle = ((M_PI) * 2.0f - startAngleRadians * 2.0f);
		Point endPoint(center.x + radius * cosf(quarterTurnClockwise + startAngleRadians + sweepAngle), center.y + radius * sinf(quarterTurnClockwise + startAngleRadians + sweepAngle));

		float nomalised = pinAnimationPosition;
		sweepAngle = nomalised * (static_cast<float>(M_PI) * 2.0f - startAngleRadians * 2.0f);
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
	auto r = Register<WirelessPatchMemGui>::withId(L"WirelessPatchMem");
}

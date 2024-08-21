#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include <iomanip>
#include <sstream>

using namespace std;
using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;

GmpiDrawing_API::MP1_POINT pointPrevious;

class ColorKnob final : public gmpi_gui::MpGuiGfxBase
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
		//----------------------------------
			if (pinColorAdj)
			{
				AnimPosToHex();

				pinHex = pinHexIn;
			}
			if (!pinColorAdj)
			{
				pinHexIn = pinHex;
			}
			invalidateRect();
	}
	void onSetHint()
	{
	}
	void onSetHexIn()
	{
	}
	void onSetHex()
	{		
	}
	void onSetCircleSize()
	{
		if(pinCircleSize > 1)
		{
			pinCircleSize = 1;
		}
	}
	void onSetColorAdj()
	{
		
	}
	void onSetLineThickness()
	{
	}
	void onSetLineColor()
	{
	}
	void onSetMouseDown()
	{		
	}
	void onSetTest()
	{
	}
	void onSetMouseOver()
	{
	}
	void onSetResetValue()
	{
	}

	FloatGuiPin pinScaleIn;
	FloatGuiPin pinAnimationIn;
	FloatGuiPin pinScaled;
	FloatGuiPin pinAnimationPosition;
	StringGuiPin pinHint;
	StringGuiPin pinHexIn;
	BoolGuiPin pinColorAdj;
	FloatGuiPin pinCircleSize;
	StringGuiPin pinHex;
	FloatGuiPin pinLineThickness;
	StringGuiPin pinLineColor;
	BoolGuiPin pinMouseDown;
	BoolGuiPin pinTest;
	BoolGuiPin pinMouseOver;
	FloatGuiPin pinResetValue;

public:
	ColorKnob()
	{
		initializePin(pinScaleIn, static_cast<MpGuiBaseMemberPtr2>(&ColorKnob::onSetScaleIn));
		initializePin(pinAnimationIn, static_cast<MpGuiBaseMemberPtr2>(&ColorKnob::onSetAnimationIn));
		initializePin(pinScaled, static_cast<MpGuiBaseMemberPtr2>(&ColorKnob::onSetScaled));
		initializePin( pinAnimationPosition, static_cast<MpGuiBaseMemberPtr2>(&ColorKnob::onSetAnimationPosition) );
		initializePin(pinHint, static_cast<MpGuiBaseMemberPtr2>(&ColorKnob::onSetHint));
		initializePin(pinHexIn, static_cast<MpGuiBaseMemberPtr2>(&ColorKnob::onSetAnimationPosition));
		initializePin(pinColorAdj, static_cast<MpGuiBaseMemberPtr2>(&ColorKnob::onSetColorAdj));
		initializePin(pinCircleSize, static_cast<MpGuiBaseMemberPtr2>(&ColorKnob::onSetCircleSize));		
		initializePin(pinHex, static_cast<MpGuiBaseMemberPtr2>(&ColorKnob::onSetAnimationPosition));
		initializePin(pinLineThickness, static_cast<MpGuiBaseMemberPtr2>(&ColorKnob::onSetLineThickness));
		initializePin(pinLineColor, static_cast<MpGuiBaseMemberPtr2>(&ColorKnob::onSetLineColor));
		initializePin(pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&ColorKnob::onSetMouseDown));
		initializePin(pinTest, static_cast<MpGuiBaseMemberPtr2>(&ColorKnob::onSetTest));
		initializePin(pinMouseOver, static_cast<MpGuiBaseMemberPtr2>(&ColorKnob::onSetMouseOver));
		initializePin(pinResetValue, static_cast<MpGuiBaseMemberPtr2>(&ColorKnob::onSetResetValue));
	}

	int32_t MP_STDCALL getToolTip(GmpiDrawing_API::MP1_POINT point, gmpi::IString* returnString) override
	{
		auto utf8String = (std::string)pinHint;
		returnString->setData(utf8String.data(), (int32_t)utf8String.size());
		return gmpi::MP_OK;
	}

	int32_t MP_STDCALL onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{
		// Let host handle right-clicks.
		if ((flags & GG_POINTER_FLAG_FIRSTBUTTON) == 0)
		{
			return gmpi::MP_OK; // Indicate successful hit, so right-click menu can show.
		}
		if ((flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL) && (0x10))
		{
			pinAnimationPosition = pinResetValue;
		}
		
		pointPrevious = point;	// note first point.
		pinMouseDown = true;
		setCapture();

		return gmpi::MP_OK;
	}

	int32_t onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point)
	{
		if (!getCapture())
		{
			return gmpi::MP_UNHANDLED;
		}

		if ((flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT) == 0)
		{
			pinColorAdj = false;
			pinTest = false;
		}

		pinColorAdj = false;		
		pinMouseDown = false;
		releaseCapture();
		
		return gmpi::MP_OK;
	}

	int32_t MP_STDCALL onMouseWheel(int32_t flags, int32_t delta, MP1_POINT point) override
	{			
		float new_pos = pinAnimationPosition;
		new_pos = new_pos + delta / 12000.0f;
		if (new_pos < 0.f)
			new_pos = 0.f;
		if (new_pos > 1.f)
			new_pos = 1.f;

		if ((flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT) == 0)
		{
			pinColorAdj = false;
			pinTest = false;
		}
		
		if (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT)
		{
			pinColorAdj = true;
			pinTest = true;
		}
		
		pinAnimationPosition = new_pos;
	
		return gmpi::MP_OK;
	}

	int32_t MP_STDCALL onPointerMove(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{
		if (!getCapture())
		{
			return gmpi::MP_UNHANDLED;
		}
		// ignore horizontal scrolling
		if (0 != (flags & gmpi_gui_api::GG_POINTER_KEY_ALT))
			return gmpi::MP_UNHANDLED;
		if ((flags & gmpi_gui_api::GG_POINTER_KEY_ALT) == 0)
		{
			pinColorAdj = false;
		}

		if (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT)
		{
			pinColorAdj = true;
			pinTest = true;
		}
		if ((flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT) == 0)
		{
			pinColorAdj = false;
			pinTest = false;
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

		
		std::string resA = "ff";

		std::stringstream ssR;
		ssR << std::setfill('0') << std::setw(sizeof(int) - 2) << std::hex << R;
		std::string resR(ssR.str());

		std::stringstream ssG;
		ssG << std::setfill('0') << std::setw(sizeof(int) - 2) << std::hex << G;
		std::string resG(ssG.str());

		std::stringstream ssB;
		ssB << std::setfill('0') << std::setw(sizeof(int) - 2) << std::hex << B;
		std::string resB(ssB.str());

		pinHexIn = resA + resR + resG + resB;

		//return gmpi::MP_OK;
	}

	void calcDimensions(Point& center, float& radius, float& thickness)
	{
		float mult;
		auto r = getRect();
		mult = (pinCircleSize) * 0.4f;
		center = Point((r.left + r.right) * 0.5f, (r.top + r.bottom) * 0.5f);
		radius = (std::min)(r.getWidth(), r.getHeight()) * mult;
		thickness = radius * 0.2f;
	}

	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override
	{
		Graphics g(drawingContext);

		Point center;
		float radius;
		float thickness;
		calcDimensions(center, radius, thickness);

		auto brushForeground = g.CreateSolidColorBrush(Color::FromHexString(pinLineColor));
		auto brushBackground = g.CreateSolidColorBrush(Color::FromHexString(pinHexIn));

		const float startAngle = 25.0f; // angle between "straight-down" and start of arc. In degrees.
		const float startAngleRadians = (startAngle * (3.14159265358979323846) / 180.f); // angle between "straight-down" and start of arc. In degrees.
		const float quarterTurnClockwise = (3.14159265358979323846) * 0.5f;

		StrokeStyleProperties strokeStyleProperties;		
		strokeStyleProperties.setCapStyle(CapStyle::Round);
		strokeStyleProperties.setLineJoin(LineJoin::Round);
		auto strokeStyle = g.GetFactory().CreateStrokeStyle(strokeStyleProperties);

		Point startPoint(center.x + radius * cosf(quarterTurnClockwise + startAngleRadians), center.y + radius * sinf(quarterTurnClockwise + startAngleRadians));
		Point midPoint(center.x+radius, center.y - radius);
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
		float line_thickness = pinLineThickness;
		// Line.
		{
			g.DrawLine(center, movingPoint, brushForeground, line_thickness, strokeStyle);
		}

		return gmpi::MP_OK;
	}
};

namespace
{
	auto r = Register<ColorKnob>::withId(L"ColorKnob1");
}

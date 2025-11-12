#include "mp_sdk_gui2.h"
#include "Drawing.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "../sharedLegacyWidgets/SubControlBase.h"
#include "../shared/xplatform_modifier_keys.h"
#include <iomanip>
#include <sstream>
#include <algorithm> 

using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;

GmpiDrawing_API::MP1_POINT pointPrevious;

// Constants for better readability
const float WHEEL_SCROLL_FACTOR = 1.0f / 12000.0f;
const float CIRCLE_SIZE_MULTIPLIER = 0.4f;
const float THICKNESS_MULTIPLIER = 0.2f;
const int HEX_WIDTH = 2; // Width for hex color string

class ExKnobGui final : public gmpi_gui::MpGuiGfxBase
{
	float clamp(float value, float minVal, float maxVal) //help clamp function
	{
		return (value < minVal) ? minVal : (value > maxVal) ? maxVal : value;
	}

	float circleSize;

	void onSetAnimationPosition()
	{

		if (pinColorAdj)
		{
			AnimPosToHex();
			pinHex = pinHexIn;
			pinBackground = pinHexIn;
			pinHexOut = pinHexIn;
		}
		else
		{
			pinHexIn = pinHex;
			pinBackground = pinHex;
			pinHexOut = pinHex;
		}

		// Invalidate for redraw
		invalidateRect();

		// Get total steps
		int steps = pinSteps.getValue();

		// Calculate current step based on position
		int current_step = static_cast<int>(round(pinAnimationPosition * (steps - 1)));

		// Update pinCurrentStep if different
		if (pinCurrentStep != current_step)
		{
			pinCurrentStep = current_step;
		}
		
	}

 	void onSetHint()
	{
		// pinHint changed
	}

 	void onSetMenuItems()
	{
		// pinMenuItems changed
	}

 	void onSetMenuSelection()
	{
		// pinMenuSelection changed
	}

 	void onSetMouseDown()
	{
		// pinMouseDown changed
	}

 	void onSetBackground()
	{
		invalidateRect();
	}

 	void onSetForeground()
	{
		invalidateRect();
	}

 	void onSetRead_only()
	{
		// pinRead_only changed
	}
	
	void onSetSteps()
	{
		//steps = pinSteps;
	}

	void onSetCurrentStep()
	{
		//
	}

	void onSetCircleSize()
	{		
		circleSize = clamp(pinCircleSize, 0.0f, 1.0f);
		invalidateRect();
	}

 	FloatGuiPin pinAnimationPosition;
 	StringGuiPin pinHint;
 	StringGuiPin pinMenuItems;
 	IntGuiPin pinMenuSelection;
 	BoolGuiPin pinMouseDown;
 	StringGuiPin pinBackground;
 	StringGuiPin pinForeground;
 	BoolGuiPin pinReadOnly;
	IntGuiPin pinSteps;
	IntGuiPin pinCurrentStep;
	IntGuiPin pinStyle;
	StringGuiPin pinHexIn;
	FloatGuiPin pinCircleSize;
	FloatGuiPin pinLineThickness;
	BoolGuiPin pinColorAdj;
	StringGuiPin pinHex;
	StringGuiPin pinHexOut;
	FloatGuiPin pinStartAngle;
	BoolGuiPin pinCtrlClick;

public:
	ExKnobGui()
	{
		initializePin( pinAnimationPosition, static_cast<MpGuiBaseMemberPtr2>(&ExKnobGui::onSetAnimationPosition) );
		initializePin( pinHint, static_cast<MpGuiBaseMemberPtr2>(&ExKnobGui::onSetHint) );
		initializePin( pinMenuItems, static_cast<MpGuiBaseMemberPtr2>(&ExKnobGui::onSetMenuItems) );
		initializePin( pinMenuSelection, static_cast<MpGuiBaseMemberPtr2>(&ExKnobGui::onSetMenuSelection) );
		initializePin( pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&ExKnobGui::onSetMouseDown) );
		initializePin( pinBackground, static_cast<MpGuiBaseMemberPtr2>(&ExKnobGui::onSetBackground) );
		initializePin( pinForeground, static_cast<MpGuiBaseMemberPtr2>(&ExKnobGui::onSetForeground) );
		initializePin(pinReadOnly, static_cast<MpGuiBaseMemberPtr2>(&ExKnobGui::onSetRead_only) );
		initializePin(pinSteps, static_cast<MpGuiBaseMemberPtr2>(&ExKnobGui::onSetSteps));
		initializePin(pinCurrentStep, static_cast<MpGuiBaseMemberPtr2>(&ExKnobGui::onSetCurrentStep));
		initializePin(pinStyle);
		initializePin(pinHexIn);
		initializePin(pinCircleSize, static_cast<MpGuiBaseMemberPtr2>(&ExKnobGui::onSetCircleSize));
		initializePin(pinLineThickness);
		initializePin(pinColorAdj);
		initializePin(pinHex);
		initializePin(pinHexOut);
		initializePin(pinStartAngle);
		initializePin(pinCtrlClick);
	}

	int32_t MP_STDCALL onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{
		// Let host handle right-clicks.
		if (pinReadOnly == true || (flags & GG_POINTER_FLAG_FIRSTBUTTON) == 0)
		{
			return gmpi::MP_OK; // Indicate successful hit, so right-click menu can show.
		}

		if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL)
		{
			pinCtrlClick = true;
		}

		pointPrevious = point;	// note first point.

		//	smallDragSuppression = true;
		pinMouseDown = true;

		setCapture();

		return gmpi::MP_OK;
	}

	virtual int32_t MP_STDCALL onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{
		if (!getCapture())
		{
			return gmpi::MP_UNHANDLED;
		}

		releaseCapture();
		pinMouseDown = false;
		pinColorAdj = false;
		pinCtrlClick = false;
		return gmpi::MP_OK;
	}

	virtual int32_t MP_STDCALL populateContextMenu(float x, float y, gmpi::IMpUnknown* contextMenuItemsSink) override
	{
		gmpi::IMpContextItemSink* sink = nullptr;
		contextMenuItemsSink->queryInterface(gmpi::MP_IID_CONTEXT_ITEMS_SINK, reinterpret_cast<void**>(&sink));

		it_enum_list itr(pinMenuItems);

		for (itr.First(); !itr.IsDone(); ++itr)
		{
			int32_t flags = 0;

			// Special commands (sub-menus)
			switch (itr.CurrentItem()->getType())
			{
			case enum_entry_type::Separator:
			case enum_entry_type::SubMenu:
				flags |= gmpi_gui::MP_PLATFORM_MENU_SEPARATOR;
				break;

			case enum_entry_type::SubMenuEnd:
			case enum_entry_type::Break:
				continue;

			default:
				break;
			}

			sink->AddItem(JmUnicodeConversions::WStringToUtf8(itr.CurrentItem()->text).c_str(), itr.CurrentItem()->value, flags);
		}
		sink->release();
		return gmpi::MP_OK;
	}

	virtual int32_t MP_STDCALL onContextMenu(int32_t selection) override
	{
		pinMenuSelection = selection; // send menu momentarily, then reset.
		pinMenuSelection = -1;

		return gmpi::MP_OK;
	}

	// Old way.
	virtual int32_t MP_STDCALL getToolTip(float x, float y, gmpi::IMpUnknown* returnToolTipString) override
	{
		gmpi::IString* returnString = nullptr;

		if (pinHint.getValue().empty() || gmpi::MP_OK != returnToolTipString->queryInterface(gmpi::MP_IID_RETURNSTRING, reinterpret_cast<void**>(&returnString)))
		{
			return gmpi::MP_NOSUPPORT;
		}

		auto utf8ToolTip = (std::string)pinHint;
		returnString->setData(utf8ToolTip.data(), (int32_t)utf8ToolTip.size());
		return gmpi::MP_OK;
	}

	// New way.
	int32_t MP_STDCALL getToolTip(GmpiDrawing_API::MP1_POINT point, gmpi::IString* returnString) override
	{
		auto utf8ToolTip = (std::string)pinHint;
		returnString->setData(utf8ToolTip.data(), static_cast<int32_t>(utf8ToolTip.size()));
		return gmpi::MP_OK;
	}

	float snapped_pos = 0.0f;

	int32_t MP_STDCALL onMouseWheel(int32_t flags, int32_t delta, MP1_POINT point) override
	{

		if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL) // <cntr> key magnifies
		{
			float new_pos = pinAnimationPosition + delta * WHEEL_SCROLL_FACTOR; // Updated scrolling logic
			new_pos = std::max(0.f, std::min(new_pos, 1.f)); // Clamp value between 0 and 1
			pinAnimationPosition = new_pos;
		}

		else
		{	

		int32_t steps = pinSteps.getValue(); // total number of steps

		float new_pos = pinAnimationPosition;

		// Use delta to determine movement; 120 per notch
		float step_size = 1.0f / (steps - 1); // size of each step in normalized units
		float movement = (delta / 120.0f) * step_size; // proportional movement

		new_pos += movement;

		// Clamp between 0 and 1
		if (new_pos < 0.f)
			new_pos = 0.f;
		if (new_pos > 1.f)
			new_pos = 1.f;

		// Map to nearest step
		int current_step = static_cast<int>(round(new_pos * (steps - 1)));
		snapped_pos = static_cast<float>(current_step) / (steps - 1);

		pinAnimationPosition = snapped_pos;
		pinCurrentStep = current_step;

		}

		return gmpi::MP_OK;
	}

	int32_t MP_STDCALL onPointerMove(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{
		if (!getCapture())
		{
			return gmpi::MP_UNHANDLED;
		}

		if (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT)
		{
			pinColorAdj = true;
		}
		if ((flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT) == 0)
		{
			pinColorAdj = false;
		}

		Point offset(point.x - pointPrevious.x, point.y - pointPrevious.y); // TODO overload subtraction.

		float coarseness = 0.005f;

		if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL) // <cntr> key magnifies

			pinCtrlClick = false;
			
			coarseness = 0.001f;

		float new_pos = pinAnimationPosition;
		new_pos = std::clamp(new_pos - coarseness * (float)offset.y, 0.0f, 1.0f);

		pointPrevious = point;

		invalidateRect();

		pinAnimationPosition = new_pos;

		return gmpi::MP_OK;
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

	int32_t AnimPosToHex() {
		int x = pinAnimationPosition * 1535; // Scale animation position to range
		int R = 0;
		int G = 0;
		int B = 0;

		// Determine RGB values based on x
		if ((x >= 255) && (x < 768)) G = 255;
		if (x > 1023) G = 0;
		if ((x >= 1279) || (x < 255)) R = 255;
		if ((x > 510) && (x <= 1024)) R = 0;
		if ((x >= 767) && (x < 1280)) B = 255;
		if (x <= 511) B = 0;

		// Additional color transitions
		if ((x >= 0) && (x <= 255)) G = x;
		if ((x >= 256) && (x <= 511)) R = 255 - (x - 256);
		if ((x >= 512) && (x <= 767)) B = x - 512;
		if ((x >= 768) && (x <= 1023)) G = 255 - (x - 768);
		if ((x >= 1024) && (x <= 1279)) R = x - 1024;
		if ((x >= 1280) && (x <= 1535)) B = 255 - (x - 1280);

		// Format to hex string with 'ff' (for full opacity) and clamped RGB values
		std::stringstream ssR, ssG, ssB;
		ssR << std::setfill('0') << std::setw(HEX_WIDTH) << std::hex << R;
		ssG << std::setfill('0') << std::setw(HEX_WIDTH) << std::hex << G;
		ssB << std::setfill('0') << std::setw(HEX_WIDTH) << std::hex << B;

		pinHexIn = "ff" + ssR.str() + ssG.str() + ssB.str(); // Combining into a final hex color string

		return gmpi::MP_OK;
	}

	void calcDimensionsBg(Point& centerBg, float& radiusBg, float& thicknessBg) {
		auto r = getRect();
		centerBg = Point((r.left + r.right) * 0.5f, (r.top + r.bottom) * 0.5f);
		radiusBg = std::max(r.getWidth(), r.getHeight()) * 0.4f; // Background radius
		thicknessBg = radiusBg * THICKNESS_MULTIPLIER; // Background thickness
	}

	void calcDimensions(Point& center, float& radius, float& thickness) 
	{
		auto r = getRect();
		float mult = circleSize * CIRCLE_SIZE_MULTIPLIER;
		center = Point((r.left + r.right) * 0.5f, (r.top + r.bottom) * 0.5f);
		radius = std::max(r.getWidth(), r.getHeight()) * mult; // Circle radius
		thickness = radius * THICKNESS_MULTIPLIER;
	}

	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override {
		Graphics g(drawingContext);
		Point center;
		float radiusBg, thicknessBg;
		calcDimensionsBg(center, radiusBg, thicknessBg);

		float radius, thickness;
		calcDimensions(center, radius, thickness);

		auto brushLine = g.CreateSolidColorBrush(Color::FromHexString(pinForeground));
		auto gradientBrushBg = g.CreateRadialGradientBrush(Color::FromHexString(pinHex), Color::Black, center, radiusBg);
		auto gradientBrush = g.CreateRadialGradientBrush(Color::FromHexString(pinHex), Color::Black, center, radius);

		const float startAngle = pinStartAngle; // Angle between "straight-down" and start of arc
		const float startAngleRadians = startAngle * (3.14159265358979323846f / 180.f); // Convert to radians
		const float quarterTurnClockwise = 3.14159265358979323846f * 0.5f;

		// Create stroke style with cap style
		StrokeStyleProperties strokeStyleProperties;
		CapStyle arr[] = { CapStyle::Flat, CapStyle::Square, CapStyle::Round, CapStyle::Triangle };
		strokeStyleProperties.setCapStyle(arr[pinStyle.getValue()]);

		auto strokeStyle = g.GetFactory().CreateStrokeStyle(strokeStyleProperties);

		Point startPoint(center.x + radius * cosf(quarterTurnClockwise + startAngleRadians), center.y + radius * sinf(quarterTurnClockwise + startAngleRadians));
		Point midPoint(center.x + radius, center.y - radius);

		float nominalized = pinAnimationPosition;
		float sweepAngle = nominalized * (static_cast<float>(3.14159265358979323846) * 2.0f - startAngleRadians * 2.0f);
		Point movingPoint(center.x + radius * cosf(quarterTurnClockwise + startAngleRadians + sweepAngle), center.y + radius * sinf(quarterTurnClockwise + startAngleRadians + sweepAngle));

		// Draw background circle
		g.FillCircle(center, radiusBg + thicknessBg * .5f, gradientBrushBg);
		// Draw the top circle
		g.FillCircle(center, radius + thickness * .5f, gradientBrush);
		// Draw the line
		g.DrawLine(center, movingPoint, brushLine, pinLineThickness, strokeStyle);

		return gmpi::MP_OK;
	}
};

namespace
{
	auto r = Register<ExKnobGui>::withId(L"exKnob");
}

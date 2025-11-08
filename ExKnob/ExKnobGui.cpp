#include "mp_sdk_gui2.h"
#include "Drawing.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "../sharedLegacyWidgets/SubControlBase.h"
#include "../shared/xplatform_modifier_keys.h"

using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;

GmpiDrawing_API::MP1_POINT pointPrevious;

class ExKnobGui final : public gmpi_gui::MpGuiGfxBase
{
	void onSetAnimationPosition()
	{
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
		// pinBackground changed
	}

 	void onSetForeground()
	{
		// pinForground changed
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
	}

	int32_t MP_STDCALL onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{
		// Let host handle right-clicks.
		if (pinReadOnly == true || (flags & GG_POINTER_FLAG_FIRSTBUTTON) == 0)
		{
			return gmpi::MP_OK; // Indicate successful hit, so right-click menu can show.
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

		if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL) // <cntr> key magnifies
			coarseness = 0.001f;

		float new_pos = pinAnimationPosition;
		new_pos = std::clamp(new_pos - coarseness * (float)offset.y, 0.0f, 1.0f);

		pointPrevious = point;

		invalidateRect();

		pinAnimationPosition = new_pos;

		return gmpi::MP_OK;
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

		auto brushForeground = g.CreateSolidColorBrush(Color::FromHexString(pinForeground));
		auto brushBackground = g.CreateSolidColorBrush(Color::FromHexString(pinBackground));

		const float startAngle = 35.0f; // angle between "straight-down" and start of arc. In degrees.
		const float startAngleRadians = startAngle * M_PI / 180.f; // angle between "straight-down" and start of arc. In degrees.
		const float quarterTurnClockwise = M_PI * 0.5f;

		StrokeStyleProperties strokeStyleProperties;
		strokeStyleProperties.setCapStyle(CapStyle::Round);
		strokeStyleProperties.setLineJoin(LineJoin::Round);
		auto strokeStyle = g.GetFactory().CreateStrokeStyle(strokeStyleProperties);

		Point startPoint(center.x + radius * cosf(quarterTurnClockwise + startAngleRadians), center.y + radius * sinf(quarterTurnClockwise + startAngleRadians));
		Point midPoint(center.x, center.y - radius);
		float sweepAngle = (M_PI * 2.0f - startAngleRadians * 2.0f);
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
	auto r = Register<ExKnobGui>::withId(L"exKnob");
}

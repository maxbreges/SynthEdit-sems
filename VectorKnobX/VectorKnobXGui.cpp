#include "../sharedLegacyWidgets/SubControlBase.h"
#include "../se_sdk3/Drawing.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "../shared/xplatform_modifier_keys.h"
#include <chrono>
#include <thread>

SE_DECLARE_INIT_STATIC_FILE(VectorKnobXGui);

using namespace std;
using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;

class VectorBase : public SubControlBase
{
protected:
	FloatGuiPin pinAnimationPosition;
	StringGuiPin pinForeground;
	StringGuiPin pinBackground;
	IntGuiPin pinColorByDatatype;
	FloatGuiPin pinCircleSize;
	FloatGuiPin pinLineThickness;

	GmpiDrawing_API::MP1_POINT pointPrevious;
	const float WHEEL_SCROLL_FACTOR = 1.0f / 12000.0f;
	const float CIRCLE_SIZE_MULTIPLIER = 0.4f;
	const float THICKNESS_MULTIPLIER = 0.2f;
	const int HEX_WIDTH = 2; // Width for hex color string

	void onSetAnimationPosition()
	{
		invalidateRect();
	}

	void onSetBackground()
	{
		invalidateRect();
	}

/*	void onSetColorByDatatype()
	{
		int colorIndex = pinColorByDatatype.getValue();

		switch (colorIndex)
		{
		case 0: pinBackground = "FFFF55FF"; break;
		case 1: pinBackground = "FF555555"; break;
		case 2: pinBackground = "FF00CCEE"; break;
		case 3: pinBackground = "FFFF8800"; break;
		case 4: pinBackground = "FFFF0000"; break;
		}
		invalidateRect();
	}
*/
private:
	int count = 0;
	bool processing = false;
public:

	void onSetMouseDown()
	{
		if (pinMouseDown)
		{
			++count;



			int countDown = 250;
			// Start countdown in a separate thread
			std::thread([this, countDown]()
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(countDown));

					// Reset count when countdown is finished
					{
						count = 0;
					}

				}
			).detach();
		}

		{
			if (count == 2)
			{
				pinAnimationPosition = 0.5f;

				// Check if we are already processing
				if (processing)
				{
					return; // Ignore further clicks while processing
				}

				processing = true; // Set flag to indicate processing has started
			}
			// Reset the processing flag to allow future clicks
			processing = false;
		}
	}

	VectorBase()
	{
		// Initialize pointPrevious to (0,0)
		pointPrevious = { 0.0f, 0.0f };

		initializePin(pinAnimationPosition, static_cast<MpGuiBaseMemberPtr2>(&VectorBase::onSetAnimationPosition));
		initializePin(pinHint);
		initializePin(pinMenuItems);
		initializePin(pinMenuSelection);
		initializePin(pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&VectorBase::onSetMouseDown));
		initializePin(pinBackground, static_cast<MpGuiBaseMemberPtr2>(&VectorBase::onSetBackground));
		initializePin(pinForeground, static_cast<MpGuiBaseMemberPtr2>(&VectorBase::onSetAnimationPosition));
		//initializePin(pinColorByDatatype, static_cast<MpGuiBaseMemberPtr2>(&VectorBase::onSetColorByDatatype));
	}

	int32_t MP_STDCALL onMouseWheel(int32_t flags, int32_t delta, MP1_POINT point) override
	{
		float new_pos = pinAnimationPosition + delta * WHEEL_SCROLL_FACTOR;
		new_pos = max(0.f, min(new_pos, 1.f)); // Clamp value between 0 and 1
		pinAnimationPosition = new_pos;
		return gmpi::MP_OK;
	}

	int32_t MP_STDCALL onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{
		// Let host handle right-clicks.
		if ((flags & GG_POINTER_FLAG_FIRSTBUTTON) == 0)
		{
			return gmpi::MP_OK; // Indicate successful hit, so right-click menu can show.
		}

		pointPrevious = point;	// note first point.

		//	smallDragSuppression = true;
		pinMouseDown = true;

		setCapture();

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
};

class VectorKnobXGui : public VectorBase
{
	float clamp(float value, float minVal, float maxVal) //help clamp function
	{
		return (value < minVal) ? minVal : (value > maxVal) ? maxVal : value;
	}

	float circleSize = 0.93f;
	float lineThickness = 3.0f;

	void onSetCircleSize()
	{
		circleSize = clamp(pinCircleSize, 0.0f, 1.0f);
		invalidateRect();
	}

	void onSetLineThickness()
	{
		lineThickness = clamp(pinLineThickness, 0.0f, 10.0f);
		invalidateRect();
	}

public:

	VectorKnobXGui()
	{
		initializePin(pinCircleSize, static_cast<MpGuiBaseMemberPtr2>(&VectorKnobXGui::onSetCircleSize));
		initializePin(pinLineThickness, static_cast<MpGuiBaseMemberPtr2>(&VectorKnobXGui::onSetLineThickness));
	}
 
	void calcDimensionsBg(Point& centerBg, float& radiusBg, float& thicknessBg) {
		auto r = getRect();
		centerBg = Point((r.left + r.right) * 0.5f, (r.top + r.bottom) * 0.5f);
		radiusBg = std::min<float>(r.getWidth(), r.getHeight()) * 0.4f; // Background radius
		thicknessBg = radiusBg * THICKNESS_MULTIPLIER; // Background thickness
	}

	void calcDimensions(Point& center, float& radius, float& thickness)
	{
		auto r = getRect();
		float mult = circleSize * CIRCLE_SIZE_MULTIPLIER;
		center = Point((r.left + r.right) * 0.5f, (r.top + r.bottom) * 0.5f);
		radius = std::min<float>(r.getWidth(), r.getHeight()) * mult; // Circle radius
		thickness = radius * THICKNESS_MULTIPLIER;
	}
	
	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext ) override
	{
		Graphics g(drawingContext);

		Point center;
		float radiusBg, thicknessBg;
		calcDimensionsBg(center, radiusBg, thicknessBg);

		float radius, thickness;
		calcDimensions(center, radius, thickness);

		auto brushLine = g.CreateSolidColorBrush(Color::FromHexString(pinForeground));
		auto gradientBrushBg = g.CreateRadialGradientBrush(Color::FromHexString(pinBackground), Color::Black, center, radiusBg);
		auto gradientBrush = g.CreateRadialGradientBrush(Color::FromHexString(pinBackground), Color::Black, center, radius);

		const float startAngle = 35.f; // Angle between "straight-down" and start of arc
		const float startAngleRadians = startAngle * (3.14159265358979323846f / 180.f); // Convert to radians
		const float quarterTurnClockwise = 3.14159265358979323846f * 0.5f;


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

		// Draw background circle
		g.FillCircle(center, radiusBg + thicknessBg * .5f, gradientBrushBg);
		// Draw the top circle
		g.FillCircle(center, radius + thickness * .5f, gradientBrush);
		// Draw the line
		g.DrawLine(center, movingPoint, brushLine, lineThickness, strokeStyle);

		return gmpi::MP_OK;
	
	}
};

namespace
{
	auto r = Register<VectorKnobXGui>::withId(L"VectorKnobX");
}

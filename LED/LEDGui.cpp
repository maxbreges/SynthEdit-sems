#include "mp_sdk_gui2.h"
#include "Drawing.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace gmpi;
using namespace GmpiDrawing;

class LedCircleGui final : public gmpi_gui::MpGuiGfxBase
{
	void onSetColor()
	{
		if (!pinpatchValue)
		{
			pinTopColor = pinTopColor1;
			pinBottomColor = pinBottomColor1;
		}
		if (pinpatchValue)
		{
			pinTopColor = pinBoolTopColor;
			pinBottomColor = pinBoolBottomColor;
		}
		invalidateRect();
	}
	FloatGuiPin pinpatchValue;
	StringGuiPin pinBgColor;
	FloatGuiPin pinTopCircleSize;
	StringGuiPin pinTopColor;
	StringGuiPin pinBottomColor;
	FloatGuiPin pinDotCircleSize;
	StringGuiPin pinTopColor1;
	StringGuiPin pinBottomColor1;
	StringGuiPin pinBoolTopColor;
	StringGuiPin pinBoolBottomColor;

public:
	LedCircleGui()
	{
		initializePin(pinpatchValue, static_cast<MpGuiBaseMemberPtr2>(&LedCircleGui::onSetColor));
		initializePin(pinBgColor, static_cast<MpGuiBaseMemberPtr2>(&LedCircleGui::onSetColor));
		initializePin(pinTopCircleSize, static_cast<MpGuiBaseMemberPtr2>(&LedCircleGui::onSetColor));
		initializePin(pinTopColor, static_cast<MpGuiBaseMemberPtr2>(&LedCircleGui::onSetColor));
		initializePin(pinBottomColor, static_cast<MpGuiBaseMemberPtr2>(&LedCircleGui::onSetColor));
		initializePin(pinDotCircleSize, static_cast<MpGuiBaseMemberPtr2>(&LedCircleGui::onSetColor));
		initializePin(pinTopColor1, static_cast<MpGuiBaseMemberPtr2>(&LedCircleGui::onSetColor));
		initializePin(pinBottomColor1, static_cast<MpGuiBaseMemberPtr2>(&LedCircleGui::onSetColor));
		initializePin(pinBoolTopColor, static_cast<MpGuiBaseMemberPtr2>(&LedCircleGui::onSetColor));
		initializePin(pinBoolBottomColor, static_cast<MpGuiBaseMemberPtr2>(&LedCircleGui::onSetColor));
	}

	//-----------------------------------
	void calcDimensionsBg(Point& centerBg, float& radiusBg, float& thicknessBg)
	{
		auto rBg = getRect();

		centerBg = Point((rBg.left + rBg.right) * 0.5f, (rBg.top + rBg.bottom) * 0.5f);
		radiusBg = (std::min)(rBg.getWidth(), rBg.getHeight()) * 0.43f;
		thicknessBg = radiusBg * 0.2f;
	}
	//-----------------------------------

	void calcDimensions(Point& center, float& radius, float& thickness)
	{
		float mult;
		auto r = getRect();
		mult = (pinTopCircleSize) * 0.4f;
		center = Point((r.left + r.right) * 0.5f, (r.top + r.bottom) * 0.5f);
		radius = (std::min)(r.getWidth(), r.getHeight()) * mult;
		thickness = radius * 0.2f;
	}

	void calcDimensionsDot(Point& centerDot, float& radiusDot, float& thicknessDot)
	{
		float multDot;
		auto rDot = getRect();
		multDot = (pinDotCircleSize * 0.3f) * 0.4f;
		if (pinDotCircleSize > 1)
		{
			pinDotCircleSize = 1;
		}

		centerDot = Point((rDot.left + rDot.right) * 0.4f, (rDot.top + rDot.bottom) * 0.4f);
		radiusDot = (std::min)(rDot.getWidth(), rDot.getHeight()) * multDot;
		thicknessDot = radiusDot * 1.2f;
	}

	Color FromHexStringBackwardCompatible(const std::wstring& s)
	{
		constexpr float oneOver255 = 1.0f / 255.0f;

		wchar_t* stopString;
		uint32_t hex = wcstoul(s.c_str(), &stopString, 16);
		float alpha = (hex >> 24) * oneOver255;

		return Color(se_sdk::FastGamma::sRGB_to_float((hex >> 16) & 0xff), se_sdk::FastGamma::sRGB_to_float((hex >> 8) & 0xff), se_sdk::FastGamma::sRGB_to_float(hex & 0xff), alpha);
	}

	Color FromHexStringBackwardCompatibleDot(const std::wstring& s)
	{
		constexpr float oneOver255Dot = 1.0f / 255.0f;

		wchar_t* stopStringDot;
		uint32_t hexDot = wcstoul(s.c_str(), &stopStringDot, 16);
		float alphaDot = (hexDot >> 24) * oneOver255Dot;

		return Color(se_sdk::FastGamma::sRGB_to_float((hexDot >> 16) & 0xff), se_sdk::FastGamma::sRGB_to_float((hexDot >> 8) & 0xff), se_sdk::FastGamma::sRGB_to_float(hexDot & 0xff), alphaDot);
	}

	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override
	{
		Graphics g(drawingContext);
		//-----------------------------------bg circle
		
		Point centerBg;
		float radiusBg;
		float thicknessBg;
		calcDimensionsBg(centerBg, radiusBg, thicknessBg);

		auto brushBackgroundBg = g.CreateSolidColorBrush(Color::FromHexString(pinBgColor));

		Size circleSizeBg(radiusBg, radiusBg);

		{
			g.FillCircle(centerBg, radiusBg + thicknessBg * 0.5f, brushBackgroundBg);
		}

		//----------------------------------- body circle
		Point center;
		float radius;
		float thickness;
		calcDimensions(center, radius, thickness);

		auto r = getRect();
		int width = r.right - r.left;
		int height = width;

		auto topCol = FromHexStringBackwardCompatible(pinTopColor);
		auto botCol = topCol;
		if (!pinBottomColor.getValue().empty())
		{
			botCol = FromHexStringBackwardCompatible(pinBottomColor);
		}

		Point point1(1, 0);
		Point point2(1, height);

		GradientStop gradientStops[]
		{
			{ 0.0f, topCol },
			{ 1.0f, botCol },
		};

		auto gradientBrush = g.CreateLinearGradientBrush(gradientStops, point1, point2);

		Size circleSize(radius, radius);
		{
			g.FillCircle(center, radius + thickness * 0.5f, gradientBrush);
		}

		//-----------------------------------------
		Point centerDot;
		float radiusDot;
		float thicknessDot;
		calcDimensionsDot(centerDot, radiusDot, thicknessDot);

		int widthDot = r.right - r.left;
		int heightDot = widthDot;

		auto topColDot = FromHexStringBackwardCompatibleDot(pinTopColor1);
		auto botColDot = topColDot;
		if (!pinBottomColor1.getValue().empty())
		{
			botColDot = FromHexStringBackwardCompatibleDot(pinBottomColor1);
		}

		Point point1Dot(1, 0);
		Point point2Dot(1, heightDot);

		GradientStop gradientStopsDot[]
		{
			{ 0.0f, topColDot },
			{ 1.0f, botColDot },
		};

		auto gradientBrushDot = g.CreateLinearGradientBrush(gradientStopsDot, point1Dot, point2Dot);

		Size circleSizeDot(radiusDot, radiusDot);
		{
			g.FillCircle(centerDot, radiusDot + thicknessDot * 0.5f, gradientBrushDot);	}


		//-----------------------------------------

		return gmpi::MP_OK;
	}
};

namespace
{
	auto r = Register<LedCircleGui>::withId(L"LED");
}

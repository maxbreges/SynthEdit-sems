#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include <string>
#include <sstream>
#include <iomanip>
#define _USE_MATH_DEFINES
#include <math.h>
#include "unicode_conversion.h"

using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;
using namespace JmUnicodeConversions;

GmpiDrawing_API::MP1_POINT pointPrevious;

class ButtonGui final : public gmpi_gui::MpGuiGfxBase
{
	bool previousState = false;

	int radius = 5;

	std::wstring HintColor = L"ffffffff";

	void onSetText()
	{
		invalidateRect();
	}

	void onSetCtrlClk()
	{
	}

	int32_t MP_STDCALL getToolTip(GmpiDrawing_API::MP1_POINT point, gmpi::IString* returnString) override {
		auto utf8String = (std::string)pinHint;
		returnString->setData(utf8String.data(), (int32_t)utf8String.size());
		return gmpi::MP_OK;
	}

	void onSetBgColor()
	{
		invalidateRect();
	}
	void onSetColor()
	{
		invalidateRect();
	}
	void onSetCornerRadius()
	{
		invalidateRect();
	}

	void onSetTextColor()
	{
		HintColor = L"ffffffff";
		invalidateRect();
	}

	void onSetFontSize()
	{
		invalidateRect();
	}

	void onSetFont()
	{
		invalidateRect();
	}

	void onSetAlignV()
	{
		invalidateRect();
	}

	void onSetAnimationPosition()
	{

		invalidateRect();
	}

	// Define padding (in pixels)
	int padding = 0;

	void onSetAlignY()
	{
		padding = pinAlignY;
		invalidateRect();
	}

	//functionality
	//hint
	//appearance

	StringGuiPin pinText;
	StringGuiPin pinHint;
	StringGuiPin pinBgColor;
	StringGuiPin pinColor;
	StringGuiPin pinTextColor;
	StringGuiPin pinFont;
	IntGuiPin pinFontSize;
	FloatGuiPin pinAnimPosShift;
	FloatGuiPin pinAnimPosAlt;
	BoolGuiPin pinMouseDown;
	IntGuiPin pinCornerRadius;
	BoolGuiPin pinCtrlClick;

	IntGuiPin pinAlignV;
	FloatGuiPin pinAlignY;


public:
	ButtonGui()
	{
		initializePin(pinText, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetText));
		initializePin(pinHint);
		initializePin(pinBgColor, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetBgColor));
		initializePin(pinColor, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetColor));
		initializePin(pinTextColor, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetTextColor));
		initializePin(pinFont, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetFont));
		initializePin(pinFontSize, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetFontSize));
		initializePin(pinAnimPosShift, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetAnimationPosition));
		initializePin(pinAnimPosAlt, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetAnimationPosition));
		initializePin(pinMouseDown);
		initializePin(pinCornerRadius, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetCornerRadius));
		initializePin(pinCtrlClick);

		initializePin(pinAlignV, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetAlignV));

		initializePin(pinAlignY, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetAlignY));;

	}

	float heightY = 1.f;

	int32_t MP_STDCALL onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{
		Point offset(point);

		if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL)
		{
			setCapture();

			goto bypass;
		}

		if ((flags & GG_POINTER_FLAG_FIRSTBUTTON) == 0)
		{
			return gmpi::MP_OK; // Let host handle
		}

		setCapture();

		pinMouseDown = true;

	bypass:
		
		return gmpi::MP_OK;

	}

	int32_t MP_STDCALL onMouseWheel(int32_t flags, int32_t delta, MP1_POINT point) override
	{
		float new_pos = pinAnimPosAlt;
		new_pos = new_pos + delta / 12000.0f;
		if (new_pos < 0.f)
			new_pos = 0.f;
		if (new_pos > 1.0f)
			new_pos = 1.0f;

		pinAnimPosAlt = new_pos;

		invalidateRect();

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

		if (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT)
		{
			Point offset(point.x - pointPrevious.x, point.y - pointPrevious.y); // TODO overload subtraction.

			float coarseness = 0.004f;

			//		if (modifier_keys::isHeldCtrl()) // <cntr> key magnifies
			/*if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL) // <cntr> key magnifies
				coarseness = 0.001f;*/

			float new_pos = pinAnimPosShift;
			new_pos = new_pos - coarseness * (float)offset.y;

			if (new_pos < 0.f)
				new_pos = 0.f;

			if (new_pos > 1.f)
				new_pos = 1.f;

			pinAnimPosShift = new_pos;
		}

		pointPrevious = point;

		invalidateRect();

		return gmpi::MP_OK;
	}

	int32_t MP_STDCALL onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{

		// Release control key
		if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL)
		{
			pinCtrlClick = true;
			pinCtrlClick = false;
		}

		// Ensure capture exists before releasing
		if (getCapture())
		{
			releaseCapture();
		}

		pinMouseDown = false;
		return gmpi::MP_OK;
	}

	// Helper to convert hex string to uint32_t
	uint32_t hexStringToUint32(const std::string& hexStr)
	{
		uint32_t value = 0;
		std::stringstream ss;
		ss << std::hex << hexStr;
		ss >> value;
		return value;
	}

	// Helper to convert uint32_t to hex string
	std::string uint32ToHexString(uint32_t value, size_t width = 6)
	{
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(width) << std::hex << value;
		return ss.str();
	}

	// Adjust color brightness based on brightness parameter (0..1)
	uint32_t adjustBrightness(uint32_t color, float brightness)
	{
		uint8_t r, g, b;
		extractRGB(color, r, g, b);

		r = static_cast<uint8_t>(std::min<>(255.0f, r * brightness));
		g = static_cast<uint8_t>(std::min<>(255.0f, g * brightness));
		b = static_cast<uint8_t>(std::min<>(255.0f, b * brightness));

		return combineRGB(r, g, b);
	}

	// Extract R, G, B components
	void extractRGB(uint32_t color, uint8_t& r, uint8_t& g, uint8_t& b)
	{
		r = (color >> 16) & 0xFF;
		g = (color >> 8) & 0xFF;
		b = color & 0xFF;
	}

	// Combine R, G, B into uint32
	uint32_t combineRGB(uint8_t r, uint8_t g, uint8_t b)
	{
		return (r << 16) | (g << 8) | b;
	}

	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override
	{
		Graphics g(drawingContext);
		ClipDrawingToBounds x(g, getRect());
		auto r = getRect();

		int width = r.right - r.left;
		int height = r.bottom - r.top;
		heightY = height;

		radius = (std::min)(radius, width / 2);
		radius = (std::min)(radius, height / 2);

		auto geometry = g.GetFactory().CreatePathGeometry();
		auto sink = geometry.Open();

		// define a corner 
		const float rightAngle = M_PI * 0.5f;
		// top left
		if (1)
		{
			sink.BeginFigure(Point(0, radius), FigureBegin::Filled);
			ArcSegment as(Point(radius, 0), Size(radius, radius), rightAngle);
			sink.AddArc(as);
		}
		else
		{
			sink.BeginFigure(Point(0, 0), FigureBegin::Filled);
		}

		// top right
		if (1)
		{
			sink.AddLine(Point(width - radius, 0));
			//		sink.AddArc(Corner, 270, 90);
			ArcSegment as(Point(width, radius), Size(radius, radius), rightAngle);
			sink.AddArc(as);
		}
		else
		{
			sink.AddLine(Point(width, 0));
		}

		// bottom right
		if (1)
		{
			sink.AddLine(Point(width, height - radius));
			//		sink.AddArc(Corner, 0, 90);
			ArcSegment as(Point(width - radius, height), Size(radius, radius), rightAngle);
			sink.AddArc(as);
		}
		else
		{
			sink.AddLine(Point(width, height));
		}

		// bottom left
		if (1)
		{
			sink.AddLine(Point(radius, height));
			ArcSegment as(Point(0, height - radius), Size(radius, radius), rightAngle);
			sink.AddArc(as);
		}
		else
		{
			sink.AddLine(Point(0, height));
		}

		// end path
		sink.EndFigure();
		sink.Close();

		Point point1(1, 0);
		Point point2(1, height);

		auto topCol = GmpiDrawing::Color::FromHexString(pinColor);
		auto botCol = GmpiDrawing::Color::FromHexString(pinBgColor);

		GradientStop gradientStops[]
		{
			{ 0.0f, topCol },
			{ 0.5f, topCol },
			{ 1.0f, botCol },
		};
		GradientStop gradientStopsR[]
		{
			{ 0.0f, topCol },
			{ 1.0f, botCol },
		};

		// Calculate gradient center based on pins
		float gradientX = r.left + 1 * r.getWidth();
		float gradientY = r.top + ((1 * -1) + 1) * r.getHeight();
		Point gradientCenter(gradientX, gradientY);

		auto gradientStopCollection = g.CreateGradientStopCollection(gradientStops);
		auto gradientStopCollectionR = g.CreateGradientStopCollection(gradientStopsR);

		// Create the radial gradient brush
		//float radiusR = std::min<>(r.getWidth()*0.5f, r.getHeight()*0.5f);
		RadialGradientBrushProperties radialGradientProps(gradientCenter, radius);
		auto BrushR = g.CreateRadialGradientBrush(radialGradientProps, BrushProperties(), gradientStopCollectionR);

		auto Brush = g.CreateLinearGradientBrush(gradientStopCollection, point1, point2);
		auto outlineBrush = g.CreateSolidColorBrush(botCol);
		float thickness = 0.0f;

			g.FillGeometry(geometry, Brush);
		g.DrawGeometry(geometry, outlineBrush, thickness);

		//drawing text

		
			int minFontSize = 8;
			int maxFontSize = 40;
			int fontSize;

			// Determine base font size
			if (pinFontSize != 0)
			{
				// Use user-defined font size, constrained within min/max
				fontSize = std::max<int32_t>(minFontSize, std::min<int32_t>(maxFontSize, pinFontSize));
			}
			else
			{
				int buttonHeight = getRect().bottom - getRect().top;
				fontSize = static_cast<int>(buttonHeight * 0.5f);
				fontSize = std::max<>(minFontSize, std::min<>(maxFontSize, fontSize));
			}


			// Create text format with the final font size
			std::string fontFace = std::string(pinFont);
			TextFormat textFormat = g.GetFactory().CreateTextFormat(fontSize, fontFace.c_str());

			// Set alignment
			ParagraphAlignment alignV[] = { ParagraphAlignment::Center, ParagraphAlignment::Near, ParagraphAlignment::Far };
			textFormat.SetParagraphAlignment(alignV[pinAlignV.getValue()]);
			textFormat.SetTextAlignment(TextAlignment::Center);

			// Original rect
			auto rect = getRect(); // GmpiDrawing::Rect

			// Create inset rect as GmpiDrawing::Rect
			GmpiDrawing::Rect textRect(
				rect.left,
				rect.top + padding,
				rect.right,
				rect.bottom - padding
			);

			auto brush = g.CreateSolidColorBrush(Color::FromHexString(HintColor));

			// Draw text within the button rect
			g.DrawTextU(getDisplayText(), textFormat, textRect, brush, 1);		

		//===================================
		return gmpi::MP_OK;
	}
	std::string getDisplayText()
	{
		return WStringToUtf8(pinText.getValue());
	}
};

namespace
{
	auto r = Register<ButtonGui>::withId(L"DisplayTextX");
}

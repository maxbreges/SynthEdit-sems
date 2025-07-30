#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include <string>
#include <sstream>
#include <iomanip>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;

class ButtonGui final : public gmpi_gui::MpGuiGfxBase
{
	bool previousState = false;
	bool SetResponse = 1;
	bool sharedState = 0;

	int Clicked = 0;
	int Stepped = 1;
	int Off = 2;

	int radius = 5;

	std::wstring HintColor = L"ffffffff";

	void onSetBoolOut()
	{		
		//pinBoolOut = sharedState;
		updateColor();
		invalidateRect();
	}

	void onSetCtrlClk()
	{
	}

	void onSetMouseOver()
	{
	}
	void onSetMouseDown()
	{
	}

	int32_t MP_STDCALL setHover(bool isMouseOverMe) override
	{
		pinMouseOver = isMouseOverMe;
		pinMouseOverIn = isMouseOverMe;

		onSetHint();
		return gmpi::MP_OK;
	}

	void onSetResponse()
	{
		int mResponse[] = { Clicked, Stepped, Off };
		SetResponse = (mResponse[pinResponse.getValue()]);
	}

	int32_t MP_STDCALL getToolTip(GmpiDrawing_API::MP1_POINT point, gmpi::IString* returnString) override {
		auto utf8String = (std::string)pinToolTip;
		returnString->setData(utf8String.data(), (int32_t)utf8String.size());
		return gmpi::MP_OK;
	}

	void onSetDisplayHint()
	{
		invalidateRect();
	}
	void onSetHint()
	{
		pinHintOut = pinHint;
		invalidateRect();
	}
	void onSetCornerRadius()
	{
		invalidateRect();
	}
	void onSetColor()
	{
		updateColor();
		invalidateRect();
	}
	void onSetGradient()
	{
		invalidateRect();
	}
	void onSetRadial()
	{
		if (pinRadial)
		{
			radius = 100;
		}
		else
			radius = 5;
		invalidateRect();
	}

	void onSetBrightness()
	{
		updateColor();
		invalidateRect();
	}

	void onSetHintColor()
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

	// Define padding (in pixels)
	int padding = 0;

	void onSetAlignY()
	{
		padding = pinAlignY;
		invalidateRect();
	}

	void onSetBoolIn()
	{
		sharedState = pinBoolIn;
		updateColor();
		invalidateRect();
	}


	void onSetMouseYPos()
	{
	}

	void onSetToolTip()
	{
	}

	void onSetCtrlClkLatch()
	{
	}

	//functionality
	//hint
	//appearance

	BoolGuiPin pinBoolOut;
	BoolGuiPin pinCtrlClk;
	BoolGuiPin pinMouseOver;
	IntGuiPin pinResponse;

	IntGuiPin pinCornerRadius;
	StringGuiPin pinColor;
	StringGuiPin pinBottomColor;

	StringGuiPin pinColorOut;
	FloatGuiPin pinGradientStop; //not used
	StringGuiPin pinColorGlow;
	FloatGuiPin pinBrightness; //would be nice to receive ADSR fade out
	FloatGuiPin pinGradientCenterX;
	FloatGuiPin pinGradientCenterY;
	BoolGuiPin pinRadial;

	BoolGuiPin pinDisplayHint;
	StringGuiPin pinHint;
	StringGuiPin pinHintOut;
	StringGuiPin pinHintColor;

	IntGuiPin pinFontSize;
	StringGuiPin pinFont;
	IntGuiPin pinAlignV;

	FloatGuiPin pinAlignY;
	FloatGuiPin pinOpacity;

	BoolGuiPin pinBoolIn;

	FloatGuiPin pinMouseYPos;
	StringGuiPin pinToolTip;
	BoolGuiPin pinCtrlClkLatch;
	BoolGuiPin pinMouseOverIn;
	BoolGuiPin pinMouseDown;

public:
	ButtonGui()
	{
		initializePin(pinBoolOut, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetBoolOut));
		initializePin(pinCtrlClk, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetCtrlClk));
		initializePin(pinMouseOver, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetMouseOver));
		initializePin(pinResponse, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetResponse));

		initializePin(pinCornerRadius, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetCornerRadius));
		initializePin(pinColor, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetColor));
		initializePin(pinBottomColor, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetColor));

		initializePin(pinColorOut, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetColor));
		initializePin(pinGradientStop, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetGradient));
		initializePin(pinColorGlow, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetColor));
		initializePin(pinBrightness, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetBrightness));
		initializePin(pinGradientCenterX, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetGradient));
		initializePin(pinGradientCenterY, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetGradient));
		initializePin(pinRadial, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetRadial));

		initializePin(pinDisplayHint, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetDisplayHint));
		initializePin(pinHint, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetHint));
		initializePin(pinHintOut, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetMouseOver));
		initializePin(pinHintColor, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetHintColor));

		initializePin(pinFontSize, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetFontSize));
		initializePin(pinFont, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetFont));
		initializePin(pinAlignV, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetAlignV));

		initializePin(pinAlignY, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetAlignY));;
		initializePin(pinOpacity, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetColor));;

		initializePin(pinBoolIn, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetBoolIn));
		initializePin(pinMouseYPos, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetMouseYPos));
		initializePin(pinToolTip, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetToolTip));

		initializePin(pinCtrlClkLatch, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetCtrlClkLatch));
		initializePin(pinMouseOverIn, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetMouseOver));
		initializePin(pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetMouseDown));
	}

	float mult = 1.f;
	float heightY = 1.f;

	int32_t MP_STDCALL onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{
		Point offset(point);

		mult = 1.f / heightY;

		pinMouseYPos = mult * (1-(float)offset.y) + 1.f;

		if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL)
		{
			setCapture();
			pinCtrlClkLatch = !pinCtrlClkLatch;
			goto bypass;
		}

		if ((flags & GG_POINTER_FLAG_FIRSTBUTTON) == 0)
		{
			return gmpi::MP_OK; // Let host handle
		}

		setCapture();

	// Handle right-clicks or other non-primary button presses
		if (pinResponse.getValue() != 2)
		{
			if (pinResponse.getValue() == 1) // Stepped mode
			{
				sharedState = !sharedState; // toggle
			}
			else // Clicked mode
			{
				sharedState = true; // momentary ON
			}
		}

		pinBoolOut = true;
		pinMouseDown = true;
		//pinCtrlClkLatch = false;
		bypass:
		return gmpi::MP_OK;

	}

	int32_t MP_STDCALL onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{

		// Release control key
		if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL)
		{
			pinCtrlClk = true;			
			pinCtrlClk = false;
		}

		// Ensure capture exists before releasing
		if (getCapture())
		{
			releaseCapture();
		}

		if (pinResponse.getValue() == 0) // Clicked mode
		{
			sharedState = false; // reset after click
		}
		// In stepped mode, keep toggle state until next press
		pinBoolOut = sharedState;
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


	void updateColor()
	{
		// Read input color
		std::string inputHex = pinColor;
		if (inputHex.size() >= 2 && inputHex[0] == '0' && (inputHex[1] == 'x' || inputHex[1] == 'X'))
			inputHex = inputHex.substr(2);

		uint32_t color = hexStringToUint32(inputHex);
		float PinBrightness = pinBrightness + 0.44f;
		// Read brightness (0..1)
		float brightness = 0.5f * PinBrightness;
		float brightnessGlow = 0.7f * PinBrightness;

		// Clamp brightness
		if (!sharedState)
		{
			brightness = 0.55f * PinBrightness;
			brightnessGlow = 0.75f * PinBrightness;
			HintColor = pinHintColor;
		}

		if (sharedState)
		{
			brightness = 0.85f * PinBrightness;
			brightnessGlow = 1.0f * PinBrightness;
			HintColor = pinBottomColor;
		}

		// Adjust color brightness
		uint32_t adjustedColor = adjustBrightness(color, brightness);
		uint32_t adjustedColorGlow = adjustBrightness(color, brightnessGlow);

		// Convert adjusted color to hex string (8 hex digits)
		std::string resultHex = uint32ToHexString(adjustedColor, 8);
		std::string resultHexGlow = uint32ToHexString(adjustedColorGlow, 8);

		// Map opacity (pinOpacity) from [0,1] to [00,FF]
		uint8_t opacityHex = static_cast<uint8_t>(pinOpacity * 255.0f);
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(opacityHex);
		std::string opacityStr = ss.str();

		// Replace first two characters with opacity
		if (resultHex.size() >= 8)
		{
			resultHex.replace(0, 2, opacityStr);
		}

		if (resultHexGlow.size() >= 8)
		{
			resultHexGlow.replace(0, 2, opacityStr);
		}

		// Output
		pinColorOut = resultHex;
		pinColorGlow = resultHexGlow;
	}

	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override
	{
		Graphics g(drawingContext);
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

		auto topCol = GmpiDrawing::Color::FromHexString(pinColorOut);
		auto botCol = GmpiDrawing::Color::FromHexString(pinBottomColor);
		auto glowCol = GmpiDrawing::Color::FromHexString(pinColorGlow);

		GradientStop gradientStops[]
		{
			{ 0.0f, topCol },
			{ 0.33f, glowCol },
			{ 0.5f, topCol },
			{ 1.0f, botCol },
		};
		GradientStop gradientStopsR[]
		{
			{ 0.0f, topCol },
			{ 1.0f, botCol },
		};

		// Calculate gradient center based on pins
		float gradientX = r.left + pinGradientCenterX * r.getWidth();
		float gradientY = r.top + ((pinGradientCenterY * -1) + 1) * r.getHeight();
		Point gradientCenter(gradientX, gradientY);

		auto gradientStopCollection = g.CreateGradientStopCollection(gradientStops);
		auto gradientStopCollectionR = g.CreateGradientStopCollection(gradientStopsR);

		// Create the radial gradient brush
		//float radiusR = std::min<>(r.getWidth()*0.5f, r.getHeight()*0.5f);
		RadialGradientBrushProperties radialGradientProps(gradientCenter, radius);
		auto BrushR = g.CreateRadialGradientBrush(radialGradientProps, BrushProperties(), gradientStopCollectionR);

		auto Brush = g.CreateLinearGradientBrush(gradientStopCollection, point1, point2);
		auto outlineBrush = g.CreateSolidColorBrush(botCol);
		float thickness = 1.f;

		if (pinRadial)
		{
			g.FillGeometry(geometry, BrushR);
			onSetRadial();
		}
		else

			g.FillGeometry(geometry, Brush);
		g.DrawGeometry(geometry, outlineBrush, thickness);

		//drawing text
		if (pinDisplayHint)
		{
			int minFontSize = 8;
			int maxFontSize = 20;
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

			// Adjust font size if pressed
			if (sharedState)
			{
				fontSize = std::max<>(minFontSize, fontSize - 1); // reduce size when pressed

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
			g.DrawTextU(pinHint, textFormat, textRect, brush, 1);
		}

		//===================================
		return gmpi::MP_OK;
	}
};

namespace
{
	auto r = Register<ButtonGui>::withId(L"Button");
}

#include "mp_sdk_gui2.h"
#include "Drawing.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace gmpi;
using namespace GmpiDrawing;

class ButtonGui final : public gmpi_gui::MpGuiGfxBase
{
	void onRedraw()
	{
		invalidateRect();
	}

 	void onSetAnimationPosition()
	{	
		if (!pinAnimPos)
		{
			pinColor = pinColorOn0;			
			pinColorB = pinColorOn1;
		}
		if (pinAnimPos)
		{
			pinColor = pinColorOn0Down;
			pinColorB = pinColorOn1Down;
		}
		previousState = pinAnimPos;
		invalidateRect();
	} 	

 	void onSetMouseDown()
	{	
		invalidateRect();
	}

	void onSetText()
	{
		invalidateRect();
	}

	void onSetTextColor()
	{
		invalidateRect();
	}

	void onSetStyle()
	{
		invalidateRect();
	}
	void onSetAlignV()
	{
		invalidateRect();
	}

	FloatGuiPin pinCornerRadius;
	BoolGuiPin pinTopLeft;
	BoolGuiPin pinTopRight;
	BoolGuiPin pinBottomLeft;
	BoolGuiPin pinBottomRight;

 	FloatGuiPin pinAnimPos;
	BoolGuiPin pinMouseDown; 

	StringGuiPin pinColor;	
	StringGuiPin pinColorB;

	StringGuiPin pinColorOn0;
	StringGuiPin pinColorOn0Down;
	StringGuiPin pinColorOn1;
	StringGuiPin pinColorOn1Down;

	StringGuiPin pinText;
	StringGuiPin pinTextColor;
	StringGuiPin pinFont;
	FloatGuiPin pinFontSize;
	IntGuiPin pinAlignV;


public:
	ButtonGui()
	{
		initializePin(pinCornerRadius, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onRedraw));
		initializePin(pinTopLeft, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onRedraw));
		initializePin(pinTopRight, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onRedraw));
		initializePin(pinBottomLeft, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onRedraw));
		initializePin(pinBottomRight, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onRedraw));

		initializePin( pinAnimPos, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetAnimationPosition) );
		initializePin(pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetMouseDown));

		initializePin(pinColor, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onRedraw));
		initializePin(pinColorB, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onRedraw));
		
		initializePin(pinColorOn0, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onRedraw));
		initializePin(pinColorOn0Down, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onRedraw));
		initializePin(pinColorOn1, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onRedraw));
		initializePin(pinColorOn1Down, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onRedraw));
		initializePin(pinText, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetText));
		initializePin(pinTextColor, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetTextColor));
		initializePin(pinFont, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetStyle));
		initializePin(pinFontSize, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onRedraw));
		initializePin(pinAlignV, static_cast<MpGuiBaseMemberPtr2>(&ButtonGui::onSetAlignV));

	}

	bool previousState = false;
	
	int32_t onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point)
	{
		// Let host handle right-clicks.
		if ((flags & 0x10) == 0)
		{
			return gmpi::MP_OK; // Indicate successful hit, so right-click menu can show.
		}

		setCapture();

		pinMouseDown = true;

		
			bool newState = !previousState;
			previousState = newState;
			pinAnimPos = previousState;
		

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


	Color FromHexStringBackwardCompatible(const std::wstring& s)
	{
		constexpr float oneOver255 = 1.0f / 255.0f;

		wchar_t* stopString;
		uint32_t hex = wcstoul(s.c_str(), &stopString, 16);
		float alpha = (hex >> 24) * oneOver255;

		return Color(se_sdk::FastGamma::sRGB_to_float((hex >> 16) & 0xff), se_sdk::FastGamma::sRGB_to_float((hex >> 8) & 0xff), se_sdk::FastGamma::sRGB_to_float(hex & 0xff), alpha);
	}

	int32_t OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext)
	{
		Graphics g(drawingContext);

		auto r = getRect();
		int width = r.right - r.left;
		int height = r.bottom - r.top;

		auto topCol = FromHexStringBackwardCompatible(pinColorB);
		auto botCol = topCol;
		if (!pinColor.getValue().empty())
		{
			botCol = FromHexStringBackwardCompatible(pinColor);
		}

		int radius = (int)pinCornerRadius;

		radius = (std::min)(radius, width / 2);
		radius = (std::min)(radius, height / 2);

		auto geometry = g.GetFactory().CreatePathGeometry();
		auto sink = geometry.Open();

		// define a corner 
		const float rightAngle = M_PI * 0.5f;
		// top left
		if (pinTopLeft)
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
		if (pinTopRight)
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
		if (pinBottomRight)
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
		if (pinBottomLeft)
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

		GradientStop gradientStops[]
		{
			{ 0.0f, topCol },
			{ 1.0f, botCol },
		};

		auto gradientBrush = g.CreateLinearGradientBrush(gradientStops, point1, point2);

		g.FillGeometry(geometry, gradientBrush);
		
		//drawing text

		int font_size = 5;

		if (pinFontSize < 5)

		{
			font_size = 5;
		}

		else
		{
			font_size = pinFontSize;
		}

		//delivering font
		std::string str = { pinFont };
		const char* fontFace = str.c_str();
		TextFormat textFormat = g.GetFactory().CreateTextFormat(font_size, fontFace);

		ParagraphAlignment alignV[] = { ParagraphAlignment::Near, ParagraphAlignment::Far, ParagraphAlignment::Center };
		
		textFormat.SetParagraphAlignment(alignV[pinAlignV.getValue()]);

		textFormat.SetTextAlignment(TextAlignment::Center);

		auto brush = g.CreateSolidColorBrush(Color::FromHexString(pinTextColor));

		g.DrawTextU(pinText, textFormat, getRect(), brush, 0);

		return gmpi::MP_OK;
	}
};

namespace
{
	auto r = Register<ButtonGui>::withId(L"Button");
}

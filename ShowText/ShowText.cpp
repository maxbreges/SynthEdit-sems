#include "mp_sdk_gui2.h"
#include "Drawing.h"

using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;

class ShowVoltsGui final : public gmpi_gui::MpGuiGfxBase
{
 	void onSetMouseDown()
	{
		invalidateRect();
	}
	void onSetTopColor()
	{
		invalidateRect();
	}
	void onSetBgColor()
	{
		invalidateRect();
	}
 	void onSetTextColor()
	{
		invalidateRect();
	}

 	void onSetFont()
	{
		invalidateRect();
	}

 	void onSetText()
	{
		invalidateRect();
	}

 	BoolGuiPin pinMouseDown;
	StringGuiPin pinTopColor;
	StringGuiPin pinBgColor;
 	StringGuiPin pinTextColor;
 	StringGuiPin pinFont;
 	StringGuiPin pinText;

public:
	ShowVoltsGui()
	{
		initializePin( pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&ShowVoltsGui::onSetMouseDown) );
		initializePin(pinTopColor, static_cast<MpGuiBaseMemberPtr2>(&ShowVoltsGui::onSetTopColor));
		initializePin( pinBgColor, static_cast<MpGuiBaseMemberPtr2>(&ShowVoltsGui::onSetBgColor) );
		initializePin(pinTextColor, static_cast<MpGuiBaseMemberPtr2>(&ShowVoltsGui::onSetTextColor));
		initializePin( pinFont, static_cast<MpGuiBaseMemberPtr2>(&ShowVoltsGui::onSetFont) );
		initializePin( pinText, static_cast<MpGuiBaseMemberPtr2>(&ShowVoltsGui::onSetText) );
	}

	int32_t onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point)
	{
		// Let host handle right-clicks.
		if ((flags & GG_POINTER_FLAG_FIRSTBUTTON) == 0)
		{
			return gmpi::MP_OK; // Indicate successful hit, so right-click menu can show.
		}

		setCapture();

		pinMouseDown = true;

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

	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override
	{
		Graphics g(drawingContext);
		//ClipDrawingToBounds x(g, getRect());

		auto brush = g.CreateSolidColorBrush(Color::FromHexString(pinBgColor));
		//g.FillRectangle(getRect(), brush);

		//======================================
		auto r = getRect();
		int width = r.right - r.left;
		int height = r.bottom - r.top;

		auto topCol = FromHexStringBackwardCompatible(pinTopColor);
		auto botCol = topCol;
		if (!pinBgColor.getValue().empty())
		{
			botCol = FromHexStringBackwardCompatible(pinBgColor);
		}

		int radius = (int)5;

		radius = (std::min)(radius, width / 2);
		radius = (std::min)(radius, height / 2);

		auto geometry = g.GetFactory().CreatePathGeometry();
		auto sink = geometry.Open();

		// define a corner 
		const float rightAngle = 3.14159265358979323846 * 0.5f;
		// top left
		if (5)
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
		if (5)
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
		if (5)
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
		if (5)
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

		std::string str = { pinFont };
		const char* fontFace = str.c_str();
		TextFormat tf = g.GetFactory().CreateTextFormat(20, fontFace);

		tf.SetParagraphAlignment(ParagraphAlignment::Center),

			tf.SetTextAlignment(TextAlignment::Center);

		brush.SetColor(Color::FromHexString(pinTextColor));
		g.DrawTextW(pinText.getValue(), tf, getRect(), brush);

		return gmpi::MP_OK;
	}
};

namespace
{
	auto r = Register<ShowVoltsGui>::withId(L"ShowText");
}

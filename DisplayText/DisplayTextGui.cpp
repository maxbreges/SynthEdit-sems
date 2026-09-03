#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include "mp_gui.h"
#include <sstream>
#include <iomanip>

using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;
using namespace JmUnicodeConversions;

GmpiDrawing_API::MP1_POINT pointPrevious;
int padding = 0;

class DisplayText final : public gmpi_gui::MpGuiGfxBase
{
	void onSetTopColor()
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
	void onSetTextFont()
	{
		invalidateRect();
	}
	void onSetFontSize()
	{
		invalidateRect();
	}
	void onSetAnimationPosition()
	{
		invalidateRect();
	}
	void onSetCornerRadius()
	{
		invalidateRect();
	}
	void onSetAlignV() { invalidateRect(); }
	void onSetAlignY()
	{
		padding = static_cast<int>(pinAlignY);
		invalidateRect();
	}

	void onSetMultiline()
	{
		invalidateRect();
	}

	StringGuiPin pinText;
	StringGuiPin pinHint;
	StringGuiPin pinBgColor;
	StringGuiPin pinTopColor;
	StringGuiPin pinTextColor;
	StringGuiPin pinFont;
	FloatGuiPin pinFontSize;
	BoolGuiPin pinMouseDown;
	FloatGuiPin pinCornerRadius;
	BoolGuiPin pinCtrlClick;
	IntGuiPin pinAlignV;
	FloatGuiPin pinAlignY;

	BoolGuiPin pinMultiline;


public:
	DisplayText()
	{
		initializePin(pinText, static_cast<MpGuiBaseMemberPtr2>(&DisplayText::onSetText));
		initializePin(pinHint);
		initializePin(pinBgColor);
		initializePin(pinTopColor, static_cast<MpGuiBaseMemberPtr2>(&DisplayText::onSetTopColor));
		initializePin(pinTextColor, static_cast<MpGuiBaseMemberPtr2>(&DisplayText::onSetTextColor));
		initializePin(pinFont, static_cast<MpGuiBaseMemberPtr2>(&DisplayText::onSetTextFont));
		initializePin(pinFontSize, static_cast<MpGuiBaseMemberPtr2>(&DisplayText::onSetFontSize));
		initializePin(pinMouseDown);
		initializePin(pinCornerRadius, static_cast<MpGuiBaseMemberPtr2>(&DisplayText::onSetCornerRadius));
		initializePin(pinCtrlClick);
		initializePin(pinAlignV, static_cast<MpGuiBaseMemberPtr2>(&DisplayText::onSetAlignV));
		initializePin(pinAlignY, static_cast<MpGuiBaseMemberPtr2>(&DisplayText::onSetAlignY));
		initializePin(pinMultiline, static_cast<MpGuiBaseMemberPtr2>(&DisplayText::onSetMultiline));
	}

	//========================================

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
		if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL)
		{
			pinCtrlClick = true;
		}
			
		pointPrevious = point;	// note first point.
		pinMouseDown = true;
		setCapture();

		return gmpi::MP_OK;
	}

	int32_t onPointerUp(int32_t flags, struct GmpiDrawing_API::MP1_POINT point)
	{
		pinCtrlClick = false;
		pinMouseDown = false;
		releaseCapture();
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
		ClipDrawingToBounds x(g, getRect());

		auto brush = g.CreateSolidColorBrush(Color::FromHexString(pinTextColor));

		//g.FillRectangle(getRect(), brush);

		//======================================
		auto r = getRect();
		float width = r.right - r.left;
		float height = r.bottom - r.top;

		auto topCol = FromHexStringBackwardCompatible(pinTopColor);
		auto botCol = topCol;
		if (!pinBgColor.getValue().empty())
		{
			botCol = FromHexStringBackwardCompatible(pinBgColor);
		}

		float radius = pinCornerRadius;

		radius = (std::min<float>)(radius, width / 2);
		radius = (std::min<float>)(radius, height / 2);

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

		//=============================================================
		std::string str = { pinFont };
		const char* fontFace = str.c_str();
		TextFormat tf = g.GetFactory().CreateTextFormat(pinFontSize, fontFace);

		tf.SetParagraphAlignment(ParagraphAlignment::Center),

			tf.SetTextAlignment(TextAlignment::Center);
		
		if (pinMultiline)
		{
			r.bottom = r.top + (r.bottom - r.top);
			tf.SetWordWrapping(WordWrapping::Wrap);	}
		else {
			//r.bottom = r.top + pinFontSize;
			tf.SetWordWrapping(WordWrapping::NoWrap); }

		brush.SetColor(Color::FromHexString(pinTextColor));

		// Platform-specific text drawing
#ifdef _WIN32
		g.DrawTextU(WStringToUtf8(pinText.getValue()), tf, r, brush, 1);
#else
		g.DrawTextU(pinText, tf, r, brush, 1);
#endif

		return gmpi::MP_OK;
	}

};

namespace
{
	auto r = Register<DisplayText>::withId(L"DisplayText");
}

#include "mp_sdk_gui2.h"
#include "Drawing.h"
//#include "..\shared\unicode_conversion.h"
//#include "..\SubControlsXp\TextSubcontrol.h"
#include "mp_gui.h"
#include <sstream>
#include <iomanip>

using namespace std;
using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;
//using namespace JmUnicodeConversions;

GmpiDrawing_API::MP1_POINT pointPrevious;
GmpiGui::PopupMenu nativeMenu;

class DisplayList final : public gmpi_gui::MpGuiGfxBase
{
	void onSetHint()
	{
		// pinHint changed
	}

	void onSetBgColor()
	{
		// pinBgColor changed
	}
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


	void onSetMouseDown()
	{
	}

	void onSetCornerRadius()
	{
		invalidateRect();
	}

	StringGuiPin pinText;
	StringGuiPin pinHint;
	StringGuiPin pinBgColor;
	StringGuiPin pinTopColor;
	StringGuiPin pinTextColor;
	StringGuiPin pinFont;
	IntGuiPin pinFontSize;
	FloatGuiPin pinAnimPosShift;
	FloatGuiPin pinAnimPosAlt;
	BoolGuiPin pinMouseDown;
	IntGuiPin pinCornerRadius;
	BoolGuiPin pinCtrlClick;

public:
	DisplayList()
	{
		initializePin(pinText, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetText));
		initializePin(pinHint, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetHint));
		initializePin(pinBgColor, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetBgColor));
		initializePin(pinTopColor, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetTopColor));		
		initializePin(pinTextColor, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetTextColor));
		initializePin(pinFont, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetTextFont));
		initializePin(pinFontSize, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetFontSize));
		initializePin(pinAnimPosShift, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetAnimationPosition));
		initializePin(pinAnimPosAlt, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetAnimationPosition));
		initializePin(pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetMouseDown));
		initializePin(pinCornerRadius, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetCornerRadius));
		initializePin(pinCtrlClick);
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

	int32_t onPointerUp(int32_t flags, struct GmpiDrawing_API::MP1_POINT point)
	{
		pinMouseDown = false;
		pinCtrlClick = false;

		releaseCapture();

		return gmpi::MP_OK;
		//invalidateRect();
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

		int radius = (int)pinCornerRadius;

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

		//=============================================================
		std::string str = { pinFont };
		const char* fontFace = str.c_str();
		TextFormat tf = g.GetFactory().CreateTextFormat(pinFontSize, fontFace);

		tf.SetParagraphAlignment(ParagraphAlignment::Center),

			tf.SetTextAlignment(TextAlignment::Center);
		auto brush = g.CreateSolidColorBrush(Color::White);
		brush.SetColor(Color::FromHexString(pinTextColor));
		g.DrawTextW(pinText.getValue(), tf, getRect(), brush);

		return gmpi::MP_OK;
	}
};

namespace
{
	auto r = Register<DisplayList>::withId(L"DisplayText");
}

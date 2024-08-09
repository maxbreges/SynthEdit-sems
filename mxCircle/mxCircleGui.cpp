#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include "../shared/unicode_conversion.h"

using namespace gmpi;
using namespace gmpi_gui_api;
using namespace GmpiDrawing;
using namespace JmUnicodeConversions;

GmpiDrawing_API::MP1_POINT_L pointPrevious;

class mxCircleGui final : public gmpi_gui::MpGuiGfxBase
{
 	void onSetAnimPosShift()
	{
	}

 	void onSetAnimPosALT()
	{
	}

	void onSetColor0()
	{
		invalidateRect();
	}

 	void onSetBgColor()
	{
		invalidateRect();
	}

 	void onSetTopColor()
	{
		invalidateRect();
	}

 	void onSetTopCircleSize()
	{
		// pinTopCircleSize changed
	}
	void onSetMouseDown()
	{
		invalidateRect();
	}
	void onSetMouseUp()
	{
		invalidateRect();
	}
	void onSetMouseOver()
	{
		invalidateRect();
	}
	void onSetText()
	{
		invalidateRect();
	}
	void onSetTextb()
	{
		invalidateRect();
	}
	void onSetAcc()
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
	void onSetFontSize()
	{
		invalidateRect();
	}
	void onSetORIn()
	{
		if (!pinORIn)
		pinToDSP = false;
		invalidateRect();
	}
	void onTrigGui()
	{
		invalidateRect();
	}
	void onToDSP()
	{
		invalidateRect();
	}
	void onSetMin()
	{
		invalidateRect();
	}
	void onSetMax()
	{
		invalidateRect();
	}
	void onSetMinAlt()
	{
		invalidateRect();
	}
	void onSetMaxAlt()
	{
		invalidateRect();
	}

 	FloatGuiPin pinAnimPosShift;
 	FloatGuiPin pinAnimPosALT;
	StringGuiPin pinColor0;
 	StringGuiPin pinBgColor;
 	StringGuiPin pinTopColor;
 	FloatGuiPin pinTopCircleSize;
 	BoolGuiPin pinMouseDown;
	BoolGuiPin pinMouseOver;
	StringGuiPin pinText;
	StringGuiPin pinTextb;
	BoolGuiPin pinSharpFlat;
	StringGuiPin pinTextColor;
	StringGuiPin pinFont;
	FloatGuiPin pinFontSize;
	BoolGuiPin pinORIn;
	BoolGuiPin pinTrigGui;
	BoolGuiPin pinToDSP;
	FloatGuiPin pinOnMouseDownValue;
	FloatGuiPin pinOnMouseUpValue;
	FloatGuiPin pinValueMin;
	FloatGuiPin pinValueMax;
	FloatGuiPin pinOnMouseDownValueAlt;
	FloatGuiPin pinOnMouseUpValueAlt;
	FloatGuiPin pinValueMinAlt;
	FloatGuiPin pinValueMaxAlt;
	
public:
	mxCircleGui()
	{
		initializePin(pinAnimPosShift, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetAnimPosShift));
		initializePin(pinAnimPosALT, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetAnimPosALT));
		initializePin(pinColor0, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetColor0));
		initializePin(pinBgColor, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetBgColor));
		initializePin(pinTopColor, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetTopColor));
		initializePin(pinTopCircleSize, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetTopCircleSize));
		initializePin(pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetMouseDown));
		initializePin(pinMouseOver, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetMouseOver));
		initializePin(pinText, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetText));
		initializePin(pinTextb, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetTextb));
		initializePin(pinSharpFlat, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetAcc));
		initializePin(pinTextColor, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetTextColor));
		initializePin(pinFont, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetStyle));
		initializePin(pinFontSize, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetFontSize));
		initializePin(pinORIn, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetORIn));
		initializePin(pinTrigGui, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onTrigGui));
		initializePin(pinToDSP, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onToDSP));
		initializePin(pinOnMouseDownValue, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetMouseDown));
		initializePin(pinOnMouseUpValue, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetMouseUp));
		initializePin(pinValueMin, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetMin));
		initializePin(pinValueMax, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetMax));
		initializePin(pinOnMouseDownValueAlt, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetMouseDown));
		initializePin(pinOnMouseUpValueAlt, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetMouseUp));
		initializePin(pinValueMinAlt, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetMinAlt));
		initializePin(pinValueMaxAlt, static_cast<MpGuiBaseMemberPtr2>(&mxCircleGui::onSetMaxAlt));
		}

	virtual int32_t MP_STDCALL setHover(bool isMouseOverMe) override
	{
	
		pinMouseOver = isMouseOverMe;

		if (pinORIn && pinMouseOver)
		{
			pinToDSP = true;
		}

		if ((!pinORIn) || (!pinMouseOver))
		{
			pinToDSP = false;
		}
		invalidateRect();
		return gmpi::MP_OK;
	}
	
	//Mouse Down
	int32_t onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{		
		pinOnMouseDownValue = point.y;
		pinOnMouseDownValueAlt = point.y;

			// Let host handle right-clicks.
			if ((flags & GG_POINTER_FLAG_FIRSTBUTTON) == 0)
			{
				return gmpi::MP_OK; // Indicate successful hit, so right-click menu can show.
			}
			else

			{
				setCapture();

				pinMouseDown = true;
				pinToDSP = true;	
				pinMouseOver = pinMouseDown;
			}			

		invalidateRect();

		return gmpi::MP_OK;
	}

	//animation position
	int32_t MP_STDCALL onPointerMove(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{
		if (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT)
		{
			if ((flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT) == 0)
			{
				pinMouseOver = false;
			}
			pinMouseOver = pinMouseDown;
			pinORIn = false;

			if (!getCapture())
			{
				return gmpi::MP_UNHANDLED;
			}

			PointL offset(point.x, pinOnMouseDownValue - point.y); // TODO overload subtraction.

			pinAnimPosShift = pinOnMouseUpValue + offset.y;

			if (pinAnimPosShift < pinValueMin)
				pinAnimPosShift = pinValueMin;

			if (pinAnimPosShift > pinValueMax)
				pinAnimPosShift = pinValueMax;

			invalidateRect();

		}

		if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL)
		{
			if ((flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL) == 0)
			{
				pinMouseOver = false;
			}
			pinMouseOver = pinMouseDown;
			pinORIn = false;

			if (!getCapture())
			{
				return gmpi::MP_UNHANDLED;
			}

			PointL offset(point.x, pinOnMouseDownValueAlt - point.y); // TODO overload subtraction.

			pinAnimPosALT = pinOnMouseUpValueAlt + offset.y;

			if (pinAnimPosALT < pinValueMinAlt)
				pinAnimPosALT = pinValueMinAlt;

			if (pinAnimPosALT > pinValueMaxAlt)
				pinAnimPosALT = pinValueMaxAlt;

			invalidateRect();

		}
		return gmpi::MP_OK;
	}
		

	//mouse up
	int32_t onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point) override

	{	
		if (!getCapture())
		{
			return gmpi::MP_UNHANDLED;
		}		

		pinOnMouseUpValue = pinAnimPosShift;
		pinOnMouseUpValueAlt = pinAnimPosALT;

		releaseCapture();		

		pinMouseDown = false;
		pinToDSP = false;
		pinMouseOver = false;

		if (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT)
		{
			pinMouseOver = false;
			pinMouseDown = false;
			pinToDSP = false;
		}

		if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL)
		{
			pinMouseOver = false;
			pinMouseDown = false;
			pinToDSP = false;
		}
		
		invalidateRect();

		return gmpi::MP_OK;
	}


	//circles
		//bg void
	void calcDimensions(Point& center, float& radius, float& thickness)
	{
		auto r = getRect();

		center = Point((r.left + r.right) * 0.5f, (r.top + r.bottom) * 0.5f);
		radius = (std::min)(r.getWidth(), r.getHeight()) * 0.4f;
		thickness = radius * 0.2f;
	}

	//top void
	void calcDimensions1(Point& center1, float& radius1, float& thickness1)
	{
		float mult;
		auto r1 = getRect();
		mult = (pinTopCircleSize) * 0.4f;
		center1 = Point((r1.left + r1.right) * 0.5f, (r1.top + r1.bottom) * 0.5f);
		radius1 = (std::min)(r1.getWidth(), r1.getHeight()) * mult;
		thickness1 = radius1 * 0.2f;
	}

	//drawing
	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override
	{
		Graphics g(drawingContext);

		//background circle
		Point center;
		float radius;
		float thickness;
		calcDimensions(center, radius, thickness);

		if ((pinORIn && pinMouseOver) || (pinMouseDown && pinMouseOver) || (pinTrigGui))
		{
			auto brushBackground = g.CreateSolidColorBrush(Color::FromHexString(pinBgColor));

			if ((!pinORIn) || (!pinMouseOver) || (!pinMouseDown) || (!pinTrigGui))
			{
				auto brush = g.CreateSolidColorBrush(Color::FromHexString(pinColor0));
			}
		
			Size circleSize1(radius, radius);

			{
				g.FillCircle(center, radius + thickness * 0.5f, brushBackground);
			}

		}

		//top circle
		Point center1;
		float radius1;
		float thickness1;
		calcDimensions1(center1, radius1, thickness1);

		if ((pinORIn && pinMouseOver) || (pinMouseDown && pinMouseOver) || (pinTrigGui))
		{
			auto brushTopColor = g.CreateSolidColorBrush(Color::FromHexString(pinTopColor));

			if ((!pinORIn) || (!pinMouseOver) || (!pinMouseDown) || (!pinTrigGui))
			{
				auto brush = g.CreateSolidColorBrush(Color::FromHexString(pinColor0));
			}

			Size circleSize1(radius1, radius1);

			{
				g.FillCircle(center1, radius1 + thickness1 * 0.5f, brushTopColor);
			}
		}

		//drawing text
		float font_size = -1;

		if (pinFontSize >= 0.01f)
		{
			if (pinFontSize <= 10.f)
			{
				font_size = (pinFontSize * 0.16f) * radius;
			}

			if (pinFontSize > 10.f)
			{
				font_size = 10.f;
			}

			if (pinFontSize < 0.01f)
			{
				font_size = 0.01f;
			}
		}
		else
		{
			font_size = radius;
		}

		//delivering font
		std::string str = { pinFont };
		const char* fontFace = str.c_str();
		TextFormat textFormat = g.GetFactory().CreateTextFormat(font_size, fontFace);

		textFormat.SetParagraphAlignment(ParagraphAlignment::Center),

			textFormat.SetTextAlignment(TextAlignment::Center);

		if ((pinORIn && pinMouseOver) || (pinMouseDown && pinMouseOver) || (pinTrigGui))
		{
			auto brush = g.CreateSolidColorBrush(Color::FromHexString(pinTextColor));

			if ((!pinORIn) || (!pinMouseOver) || (!pinMouseDown) || (!pinTrigGui))
			{
				auto brush = g.CreateSolidColorBrush(Color::FromHexString(pinColor0));
			}

			auto displayText = acc();

			g.DrawTextU(displayText, textFormat, getRect(), brush, 0);

		}
		return gmpi::MP_OK;
	}

	std::string acc()
	{
		if (pinSharpFlat)
		{
			return WStringToUtf8(pinTextb.getValue());
		}
		else
			return WStringToUtf8(pinText.getValue());
	}	
};

namespace
{
	auto r = Register<mxCircleGui>::withId(L"mxCircle");
}

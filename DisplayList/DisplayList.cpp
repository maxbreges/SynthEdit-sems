#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include "..\shared\unicode_conversion.h"
#include "C:\Users\Jeff\Documents\new_module\se_sdk3\SubControlsXp\TextSubcontrol.h"
#include "..\se_sdk3\mp_gui.h"
#include <sstream>
#include <iomanip>
//#include <algorithm>
//#include <string>
//#include <iostream>

using namespace std;
using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;
using namespace JmUnicodeConversions;

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
		// pinTopColor changed
	}

 	void onSetText()
	{
		//pinText = pinListItems;
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
		//----------------------------------
		if (pinColorAdj)
		{
			AnimPosToHex();

			pinHex = pinHexIn;
			pinTopColor = pinHexIn;
		}
		if (!pinColorAdj)
		{
			pinHexIn = pinHex;
			pinTopColor = pinHex;
		}
		//==========================
		invalidateRect();
	}

 	void onSetList()
	{
		//pinText = getDisplayText();	

		it_enum_list it(pinListItems.getValue());
		it.FindValue(pinListIndex);
		int listsize = it.size(); // This would get the size of the list or number of list items total it.size()
		if (it.IsDone())
		{
			pinText = std::string();
		}
		else
		{
			pinText = it.CurrentItem()->text;
		}
		
		listsize_ = listsize-1;
		pinListSize = listsize;
		invalidateRect();		
	}
	 
	void onSetMouseDown()
	{
	}

	void onSetListSize()
	{
	}

	int listsize_ = 0;
	
 	StringGuiPin pinHint;
 	StringGuiPin pinBgColor;
 	StringGuiPin pinTopColor;
 	StringGuiPin pinText;
 	StringGuiPin pinTextColor;
 	StringGuiPin pinFont;
	IntGuiPin pinFontSize;
	FloatGuiPin pinAnimationPosition;
 	StringGuiPin pinHexIn;
 	StringGuiPin pinHex;
	IntGuiPin pinListIndex;
	StringGuiPin pinListItems; 
	IntGuiPin pinListSize;
	BoolGuiPin pinMouseDown;
	BoolGuiPin pinColorAdj;

public:
	DisplayList()
	{
		initializePin( pinHint, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetHint) );
		initializePin( pinBgColor, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetBgColor) );
		initializePin( pinTopColor, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetTopColor) );
		initializePin( pinText, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetText) );
		initializePin( pinTextColor, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetTextColor) );
		initializePin( pinFont, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetTextFont) );
		initializePin(pinFontSize, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetFontSize));
		initializePin(pinAnimationPosition, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetAnimationPosition));
		initializePin( pinHexIn, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetAnimationPosition) );
		initializePin( pinHex, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetAnimationPosition) );
		initializePin(pinListIndex, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetList));
		initializePin( pinListItems, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetList) );		
		initializePin(pinListSize, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetListSize));
		initializePin(pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetMouseDown));		
		initializePin(pinColorAdj, static_cast<MpGuiBaseMemberPtr2>(&DisplayList::onSetAnimationPosition));
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

		pointPrevious = point;	// note first point.
		pinMouseDown = true;
		setCapture();

		return gmpi::MP_OK;
	}

	
	int32_t MP_STDCALL onMouseWheel(int32_t flags, int32_t delta, MP1_POINT point) override
	{
		float new_pos = pinListIndex;
		new_pos = new_pos + delta / 120.0f;
		if (new_pos < 0.f)
			new_pos = 0.f;
		if (new_pos > listsize_)
			new_pos = listsize_;
	
		pinListIndex = new_pos;

		if (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT)
		{
			pinColorAdj = true;
			pinAnimationPosition = float (1.f / listsize_) * pinListIndex;
			onSetAnimationPosition();

		}
		if ((flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT) == 0)
		{
			pinColorAdj = false;
		}


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
		if ((flags & gmpi_gui_api::GG_POINTER_KEY_ALT) == 0)
		{
			pinColorAdj = false;
		}

		if (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT)
		{
			pinColorAdj = true;
		}
		if ((flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT) == 0)
		{
			pinColorAdj = false;
		}

		Point offset(point.x - pointPrevious.x, point.y - pointPrevious.y); // TODO overload subtraction.

		float coarseness = 0.005f;

		//		if (modifier_keys::isHeldCtrl()) // <cntr> key magnifies
		if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL) // <cntr> key magnifies
			coarseness = 0.001f;

		float new_pos = pinAnimationPosition;
		new_pos = new_pos - coarseness * (float)offset.y;

		if (new_pos < 0.f)
			new_pos = 0.f;

		if (new_pos > 1.f)
			new_pos = 1.f;

		pinAnimationPosition = new_pos;
	
		pointPrevious = point;

		invalidateRect();

		return gmpi::MP_OK;
	}

	int32_t onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point)
	{
		if (getCapture())
		{
			releaseCapture();

			GmpiGui::GraphicsHost host(getGuiHost());
			nativeMenu = host.createPlatformMenu(Point(0, 0));
			nativeMenu.SetAlignment(TextAlignment::Leading);

			it_enum_list itr(pinListItems);

			const int popupMenuWrapRowCount = 32;
			int vertical_size = 0; // for collumns on tall menus.
			for (itr.First(); !itr.IsDone(); itr.Next())
			{
				int32_t flags = itr.CurrentItem()->value == pinListIndex ? gmpi_gui::MP_PLATFORM_MENU_TICKED : 0;
				if (vertical_size++ == popupMenuWrapRowCount)
				{
					flags |= gmpi_gui::MP_PLATFORM_MENU_BREAK;
					vertical_size = 1;
				}

				auto& txt = itr.CurrentItem()->text;

				switch (itr.CurrentItem()->getType())
				{
				case enum_entry_type::Separator:
					flags |= gmpi_gui::MP_PLATFORM_MENU_SEPARATOR;
					break;

				case enum_entry_type::Break:
					flags |= gmpi_gui::MP_PLATFORM_MENU_BREAK;
					vertical_size = 1;
					break;

				case enum_entry_type::SubMenu:
					flags = gmpi_gui::MP_PLATFORM_SUB_MENU_BEGIN; // ignore ticked flag.
					txt = txt.substr(4);
					vertical_size = 0; // not quite right, loses count on parent menu.
					break;

				case enum_entry_type::SubMenuEnd:
					flags |= gmpi_gui::MP_PLATFORM_SUB_MENU_END;
					break;

				case enum_entry_type::Normal:
					break;
				}

				nativeMenu.AddItem(txt, itr.CurrentItem()->value, flags);
			}

			nativeMenu.ShowAsync([this](int32_t result) -> void { this->OnPopupComplete(result); });
		}

		if ((flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT) == 0)
		{
			pinColorAdj = false;
		}

		pinColorAdj = false;
		pinMouseDown = false;

		return gmpi::MP_OK;
		//invalidateRect();
	}

	int32_t AnimPosToHex()
	{
		int x = pinAnimationPosition * 1535;
		int R = 0;
		int G = 0;
		int B = 0;
		//--------------------------------

		if ((x >= 255) && (x < 768))
			G = 255;
		if (x > 1023)
			G = 0;

		if ((x >= 1279) || (x < 255))
			R = 255;
		if ((x > 510) && (x <= 1024))
			R = 0;

		if ((x >= 767) && (x < 1280))
			B = 255;
		if (x <= 511)
			B = 0;

		//-----------------------------

		if ((x >= 0) && (x <= 255))
		{
			G = x;
		}
		//-------------------

		if ((x >= 256) && (x <= 511))
		{
			R = 255 - (x - 256);
		}

		//-------------------
		if ((x >= 512) && (x <= 767))
		{
			B = x - 512;
		}

		//-------------------
		if ((x >= 768) && (x <= 1023))
		{
			G = 255 - (x - 768);
		}

		//-------------------

		if ((x >= 1024) && (x <= 1279))
		{
			R = x - 1024;
		}

		//-------------------
		if ((x >= 1280) && (x <= 1535))
		{
			B = 255 - (x - 1280);
		}


		std::string resA = "ff";

		std::stringstream ssR;
		ssR << std::setfill('0') << std::setw(sizeof(int) - 2) << std::hex << R;
		std::string resR(ssR.str());

		std::stringstream ssG;
		ssG << std::setfill('0') << std::setw(sizeof(int) - 2) << std::hex << G;
		std::string resG(ssG.str());

		std::stringstream ssB;
		ssB << std::setfill('0') << std::setw(sizeof(int) - 2) << std::hex << B;
		std::string resB(ssB.str());

		pinHexIn = resA + resR + resG + resB;

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

		auto brush = g.CreateSolidColorBrush(Color::FromHexString(pinTextColor));

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

		//=============================================================
		std::string str = { pinFont };
		const char* fontFace = str.c_str();
		TextFormat tf = g.GetFactory().CreateTextFormat(pinFontSize, fontFace);

		tf.SetParagraphAlignment(ParagraphAlignment::Center),

			tf.SetTextAlignment(TextAlignment::Center);

		brush.SetColor(Color::FromHexString(pinTextColor));
		g.DrawTextW(pinText.getValue(), tf, getRect(), brush);

		return gmpi::MP_OK;
	}
	//====================================

	void OnPopupComplete(int32_t result)
	{
		if (result == gmpi::MP_OK)
		{
#if 1
			if (nativeMenu.GetSelectedId() == pinListIndex)
			{
				pinListIndex = -1;
			}
#endif
			pinListIndex = nativeMenu.GetSelectedId();

		}

		nativeMenu.setNull(); // release it.
	}

};

namespace
{
	auto r = Register<DisplayList>::withId(L"DisplayList");
}

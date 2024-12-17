#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include "mp_gui.h"

using namespace gmpi;
using namespace GmpiDrawing;

GmpiDrawing_API::MP1_POINT pointPrevious;
GmpiGui::PopupMenu nativeMenu;

class PopUpGui final : public gmpi_gui::MpGuiGfxBase
{
 	void onSetHint()
	{
		// pinHint changed
	}

 	void onSetBgColor()
	{
		// pinBgColor changed
	}

 	void onSetColor()
	{
		// pinColor changed
	}

 	void onSetText()
	{
		// pinText changed
	}

 	void onSetTextColor()
	{
		// pinTextColor changed
	}

 	void onSetTextFont()
	{
		// pinTextFont changed
	}

 	void onSetFontSize()
	{
		// pinFontSize changed
	}

 	void onSetAnimationPosition()
	{
		// pinAnimationPosition changed
	}

 	void onSetHexOut()
	{
		// pinHexOut changed
	}

 	void onSetHexMem()
	{
		// pinHexMem changed
	}

 	void onSetChoice()
	{
		invalidateRect();
	}

 	void onSetListItems()
	{
		invalidateRect();
	}

	int listsize_ = 0;
 	void onSetListSize()
	{
		it_enum_list it(pinListItems.getValue());
		it.FindValue(pinChoice);
		int listsize = it.size(); // This would get the size of the list or number of list items total it.size()
		if (it.IsDone())
		{
			pinText = std::string();
		}
		else
		{
			pinText = it.CurrentItem()->text;
		}

		listsize_ = listsize - 1;
		pinListSize = listsize;
		invalidateRect();
	}

 	void onSetMouseDown()
	{
		// pinMouseDown changed
	}

 	void onSetShiftDrag()
	{
		// pinShiftDrag changed
	}

 	void onSetCornerRadius()
	{
		invalidateRect();
	}

 	StringGuiPin pinHint;
 	StringGuiPin pinBgColor;
 	StringGuiPin pinColor;
 	StringGuiPin pinText;
 	StringGuiPin pinTextColor;
 	StringGuiPin pinTextFont;
 	IntGuiPin pinFontSize;
 	FloatGuiPin pinAnimationPosition;
 	StringGuiPin pinHexOut;
 	StringGuiPin pinHexMem;
 	IntGuiPin pinChoice;
 	StringGuiPin pinListItems;
 	IntGuiPin pinListSize;
 	BoolGuiPin pinMouseDown;
 	BoolGuiPin pinShiftDrag;
 	IntGuiPin pinCornerRadius;

public:
	PopUpGui()
	{
		initializePin( pinHint, static_cast<MpGuiBaseMemberPtr2>(&PopUpGui::onSetHint) );
		initializePin( pinBgColor, static_cast<MpGuiBaseMemberPtr2>(&PopUpGui::onSetBgColor) );
		initializePin( pinColor, static_cast<MpGuiBaseMemberPtr2>(&PopUpGui::onSetColor) );
		initializePin( pinText, static_cast<MpGuiBaseMemberPtr2>(&PopUpGui::onSetText) );
		initializePin( pinTextColor, static_cast<MpGuiBaseMemberPtr2>(&PopUpGui::onSetTextColor) );
		initializePin( pinTextFont, static_cast<MpGuiBaseMemberPtr2>(&PopUpGui::onSetTextFont) );
		initializePin( pinFontSize, static_cast<MpGuiBaseMemberPtr2>(&PopUpGui::onSetFontSize) );
		initializePin( pinAnimationPosition, static_cast<MpGuiBaseMemberPtr2>(&PopUpGui::onSetAnimationPosition) );
		initializePin( pinHexOut, static_cast<MpGuiBaseMemberPtr2>(&PopUpGui::onSetHexOut) );
		initializePin( pinHexMem, static_cast<MpGuiBaseMemberPtr2>(&PopUpGui::onSetHexMem) );
		initializePin( pinChoice, static_cast<MpGuiBaseMemberPtr2>(&PopUpGui::onSetChoice) );
		initializePin( pinListItems, static_cast<MpGuiBaseMemberPtr2>(&PopUpGui::onSetListItems) );
		initializePin( pinListSize, static_cast<MpGuiBaseMemberPtr2>(&PopUpGui::onSetListSize) );
		initializePin( pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&PopUpGui::onSetMouseDown) );
		initializePin( pinShiftDrag, static_cast<MpGuiBaseMemberPtr2>(&PopUpGui::onSetShiftDrag) );
		initializePin( pinCornerRadius, static_cast<MpGuiBaseMemberPtr2>(&PopUpGui::onSetCornerRadius) );
	}

	int32_t MP_STDCALL onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{
		// Let host handle right-clicks.
		if ((flags & 0x10) == 0)
		{
			return gmpi::MP_OK; // Indicate successful hit, so right-click menu can show.
		}

		pointPrevious = point;	// note first point.
		pinMouseDown = true;
		setCapture();

		return gmpi::MP_OK;
	}

	int32_t onPointerUp(int32_t flags, struct GmpiDrawing_API::MP1_POINT point)
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
				int32_t flags = itr.CurrentItem()->value == pinChoice ? gmpi_gui::MP_PLATFORM_MENU_TICKED : 0;
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

		return gmpi::MP_OK;
	}

	void OnPopupComplete(int32_t result)
	{
		if (result == gmpi::MP_OK)
		{
#if 1
			if (nativeMenu.GetSelectedId() == pinChoice)
			{
				pinChoice = -1;
			}
#endif
			pinChoice = nativeMenu.GetSelectedId();

		}

		nativeMenu.setNull(); // release it.
	}

	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext ) override
	{
		Graphics g(drawingContext);

		auto textFormat = GetGraphicsFactory().CreateTextFormat();
		auto brush = g.CreateSolidColorBrush(Color::Red);

		g.DrawTextU("REDPOPA", textFormat, 0.0f, 0.0f, brush);

		return gmpi::MP_OK;
	}
};

namespace
{
	auto r = Register<PopUpGui>::withId(L"Pop-Up");
}

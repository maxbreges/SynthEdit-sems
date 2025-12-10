#include "PopUpMenuGui.h"
#include "Drawing.h"
#include "unicode_conversion.h"

using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;
using namespace JmUnicodeConversions;

GMPI_REGISTER_GUI(MP_SUB_TYPE_GUI2, PopupMenuGui, L"PopUpMenu");
SE_DECLARE_INIT_STATIC_FILE(PopUpMenu);

PopupMenuGui::PopupMenuGui()
{
	// initialise pins.
	initializePin(pinChoice, static_cast<MpGuiBaseMemberPtr2>(&PopupMenuGui::onSetChoice));
	initializePin(pinItemList, static_cast<MpGuiBaseMemberPtr2>(&PopupMenuGui::redraw));
	initializePin(pinStyle, static_cast<MpGuiBaseMemberPtr2>(&PopupMenuGui::onSetStyle));
	initializePin(pinWriteable);
	initializePin(pinGreyed, static_cast<MpGuiBaseMemberPtr2>(&PopupMenuGui::redraw));
	initializePin(pinHint);
	initializePin(pinMenuItems);
	initializePin(pinMenuSelection);
	initializePin(pinHeading, static_cast<MpGuiBaseMemberPtr2>(&PopupMenuGui::redraw));
	initializePin(pinMomentary);
	initializePin(pinEnableSpecialStrings);
	initializePin(pinPopUpOpen);
	initializePin(pinOpenExt, static_cast<MpGuiBaseMemberPtr2>(&PopupMenuGui::onSetExt));
	initializePin(pinSelection, static_cast<MpGuiBaseMemberPtr2>(&PopupMenuGui::redraw));
	initializePin(pinMouseDown);
}
bool onSetExtFlag = false;

void PopupMenuGui::onSetExt()
{
	if (pinPopUpOpen) return; // Prevent re-entrant opening

	if (pinOpenExt)
	{
		onSetExtFlag = true;
	}

	if (!pinOpenExt && onSetExtFlag == true)
	{
		
		int32_t flags = GG_POINTER_FLAG_FIRSTBUTTON;
		GmpiDrawing_API::MP1_POINT point = { 0, 0 };

		setCapture();
		onPointerUp(flags, point);
		onSetExtFlag = false;
	}
}

void PopupMenuGui::onSetChoice()
{
	it_enum_list itr(pinItemList);
	for (itr.First(); !itr.IsDone(); itr.Next())
	{
		if (itr.CurrentItem()->value == pinChoice.getValue())
		{
			pinSelection = itr.CurrentItem()->text;
			//	found = true;
			break; // Exit loop once found
		}
	}
}

void PopupMenuGui::onSetSelection()
{
	pinSelection = pinSelection.getValue();
//	bool found = false;
	it_enum_list itr(pinItemList);
	for (itr.First(); !itr.IsDone(); itr.Next())
	{
		if (itr.CurrentItem()->value == pinChoice.getValue())
		{
			pinSelection = itr.CurrentItem()->text;
		//	found = true;
			break; // Exit loop once found
		}
	}

	// Optional: if no match found, clear selection or set default
//	if (!found)
	//	pinSelection = pinSelection; // or some default value
}

int32_t MP_STDCALL PopupMenuGui::onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point)
{
	// Let host handle right-clicks.
	if ((flags & GG_POINTER_FLAG_FIRSTBUTTON) == 0)
	{
		return gmpi::MP_OK; // Indicate successful hit, so right-click menu can show.
	}

	if (pinWriteable == false)
		return gmpi::MP_OK; // Indicate successful hit.
	pinMouseDown = true;
	setCapture(); // prevent mouse-up going to menu (and dismissing it).

	return gmpi::MP_OK;
}

int32_t MP_STDCALL PopupMenuGui::onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point)
{
	pinMouseDown = false;

	if (getCapture())
	{
		releaseCapture();

		GmpiGui::GraphicsHost host(getGuiHost());
		nativeMenu = host.createPlatformMenu(Point(0, 0));
		nativeMenu.SetAlignment(TextAlignment::Leading);

		it_enum_list itr(pinItemList);

		const int popupMenuWrapRowCount = 32;
		int vertical_size = 0; // for collumns on tall menus.
		for (itr.First(); !itr.IsDone(); itr.Next())
		{
			int32_t flags = !pinMomentary.getValue() && itr.CurrentItem()->value == pinChoice.getValue() ? gmpi_gui::MP_PLATFORM_MENU_TICKED : 0;
			if (vertical_size++ == popupMenuWrapRowCount)
			{
				flags |= gmpi_gui::MP_PLATFORM_MENU_BREAK;
				vertical_size = 1;
			}

			auto& txt = itr.CurrentItem()->text;
			//pinHeading = txt;
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
		pinPopUpOpen = true;
		nativeMenu.ShowAsync([this](int32_t result) -> void { this->OnPopupComplete(result); });		
	}
	return gmpi::MP_OK;
}

std::string PopupMenuGui::getDisplayText()
{
	return WStringToUtf8(pinHeading.getValue());
}

void PopupMenuGui::OnPopupComplete(int32_t result)
{	
	if (result == gmpi::MP_OK)
	{
		// Handle selection
		if (pinMomentary.getValue() && nativeMenu.GetSelectedId() == pinChoice.getValue())
		{
			pinChoice = -1;
		}
		else
		{
			pinChoice = nativeMenu.GetSelectedId();
		}
	}
	releaseCapture();
	pinPopUpOpen = false;
	nativeMenu.setNull();
	pinOpenExt = false;
	onSetSelection();
}
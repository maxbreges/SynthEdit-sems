#include "mp_sdk_gui2.h"

using namespace gmpi;

class GuiTextToTextGui final : public SeGuiInvisibleBase
{
 	void onSetValueIn()
	{
		// pinValueIn changed
	}

 	void onSetNameIn()
	{
		// pinNameIn changed
	}

 	void onSetFileExtension()
	{
		// pinFileExtension changed
	}

 	void onSetMenuItems()
	{
		// pinMenuItems changed
	}

 	void onSetMenuSelection()
	{
		// pinMenuSelection changed
	}

 	void onSetName()
	{
		// pinName changed
	}

 	void onSetValue()
	{
		pinValueIn = pinValue;
	}

 	void onSetFileExtensionOut()
	{
		// pinFileExtensionOut changed
	}

 	void onSetMenuItemsOut()
	{
		// pinMenuItemsOut changed
	}

 	void onSetMenuSelectionOut()
	{
		// pinMenuSelectionOut changed
	}

 	StringGuiPin pinValueIn;
 	StringGuiPin pinNameIn;
 	StringGuiPin pinFileExtension;
 	StringGuiPin pinMenuItems;
 	IntGuiPin pinMenuSelection;
 	StringGuiPin pinValue;

public:
	GuiTextToTextGui()
	{
		initializePin( pinValueIn, static_cast<MpGuiBaseMemberPtr2>(&GuiTextToTextGui::onSetValueIn) );
		initializePin( pinNameIn, static_cast<MpGuiBaseMemberPtr2>(&GuiTextToTextGui::onSetNameIn) );
		initializePin( pinFileExtension, static_cast<MpGuiBaseMemberPtr2>(&GuiTextToTextGui::onSetFileExtension) );
		initializePin( pinMenuItems, static_cast<MpGuiBaseMemberPtr2>(&GuiTextToTextGui::onSetMenuItems) );
		initializePin( pinMenuSelection, static_cast<MpGuiBaseMemberPtr2>(&GuiTextToTextGui::onSetMenuSelection) );
		
		initializePin( pinValue, static_cast<MpGuiBaseMemberPtr2>(&GuiTextToTextGui::onSetValue) );
	}

};

namespace
{
	auto r = Register<GuiTextToTextGui>::withId(L"GuiTextToText");
}

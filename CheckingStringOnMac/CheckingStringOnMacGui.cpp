#include "mp_sdk_gui2.h"

using namespace gmpi;

class CheckingStringOnMacGui final : public SeGuiInvisibleBase
{
 	void onSetFileNameIn()
	{
		pinDebug = pinFileNameIn;
	}

 	void onSetItemList()
	{
		// pinItemList changed
	}

 	void onSetDebug()
	{
		// pinDebug changed
	}

 	StringGuiPin pinFileNameIn;
 	StringGuiPin pinItemList;
 	StringGuiPin pinDebug;

public:
	CheckingStringOnMacGui()
	{
		initializePin( pinFileNameIn, static_cast<MpGuiBaseMemberPtr2>(&CheckingStringOnMacGui::onSetFileNameIn) );
		initializePin( pinItemList, static_cast<MpGuiBaseMemberPtr2>(&CheckingStringOnMacGui::onSetItemList) );
		initializePin( pinDebug, static_cast<MpGuiBaseMemberPtr2>(&CheckingStringOnMacGui::onSetDebug) );
	}

};

namespace
{
	auto r = Register<CheckingStringOnMacGui>::withId(L"My CheckingStringOnMac");
}

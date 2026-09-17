#include "mp_sdk_gui2.h"
#include "it_enum_list.h"

using namespace gmpi;

class Og_RandomListGui final : public SeGuiInvisibleBase
{
	void onSetBool()
	{
		onSetItemList();
	}

 	void onSetItemList()
	{
		it_enum_list it(pinItemList);

		int listsize = it.size(); // This would get the size of the list or number of list items total it.size()

		pinListSize = listsize; //to make sure the list size is correct

		if (pinBool)
		{
			pinChoice = rand() % listsize;
		}

	}

	BoolGuiPin pinBool;
	IntGuiPin pinChoice;
 	StringGuiPin pinItemList;
	IntGuiPin pinListSize;

public:
	Og_RandomListGui()
	{
		initializePin(pinBool, static_cast<MpGuiBaseMemberPtr2>(&Og_RandomListGui::onSetBool));
		initializePin(pinChoice);
		initializePin( pinItemList, static_cast<MpGuiBaseMemberPtr2>(&Og_RandomListGui::onSetItemList) );
		initializePin(pinListSize);
	}
};

namespace
{
	auto r = Register<Og_RandomListGui>::withId(L"My Og_RandomList");
}

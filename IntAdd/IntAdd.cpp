#include "mp_sdk_gui2.h"

using namespace gmpi;

class IntAddGui final : public SeGuiInvisibleBase
{
 	void onSetValueIn()
	{
		pinValueOut = pinAdd + pinValueIn;
	}

 	void onSetAdd()
	{
		pinValueOut = pinAdd + pinValueIn;
	}

 	void onSetValueOut()
	{
		// pinValueOut changed
	}

 	IntGuiPin pinValueIn;
 	IntGuiPin pinAdd;
 	IntGuiPin pinValueOut;

public:
	IntAddGui()
	{
		initializePin( pinValueIn, static_cast<MpGuiBaseMemberPtr2>(&IntAddGui::onSetValueIn) );
		initializePin( pinAdd, static_cast<MpGuiBaseMemberPtr2>(&IntAddGui::onSetAdd) );
		initializePin( pinValueOut, static_cast<MpGuiBaseMemberPtr2>(&IntAddGui::onSetValueOut) );
	}

};

namespace
{
	auto r = Register<IntAddGui>::withId(L"IntAdd");
}

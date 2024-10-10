#include "mp_sdk_gui2.h"

using namespace gmpi;

class IntMultiplyGui final : public SeGuiInvisibleBase
{
 	void onSetValueIn()
	{
		pinValueOut = pinValueIn * pinMultiply;
	}

 	void onSetMultiply()
	{
		pinValueOut = pinValueIn * pinMultiply;
	}

 	void onSetValueOut()
	{
		// pinValueOut changed
	}

 	IntGuiPin pinValueIn;
 	IntGuiPin pinMultiply;
 	IntGuiPin pinValueOut;

public:
	IntMultiplyGui()
	{
		initializePin( pinValueIn, static_cast<MpGuiBaseMemberPtr2>(&IntMultiplyGui::onSetValueIn) );
		initializePin(pinMultiply, static_cast<MpGuiBaseMemberPtr2>(&IntMultiplyGui::onSetMultiply) );
		initializePin( pinValueOut, static_cast<MpGuiBaseMemberPtr2>(&IntMultiplyGui::onSetValueOut) );
	}

};

namespace
{
	auto r = Register<IntMultiplyGui>::withId(L"IntMultiply");
}

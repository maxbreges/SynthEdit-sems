#include "mp_sdk_gui2.h"

using namespace gmpi;

class FloatSubtractGui final : public SeGuiInvisibleBase
{
 	void onSetValueIn()
	{
		pinValueOut = pinValueIn - pinSubtract;
	}

 	void onSetSubtract()
	{
		pinValueOut = pinValueIn - pinSubtract;
	}

 	void onSetValueOut()
	{
		// pinValueOut changed
	}

 	FloatGuiPin pinValueIn;
 	FloatGuiPin pinSubtract;
 	FloatGuiPin pinValueOut;

public:
	FloatSubtractGui()
	{
		initializePin( pinValueIn, static_cast<MpGuiBaseMemberPtr2>(&FloatSubtractGui::onSetValueIn) );
		initializePin(pinSubtract, static_cast<MpGuiBaseMemberPtr2>(&FloatSubtractGui::onSetSubtract) );
		initializePin( pinValueOut, static_cast<MpGuiBaseMemberPtr2>(&FloatSubtractGui::onSetValueOut) );
	}

};

namespace
{
	auto r = Register<FloatSubtractGui>::withId(L"FloatSubtract");
}

#include "mp_sdk_gui2.h"

using namespace gmpi;

class IntSubtractGui final : public SeGuiInvisibleBase
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

	IntGuiPin pinValueIn;
	IntGuiPin pinSubtract;
	IntGuiPin pinValueOut;

public:
	IntSubtractGui()
	{
		initializePin(pinValueIn, static_cast<MpGuiBaseMemberPtr2>(&IntSubtractGui::onSetValueIn));
		initializePin(pinSubtract, static_cast<MpGuiBaseMemberPtr2>(&IntSubtractGui::onSetSubtract));
		initializePin(pinValueOut, static_cast<MpGuiBaseMemberPtr2>(&IntSubtractGui::onSetValueOut));
	}

};

namespace
{
	auto r = Register<IntSubtractGui>::withId(L"IntSubtract");
}

#include "mp_sdk_gui2.h"

using namespace gmpi;

class FloatHoldGui final : public SeGuiInvisibleBase
{
 	void onSetValueIn()
	{
		// pinValueIn changed
	}

 	void onSetHold()
	{
		if (pinHold)
		{
			pinFloatHold = pinValueIn;
		}
	
	}

 	void onSetValueOut()
	{
		pinValueOut = pinFloatHold;
	}

 	FloatGuiPin pinValueIn;
 	BoolGuiPin pinHold;
 	FloatGuiPin pinValueOut;
	FloatGuiPin pinFloatHold;

public:
	FloatHoldGui()
	{
		initializePin( pinValueIn, static_cast<MpGuiBaseMemberPtr2>(&FloatHoldGui::onSetValueIn) );
		initializePin( pinHold, static_cast<MpGuiBaseMemberPtr2>(&FloatHoldGui::onSetHold) );
		initializePin( pinValueOut, static_cast<MpGuiBaseMemberPtr2>(&FloatHoldGui::onSetValueOut) );
		initializePin(pinFloatHold, static_cast<MpGuiBaseMemberPtr2>(&FloatHoldGui::onSetValueOut));
	}

};

namespace
{
	auto r = Register<FloatHoldGui>::withId(L"FloatHold");
}

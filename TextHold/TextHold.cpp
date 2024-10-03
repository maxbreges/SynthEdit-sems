#include "mp_sdk_gui2.h"

using namespace gmpi;

class TextHoldGui final : public SeGuiInvisibleBase
{
 	void onSetValueIn()
	{
		// pinValueIn changed
	}

 	void onSetHold()
	{
		if (pinHold)
		{
			pinHoldString = pinValueIn;
		}
		/*if (!pinHold)
		{
			pinValueOut = pinValueOut;
		}*/

	}

 	void onSetValueOut()
	{
		pinValueOut = pinHoldString;
	}

 	StringGuiPin pinValueIn;
 	BoolGuiPin pinHold;
 	StringGuiPin pinValueOut;
	StringGuiPin pinHoldString;

public:
	TextHoldGui()
	{
		initializePin( pinValueIn, static_cast<MpGuiBaseMemberPtr2>(&TextHoldGui::onSetValueIn) );
		initializePin( pinHold, static_cast<MpGuiBaseMemberPtr2>(&TextHoldGui::onSetHold) );
		initializePin( pinValueOut, static_cast<MpGuiBaseMemberPtr2>(&TextHoldGui::onSetValueOut) );
		initializePin(pinHoldString, static_cast<MpGuiBaseMemberPtr2>(&TextHoldGui::onSetValueOut));

	}

};

namespace
{
	auto r = Register<TextHoldGui>::withId(L"TextHold");
}

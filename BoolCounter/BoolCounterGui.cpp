#include "mp_sdk_gui2.h"

using namespace gmpi;

class BoolCounterGui final : public SeGuiInvisibleBase
{
	int val = 0;

 	void onSetIntVal()
	{
		// pinIntVal changed
	}

 	void onSetBoolVal()
	{
		if (pinBoolVal)
		{
			val++;
			pinIntVal = val;
		}
		else
		{			
		}
	}

	void onSetReset()
	{
		val = 0;
		pinIntVal = val;
	}

	BoolGuiPin pinBoolVal;
 	IntGuiPin pinIntVal;
	BoolGuiPin pinReset;

public:
	BoolCounterGui()
	{
		initializePin(pinBoolVal, static_cast<MpGuiBaseMemberPtr2>(&BoolCounterGui::onSetBoolVal));
		initializePin( pinIntVal, static_cast<MpGuiBaseMemberPtr2>(&BoolCounterGui::onSetIntVal) );
		initializePin(pinReset, static_cast<MpGuiBaseMemberPtr2>(&BoolCounterGui::onSetReset));
		
	}

};

namespace
{
	auto r = Register<BoolCounterGui>::withId(L"My BoolCounter");
}

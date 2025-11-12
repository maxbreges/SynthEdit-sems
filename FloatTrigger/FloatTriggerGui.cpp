#include "mp_sdk_gui2.h"

using namespace gmpi;

class FloatTriggerGui final : public SeGuiInvisibleBase
{
	float previousVal = 0.f;

 	void onSetFloatVal()
	{
		if (pinFloatVal != previousVal)
		{
			pinBoolVal = true;
		}
		previousVal = pinFloatVal;

		pinBoolVal = false;
	}

 	void onSetBoolVal()
	{
		// pinBoolVal changed
	}

 	FloatGuiPin pinFloatVal;
 	BoolGuiPin pinBoolVal;

public:
	FloatTriggerGui()
	{
		initializePin( pinFloatVal, static_cast<MpGuiBaseMemberPtr2>(&FloatTriggerGui::onSetFloatVal) );
		initializePin( pinBoolVal, static_cast<MpGuiBaseMemberPtr2>(&FloatTriggerGui::onSetBoolVal) );
	}

};

namespace
{
	auto r = Register<FloatTriggerGui>::withId(L"My FloatTrigger");
}

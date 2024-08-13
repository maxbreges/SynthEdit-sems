#include "mp_sdk_gui2.h"

using namespace gmpi;

class FloatCompareGui final : public SeGuiInvisibleBase
{


 	void onSetA()
	{
		if (pinA >= pinB)
			pinBoolOut = true;
		if (pinA <= pinB)
			pinBoolOut = false;
	}

 	void onSetB()
	{
		/*if (pinB < pinA)
			pinBoolOut = true;
		if (pinB > pinA)
			pinBoolOut = false;*/
	}

 	void onSetBoolOut()
	{
		// pinValueOut changed
	}

 	
 	FloatGuiPin pinA;
 	FloatGuiPin pinB;
 	FloatGuiPin pinBoolOut;

public:
	FloatCompareGui()
	{
		initializePin( pinA, static_cast<MpGuiBaseMemberPtr2>(&FloatCompareGui::onSetA) );
		initializePin( pinB, static_cast<MpGuiBaseMemberPtr2>(&FloatCompareGui::onSetB) );
		initializePin( pinBoolOut, static_cast<MpGuiBaseMemberPtr2>(&FloatCompareGui::onSetBoolOut) );
	}

};

namespace
{
	auto r = Register<FloatCompareGui>::withId(L"FloatCompare");
}

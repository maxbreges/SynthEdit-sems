#include "mp_sdk_gui2.h"

using namespace gmpi;

class BoolANDGui final : public SeGuiInvisibleBase
{
 	void onSetBool1()
	{
		onSetBoolOut();
	}

 	void onSetBool2()
	{
		onSetBoolOut();
	}

 	void onSetBoolOut()
	{
		if (pinBool1 && pinBool2)
		{
			pinBoolOut = true;
		}
		else

			pinBoolOut = false;
	
	}

 	BoolGuiPin pinBool1;
 	BoolGuiPin pinBool2;
 	BoolGuiPin pinBoolOut;

public:
	BoolANDGui()
	{
		initializePin(pinBool1, static_cast<MpGuiBaseMemberPtr2>(&BoolANDGui::onSetBool1) );
		initializePin(pinBool2, static_cast<MpGuiBaseMemberPtr2>(&BoolANDGui::onSetBool2) );
		initializePin( pinBoolOut, static_cast<MpGuiBaseMemberPtr2>(&BoolANDGui::onSetBoolOut) );
	}

};

namespace
{
	auto r = Register<BoolANDGui>::withId(L"BoolAND");
}

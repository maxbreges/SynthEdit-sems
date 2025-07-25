#include "mp_sdk_gui2.h"

using namespace gmpi;

class GUIBoolGateVoltsGui final : public SeGuiInvisibleBase
{
 	void onSetToDSP()
	{
		// pinToDSP changed
	}

 	void onSetL_In()
	{
		pinToDSP = pinL_In;
		pinR_In = pinL_In;
	}

 	void onSetR_In()
	{
		pinToDSP = pinR_In;
		pinL_In = pinR_In;
	}

 	BoolGuiPin pinToDSP;
 	BoolGuiPin pinL_In;
 	BoolGuiPin pinR_In;

public:
	GUIBoolGateVoltsGui()
	{
		initializePin( pinToDSP, static_cast<MpGuiBaseMemberPtr2>(&GUIBoolGateVoltsGui::onSetToDSP) );
		initializePin( pinL_In, static_cast<MpGuiBaseMemberPtr2>(&GUIBoolGateVoltsGui::onSetL_In) );
		initializePin( pinR_In, static_cast<MpGuiBaseMemberPtr2>(&GUIBoolGateVoltsGui::onSetR_In) );
	}

};

namespace
{
	auto r = Register<GUIBoolGateVoltsGui>::withId(L"GUIBoolGateVolts");
}

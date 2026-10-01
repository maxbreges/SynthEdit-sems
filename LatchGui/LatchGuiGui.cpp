#include "mp_sdk_gui2.h"

using namespace gmpi;

class LatchGuiGui final : public SeGuiInvisibleBase
{
 	void onSetInput()
	{
		if (pinInput||pinInputR)
		{
			pinOutput = !pinOutput;
		}
	}

	BoolGuiPin pinInput;
 	BoolGuiPin pinInputR;
 	BoolGuiPin pinOutput;

public:
	LatchGuiGui()
	{
		initializePin(pinInput, static_cast<MpGuiBaseMemberPtr2>(&LatchGuiGui::onSetInput));
		initializePin( pinInputR, static_cast<MpGuiBaseMemberPtr2>(&LatchGuiGui::onSetInput) );
		initializePin( pinOutput);
	}
};

namespace
{
	auto r = Register<LatchGuiGui>::withId(L"LatchGui");
}

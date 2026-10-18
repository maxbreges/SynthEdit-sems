#include "mp_sdk_gui2.h"

using namespace gmpi;

class onStringChangeGui final : public SeGuiInvisibleBase
{
	void onSetBool()
	{
		if (pinBool)
		{
			getHost()->sendMessageToAudio(2232325, sizeof(bool), &pinBool);
		}
	}

	BoolGuiPin pinBool;

public:
	onStringChangeGui()
	{
		initializePin(pinBool, static_cast<MpGuiBaseMemberPtr2>(&onStringChangeGui::onSetBool));
	}
};

namespace
{
	auto r = Register<onStringChangeGui>::withId(L"My onStringChangePulse");
}
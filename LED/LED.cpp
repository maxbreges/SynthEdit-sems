#include "mp_sdk_audio.h"

using namespace gmpi;

class LED final : public MpBase2
{
	BoolInPin pinSignalin;
	FloatOutPin pinpatchValue;

public:
	LED()
	{
		initializePin(pinSignalin);
		initializePin(pinpatchValue);
	}


	void onSetPins() override
	{
		// Check which pins are updated.
		if (pinSignalin.isUpdated())
		{
			pinpatchValue = pinSignalin;
		}

	}
};

namespace
{
	auto r = Register<LED>::withId(L"LED");
}

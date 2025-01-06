#include "mp_sdk_audio.h"

using namespace gmpi;

class FretLed final : public MpBase2
{
	BoolInPin pinGateIn;
	BoolOutPin pinGateOut;

public:
	FretLed()
	{
		initializePin(pinGateIn);
		initializePin(pinGateOut);
	}

	void onSetPins() override
	{
		// Check which pins are updated.
		if (pinGateIn.isUpdated())
		{
			pinGateOut = pinGateIn;
		}
	}
};

namespace
{
	auto r = Register<FretLed>::withId(L"FretLed");
}

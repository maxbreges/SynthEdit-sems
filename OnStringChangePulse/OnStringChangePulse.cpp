#include "mp_sdk_audio.h"

using namespace gmpi;

class OnStringChangePulse final : public MpBase2
{
	bool isEnabled = false;
	int pulseLength = 45;

	StringInPin pinString;
	BoolOutPin pinBool;
	IntInPin pinPulseLength;

public:
	OnStringChangePulse()
	{
		initializePin( pinString );
		initializePin( pinBool );
		initializePin( pinPulseLength );
	}

	int32_t receiveMessageFromGui(int32_t id, int32_t size, void* msg)
	{
		if (id == 2232325)
		{
			isEnabled = true;
		}

		return gmpi::MP_OK;
	}

	void onSetPins() override
	{
		pulseLength = pinPulseLength.getValue();

		if (isEnabled)
		{
			if (pinString.isUpdated())
			{
				pinBool.setValue(true, getBlockPosition());
				pinBool.setValue(false, getBlockPosition() + pulseLength);
				isEnabled = false;
			}			
		}
	}
};

namespace
{
	auto r = Register<OnStringChangePulse>::withId(L"My onStringChangePulse");
}

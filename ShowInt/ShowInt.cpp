#include "mp_sdk_audio.h"

using namespace gmpi;

class VoltMeter final : public MpBase2
{
	IntInPin pinSignalin;
	IntOutPin pinpatchValue;

public:
	VoltMeter()
	{
		initializePin( pinSignalin );
		initializePin( pinpatchValue );
	}


	void onSetPins() override
	{
		// Check which pins are updated.
		if( pinSignalin.isUpdated() )
		{
			pinpatchValue = pinSignalin;
		}
	}
};

namespace
{
	auto r = Register<VoltMeter>::withId(L"ShowInt");
}

#include "mp_sdk_audio.h"

using namespace gmpi;

class VoltMeter final : public MpBase2
{
	FloatInPin pinSignalin;
	FloatOutPin pinpatchValue;

public:
	VoltMeter()
	{
		initializePin( pinSignalin );
		initializePin( pinpatchValue );
	}

	/*void subProcess(int sampleFrames)
	{

		float* in = getBuffer(pinSignalin);

		for (int s = 0; s < sampleFrames; s++) // Counts upward
		{
			pinpatchValue.setValue(*in * 10.f, getBlockPosition() + s); // Use getBlockPosition() function

			++in; // Shorter version of pointer increment, add ++ to last instance of each pointer
		}

	}*/

	void onSetPins() override
	{
		// Check which pins are updated.
		if( pinSignalin.isUpdated() )
		{
			pinpatchValue = pinSignalin;
		}


		// Set processing method.
		//setSubProcess(&VoltMeter::subProcess);
	}
};

namespace
{
	auto r = Register<VoltMeter>::withId(L"ShowFloat");
}

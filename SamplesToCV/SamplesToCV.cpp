#include "mp_sdk_audio.h"
#include <cmath> 

using namespace gmpi;

class SamplesToCV final : public MpBase2
{
	AudioInPin pinNumberOfSamples;
	AudioOutPin pinVoltsOut;

public:
	SamplesToCV()
	{
		initializePin( pinNumberOfSamples );
		initializePin( pinVoltsOut );
	}

	void subProcess( int sampleFrames )
	{
		// get pointers to in/output buffers.
		auto onoff = getBuffer(pinNumberOfSamples);
		auto output = getBuffer(pinVoltsOut);

		for (int s = sampleFrames; s > 0; --s)
		{
			float sampleCount = *onoff; // current sample count
			float voltage = 0.0f;

			if (sampleCount > 0.0f)
			{
				// Calculate voltage based on your formula
				voltage = std::log2(1600.0f / sampleCount) * 0.1f - 0.33219281324f;
			}
			else
			{
				voltage = 0.0f;
			}

			*output = voltage;

			++onoff;
			++output;
		}
	}

	void onSetPins() override
	{
		// Check which pins are updated.
		if( pinNumberOfSamples.isStreaming() )
		{
		}

		// Set state of output audio pins.
		pinVoltsOut.setStreaming(true);

		// Set processing method.
		setSubProcess(&SamplesToCV::subProcess);
	}
};

namespace
{
	auto r = Register<SamplesToCV>::withId(L"SamplesToCV");
}

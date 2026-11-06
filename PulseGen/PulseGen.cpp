#include "mp_sdk_audio.h"

using namespace gmpi;

class PulseGen final : public MpBase2
{
	BoolInPin pinBool;
	AudioInPin pinRateHz;
	AudioOutPin pinPulseOut;

	double sampleRate = 44100.0; // default, will be set from getSampleRate()
	double cycleSamples = 0; // samples per cycle
	double pulseDurationSamples = 0; // half of the cycle
	double phase = 0; // phase accumulator for pulse timing

public:
	PulseGen()
	{
		initializePin( pinBool );
		initializePin( pinRateHz );
		initializePin( pinPulseOut );
	}

	void subProcess(int sampleFrames)
	{
		bool onOff = pinBool;
		auto signalOut = getBuffer(pinPulseOut);

		for (int s = 0; s < sampleFrames; ++s)
		{
			if (onOff)
			{
				phase += 1.0;

				if (phase >= cycleSamples)
					phase -= cycleSamples;

				*signalOut = (phase < pulseDurationSamples) ? 0.5 : -0.5;
			}
			else
			{
				phase = 0;
				*signalOut = 0.0;
			}
			
			++signalOut;
		}
	}

	void onSetPins() override
	{
		sampleRate = getSampleRate(); // make sure to get actual sample rate

		if (pinRateHz.isUpdated())
		{
			double Hz = *getBuffer(pinRateHz);
			Hz = Hz * 10.f;
			if (Hz > 0)
			{
				cycleSamples = sampleRate / Hz;
				pulseDurationSamples = cycleSamples / 2.0; // half of the period
			}
			else
			{
				cycleSamples = 0;
				pulseDurationSamples = 0;
			}
		}
		
			if (pinBool)
			{
				setSubProcess(&PulseGen::subProcess);
				pinPulseOut.setStreaming(true);
			}		
		else
			pinPulseOut.setStreaming(false);
	}
};

namespace
{
	auto r = Register<PulseGen>::withId(L"My PulseGen");
}


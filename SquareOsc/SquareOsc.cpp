#include "mp_sdk_audio.h"
#include <cmath> // for pow()

using namespace gmpi;

class SquareOsc final : public MpBase2
{
	AudioInPin pinGateIn;
	AudioInPin pinPitch;
	AudioOutPin pinAudioOut;

	double sampleRate = 44100.0; // default, will be set from getSampleRate()
	double cycleSamples = 0; // samples per cycle
	double pulseDurationSamples = 0; // half of the cycle
	double phase = 0; // phase accumulator for pulse timing
	double V; // voltage input
	double frequency = 440 * pow(2, V - 5);

public:
	SquareOsc()
	{
		initializePin(pinGateIn);
		initializePin(pinPitch);
		initializePin(pinAudioOut);
		setSubProcess(&SquareOsc::subProcess); // Register the process function
	}

	void onSetPins() override
	{
		sampleRate = getSampleRate(); // make sure to get actual sample rate

		if (pinPitch.isUpdated())
		{
			V = *getBuffer(pinPitch) * 10.f;
			double Hz = 440 * pow(2, V - 5);
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

		pinAudioOut.setStreaming(true);
	}

	void subProcess(int sampleFrames)
	{
		auto onOff = getBuffer(pinGateIn);
		auto signalOut = getBuffer(pinAudioOut);

		for (int s = 0; s < sampleFrames; ++s)
		{
			if (*onOff > 0.0)
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
			++onOff;
			++signalOut;
		}
	}
};

namespace
{
	auto r = Register<SquareOsc>::withId(L"SquareOsc");
}

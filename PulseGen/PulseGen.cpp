#include "mp_sdk_audio.h"

using namespace gmpi;

class PulseGen final : public MpBase2
{
    AudioInPin pinOnOff;
    AudioInPin pinRateHz;      // input for frequency in Hz
    AudioOutPin pinSignalOut;

    double sampleRate = 44100.0; // default, will be set from getSampleRate()
    double cycleSamples = 0; // samples per cycle
    double pulseDurationSamples = 0; // half of the cycle
    double phase = 0; // phase accumulator for pulse timing

public:
    PulseGen()
    {
        initializePin(pinOnOff);
        initializePin(pinRateHz);
        initializePin(pinSignalOut);
        setSubProcess(&PulseGen::subProcess); // Register the process function
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

        pinSignalOut.setStreaming(true);
    }

    void subProcess(int sampleFrames)
    {
        auto onOff = getBuffer(pinOnOff);
        auto signalOut = getBuffer(pinSignalOut);

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
    auto r = Register<PulseGen>::withId(L"PulseGen");
}
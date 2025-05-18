#include "mp_sdk_audio.h"

using namespace gmpi;

class Monostable final : public MpBase2
{
    AudioInPin pinOnOff;
    AudioInPin pinPulseLength;      // input for pulse length in ms
    AudioOutPin pinSignalOut;

    double sampleRate = 44100.0; // default, will be set from getSampleRate()
    double cycleSamples = 0;     // samples for pulse duration
    double phase = 0;            // phase accumulator for pulse timing

public:
    Monostable()
    {
        initializePin(pinOnOff);
        initializePin(pinPulseLength);
        initializePin(pinSignalOut);
        setSubProcess(&Monostable::subProcess); // Register the process function
    }

    void onSetPins() override
    {
        sampleRate = getSampleRate();

        if (pinPulseLength.isUpdated())
        {
            double ms = *getBuffer(pinPulseLength);
            ms = ms * 10.f; // se scaling
            if (ms > 0)
                cycleSamples = sampleRate / 1000 * ms;
            else
                cycleSamples = 0;
        }

        pinSignalOut.setStreaming(true);
    }

    void subProcess(int sampleFrames)
    {
        auto onOff = getBuffer(pinOnOff);
        auto signalOut = getBuffer(pinSignalOut);

        // Keep track of whether the pulse is currently active
        static bool pulseActive = false;

        for (int s = 0; s < sampleFrames; ++s)
        {
            bool currentOnOff = (*onOff > 0.0);
            static bool previousOnOff = false;

            // Detect rising edge to start a new pulse
            if (currentOnOff && !previousOnOff)
            {
                // Start new pulse
                phase = 0;
                pulseActive = true;
            }

            previousOnOff = currentOnOff;

            if (pulseActive)
            {
                if (phase < cycleSamples)
                {
                    *signalOut = 0.5f;
                    phase += 1.0;
                }
                else
                {
                    *signalOut = 0.0f;
                    // If onOff is off, and cycle is finished, reset phase
                    if (!currentOnOff)
                    {
                        pulseActive = false;
                        phase = 0; // reset phase after cycle completes
                    }
                }
            }
            else
            {
                // Not active pulse, output zero
                *signalOut = 0.0f;
            }

            ++onOff;
            ++signalOut;
        }
    }
};

namespace
{
    auto r = Register<Monostable>::withId(L"Polystable");
}
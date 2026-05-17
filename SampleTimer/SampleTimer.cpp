#include "mp_sdk_audio.h"

using namespace gmpi;

class SampleTimer final : public MpBase2
{
    AudioInPin pinGate;
    IntInPin pinTriggerTimesamples;
    AudioOutPin pinSignalOut;

private:
    uint64_t timer; // count of samples
    bool outputHigh; // whether output is currently high

public:
    SampleTimer()
    {
        initializePin(pinGate);
        initializePin(pinTriggerTimesamples);
        initializePin(pinSignalOut);
        timer = 0;
        outputHigh = false;
    }

    void subProcess(int sampleFrames)
    {
        auto gate = getBuffer(pinGate);
        auto signalOut = getBuffer(pinSignalOut);
        int triggerSamples = pinTriggerTimesamples.getValue();

        for (int s = 0; s < sampleFrames; ++s)
        {
            // Check if gate is triggered
            if (*gate > 0)
            {
                // Start or continue counting and set output high
                outputHigh = true;
                ++timer;
                *signalOut = 1.0f;
            }
            else
            {
                // Gate not triggered, but if output is still high, keep counting
                if (outputHigh)
                {
                    ++timer;
                    *signalOut = 1.0f;
                }
                else
                {
                    // Output low, reset timer
                    *signalOut = 0.0f;
                    timer = 0;
                }
            }

            // Check if timer reached trigger samples
            if (outputHigh && timer > triggerSamples)
            {
                // Reset everything
                outputHigh = false;
                timer = 0;
                *signalOut = 0.0f;
            }

            ++gate;
            ++signalOut;
        }
    }

    void onSetPins() override
    {
        if (pinTriggerTimesamples.isUpdated())
        {            
        }
        if (pinGate.isStreaming())
        {
            // handle gate streaming state if needed
        }

        pinSignalOut.setStreaming(true);
        setSubProcess(&SampleTimer::subProcess);
    }
};

namespace
{
    auto r = Register<SampleTimer>::withId(L"SampleTimer");
}
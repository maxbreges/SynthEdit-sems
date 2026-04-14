#include "mp_sdk_audio.h"

using namespace gmpi;

class TimeCount final : public MpBase2
{
    AudioInPin pinGate;
    AudioInPin pinReset;
    AudioOutPin pinCount;
    AudioOutPin pinCountMs;

    double sampleRate = 44100.0; // default, will be set from getSampleRate()
    int counter = 0;

public:
    TimeCount()
    {
        initializePin(pinGate);
        initializePin(pinReset);
        initializePin(pinCount);
        initializePin(pinCountMs);
    }

    void subProcess(int sampleFrames)
    {
        // get pointers to in/output buffers.
        auto gate = getBuffer(pinGate);
        auto reset = getBuffer(pinReset);
        auto count = getBuffer(pinCount);
        auto countms = getBuffer(pinCountMs);

        for (int s = sampleFrames; s > 0; --s)
        {
            // Check reset pin first
            if (*reset > 0.0)
            {
                counter = 0;
            }
            else if (*gate > 0.0)
            {
                // Increment counter if gate is active
                ++counter;
            }

            // Write output values
            *count++ = static_cast<float>(counter * 0.1f);
            *countms++ = static_cast<float>(counter / sampleRate * 100.0f);

            // Increment buffer pointers
            ++reset;
            ++gate;
        }
    }

    void onSetPins() override
    {
        // Update sample rate
        sampleRate = getSampleRate();

        // Determine if reset or gate pins are streaming
        if (pinReset.isStreaming())
        {
            // Handle if needed
        }
        if (pinGate.isStreaming())
        {
            // Handle if needed
        }

        // Set output pin to stream
        pinCount.setStreaming(true);

        // Set processing method
        setSubProcess(&TimeCount::subProcess);
    }
};

namespace
{
    auto r = Register<TimeCount>::withId(L"TimeCount");
}
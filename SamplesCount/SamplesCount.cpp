#include "mp_sdk_audio.h"

using namespace gmpi;

class SamplesCount final : public MpBase2
{
    AudioInPin pinSamplesIn;
    AudioInPin pinPulse;
    AudioInPin pinOnOff;    // New on/off control pin
    AudioOutPin pinCount;

private:
    int sampleCounter = 0; // Stores the current sample count
    bool previousPulseState = false; // To detect rising edge
    bool isCountingEnabled = true; // To store the on/off state

public:
    SamplesCount()
    {
        initializePin(pinSamplesIn);
        initializePin(pinPulse);
        initializePin(pinOnOff);   // Initialize on/off pin
        initializePin(pinCount);
    }

    void subProcess(int sampleFrames)
    {
        auto SamplesIn = getBuffer(pinSamplesIn);
        auto Pulse = getBuffer(pinPulse);
        auto countOut = getBuffer(pinCount);
        auto onOffBuffer = getBuffer(pinOnOff);

        for (int s = 0; s < sampleFrames; ++s)
        {
            // Check if counting is enabled
            isCountingEnabled = (onOffBuffer[s] > 0.0f);

            bool currentPulseState = (Pulse[s] > 0.0f);

            if (isCountingEnabled)
            {
                // Detect rising edge of pulse
                if (currentPulseState && !previousPulseState)
                {
                    // Output the current count at pulse
                    countOut[s] = static_cast<float>(sampleCounter) * 0.1f;
                    // Reset counter
                    sampleCounter = 0;
                }
                else
                {
                    // Count samples only if enabled
                    countOut[s] = static_cast<float>(sampleCounter) * 0.1f;
                    ++sampleCounter;
                }
            }
            else
            {
                // When disabled, just output the current count without incrementing
                countOut[s] = static_cast<float>(sampleCounter) * 0.1f;
            }

            previousPulseState = currentPulseState;
        }
    }

    void onSetPins() override
    {
        if (pinSamplesIn.isStreaming())
        {
        }
        if (pinPulse.isStreaming())
        {
        }
        if (pinOnOff.isStreaming())
        {
            // Optional: read initial state of on/off pin
        }

        pinCount.setStreaming(true);

        setSubProcess(&SamplesCount::subProcess);
    }
};

namespace
{
    auto r = Register<SamplesCount>::withId(L"Samples Count");
}
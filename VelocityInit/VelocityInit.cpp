#include "mp_sdk_audio.h"

using namespace gmpi;

class VelocityInit final : public MpBase2
{
    AudioInPin pinSignalin;
    AudioOutPin pinSignalOut;

    bool hasChanged = false; // Tracks if input has changed since last time
    float previousInput = 0.0f; // Stores last input value

public:
    VelocityInit()
    {
        initializePin(pinSignalin);
        initializePin(pinSignalOut);
    }

    void subProcess(int sampleFrames)
    {
        auto signalin = getBuffer(pinSignalin);
        auto signalOut = getBuffer(pinSignalOut);

        if (!signalin || !signalOut)
        {
            // Handle invalid buffers gracefully by doing nothing or zeroing output
            // Since dereferencing is unsafe here, just return
            return;
        }

        for (int s = 0; s < sampleFrames; ++s)
        {
            float currentInput = *signalin;

            if (!hasChanged)
            {
                // On first run, output 0.5f
                *signalOut = 0.7812f;

                // Check if input differs from previous (initially 0.0f)
                if (currentInput != previousInput)
                {
                    hasChanged = true;
                }
            }
            else
            {
                // After first change, mirror input
                *signalOut = currentInput;
            }

            // Save current input for next comparison
            previousInput = currentInput;

            // Increment pointers
            ++signalin;
            ++signalOut;
        }
    }

    void onSetPins() override
    {
        // Set the output pin to streaming
        pinSignalOut.setStreaming(true);

        // Set the processing method
        setSubProcess(&VelocityInit::subProcess);
    }
};

namespace
{
	auto r = Register<VelocityInit>::withId(L"VelocityInit");
}

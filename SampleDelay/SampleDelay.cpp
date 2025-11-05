#include "mp_sdk_audio.h"

using namespace gmpi;

class SampleDelay final : public MpBase2
{
    AudioInPin pinAudioIn;
    IntInPin pinSamplesAmnt; // Delay in samples
    AudioOutPin pinAudioOut;

    // Circular buffer for delay
    std::vector<float> delayBuffer;
    int bufferSize = 0;
    int writeIndex = 0;

public:
    SampleDelay()
    {
        initializePin(pinAudioIn);
        initializePin(pinSamplesAmnt);
        initializePin(pinAudioOut);
    }

    void onSetPins() override
    {
        // Recompute buffer size when delay amount changes
        int delaySamples = pinSamplesAmnt;
        if (delaySamples != bufferSize)
        {
            bufferSize = delaySamples;
            delayBuffer.resize(bufferSize, 0.0f);
            writeIndex = 0;
        }

        // Set output pin to streaming
        pinAudioOut.setStreaming(true);

        // Register the process callback
        setSubProcess(&SampleDelay::subProcess);
    }

    void subProcess(int sampleFrames)
    {
        auto audioIn = getBuffer(pinAudioIn);
        auto audioOut = getBuffer(pinAudioOut);
        int delaySamples = pinSamplesAmnt;

        if (bufferSize == 0)
        {
            // No delay set, pass input directly
            for (int s = 0; s < sampleFrames; ++s)
            {
                audioOut[s] = audioIn[s];
            }
            return;
        }

        for (int s = 0; s < sampleFrames; ++s)
        {
            // Read the delayed sample from the buffer
            int readIndex = (writeIndex - delaySamples + bufferSize) % bufferSize;
            float delayedSample = delayBuffer[readIndex];

            // Write current input sample into buffer
            delayBuffer[writeIndex] = audioIn[s];

            // Output the delayed sample
            audioOut[s] = delayedSample;

            // Increment write index with wrap-around
            writeIndex = (writeIndex + 1) % bufferSize;
        }
    }
};

namespace
{
    auto r = Register<SampleDelay>::withId(L"Sample Delay");
}
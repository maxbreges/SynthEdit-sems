#include "mp_sdk_audio.h"
#include <cmath> // for fmod

using namespace gmpi;

class Modulus final : public MpBase2
{
    AudioInPin pinSignalin;
    FloatInPin pinModulus;
    AudioOutPin pinSignalOut;

public:
    Modulus()
    {
        initializePin(pinSignalin);
        initializePin(pinModulus);
        initializePin(pinSignalOut);
    }

    void subProcess(int sampleFrames)
    {
        auto signalin = getBuffer(pinSignalin);
        auto signalOut = getBuffer(pinSignalOut);
        float modulus = pinModulus.getValue() / 10.f;

        if (modulus == 0)
            return; // Avoid division by zero

        for (int s = 0; s < sampleFrames; ++s)
        {
            // Wrap signal using fmod
            signalOut[s] = fmod(signalin[s], static_cast<float>(modulus));
            if (signalOut[s] < 0)
                signalOut[s] += static_cast<float>(modulus); // Ensure positive result if needed
        }
    }

    void onSetPins() override
    {
        // Check if input signal is streaming
        if (pinSignalin.isStreaming())
        {
            // do something if needed
        }

        // Check if modulus pin is updated
        if (pinModulus.isUpdated())
        {
            // do something if needed
        }

        // Set output pin to streaming if input is streaming
        pinSignalOut.setStreaming(pinSignalin.isStreaming());

        // Set process callback
        setSubProcess(&Modulus::subProcess);
    }
};

class ModulusInt final : public MpBase2
{
    IntInPin pinSignalin;
    IntInPin pinModulus;
    IntOutPin pinSignalOut;

public:
    ModulusInt()
    {
        initializePin(pinSignalin);
        initializePin(pinModulus);
        initializePin(pinSignalOut);
    }

    void subProcess(int sampleFrames)
    {        
    }

    void onSetPins() override
    {
        int signalin = pinSignalin;
        int signalOut = pinSignalOut;
        int modulus = pinModulus;

        if (modulus == 0)
            return; // Avoid division by zero
        // Check if input signal is streaming
        if (pinSignalin.isUpdated())
        {
            // Wrap signal using fmod
            int signalOutValue = signalin % modulus;
            // Assign output pin value
            pinSignalOut.setValue(static_cast<int>(signalOutValue));
        }

        // Check if modulus pin is updated
        if (pinModulus.isUpdated())
        {
            // do something if needed
        }
    }
};

namespace
{
    bool r[] = { Register<Modulus>::withId(L"Modulus"),
        Register<ModulusInt>::withId(L"ModulusInt")
    };
}
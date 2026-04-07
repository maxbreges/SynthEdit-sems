#include "mp_sdk_audio.h"

using namespace gmpi;

class QuadroMix final : public MpBase2
{
    AudioInPin pinInA;
    AudioInPin pinInB;
    AudioInPin pinInC;
    AudioInPin pinInD;
    AudioInPin pinLevel;
    AudioInPin pinJoystickX;
    AudioInPin pinJoystickY;
    AudioOutPin pinOutput;
    IntInPin pinMode;

public:
    QuadroMix()
    {
        initializePin(pinInA);
        initializePin(pinInB);
        initializePin(pinInC);
        initializePin(pinInD);
        initializePin(pinLevel);
        initializePin(pinJoystickX);
        initializePin(pinJoystickY);
        initializePin(pinOutput);
        initializePin(pinMode);
    }

    void subProcess(int sampleFrames)
    {
        auto inA = getBuffer(pinInA);
        auto inB = getBuffer(pinInB);
        auto inC = getBuffer(pinInC);
        auto inD = getBuffer(pinInD);
        auto level = getBuffer(pinLevel);
        auto joystickX = getBuffer(pinJoystickX);
        auto joystickY = getBuffer(pinJoystickY);
        auto output = getBuffer(pinOutput);
        float modeValue = pinMode.getValue();

        // Determine gain based on mode
        float modeGain;
        switch (static_cast<int>(modeValue))
        {
        case 0: // 0dB
            modeGain = 1.0f;
            break;

        case 1: // +3dB
            modeGain = 1.414f; // 10^(3/20)
            break;
        case 2: // +6dB
            modeGain = 2.0f; // 10^(6/20)
            break;
        default:
            modeGain = 1.0f; // fallback
            break;
        }

        for (int s = sampleFrames; s > 0; --s)
        {
            float joyX = *joystickX;
            float joyY = *joystickY;

            float gain = *level;

            float totalGain = gain * modeGain;

            float a = *inA;
            float b = *inB;
            float c = *inC;
            float d = *inD;

            float outA = (0.5f - joyX) * (joyY - (-0.5f)) * a;
            float outB = (joyX - (-0.5f)) * (joyY - (-0.5f)) * b;
            float outC = (joyX - (-0.5f)) * (0.5f - joyY) * c;
            float outD = (0.5f - joyX) * (0.5f - joyY) * d;

            float outSample = (outA + outB + outC + outD) * totalGain;

            *output = outSample;

            ++inA; ++inB; ++inC; ++inD; ++level; ++joystickX; ++joystickY; ++output;
        }
    }

    void onSetPins() override
    {
        // Check streaming status
        if (pinInA.isStreaming() || pinInB.isStreaming() || pinInC.isStreaming() || pinInD.isStreaming() ||
            pinLevel.isStreaming() || pinJoystickX.isStreaming() || pinJoystickY.isStreaming())
        {
            setSubProcess(&QuadroMix::subProcess);
            pinOutput.setStreaming(true);
        }
        else
        {
            setSubProcess(nullptr);
            pinOutput.setStreaming(false);
        }

        if (pinMode.isUpdated())
        {
            // Mode pin updated; can be used to trigger any mode-specific actions if needed
        }
    }
};

namespace
{
    auto r = Register<QuadroMix>::withId(L"QuadroMix");
}
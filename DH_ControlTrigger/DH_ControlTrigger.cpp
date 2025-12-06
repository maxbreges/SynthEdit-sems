#include "mp_sdk_audio.h"

using namespace gmpi;

class DH_ControlTrigger final : public MpBase2
{
    int triggerCounter = 0;
    int triggerDuration = 0;

    BoolOutPin pinTrigger;

public:
    DH_ControlTrigger()
    {
        initializePin(pinTrigger);
    }

    int32_t open() override
    {
        MpBase2::open();
        triggerDuration = static_cast<int>(getSampleRate() * 0.0005f); // 0.5 ms trigger pulse
        return gmpi::MP_OK;
    }

    int32_t receiveMessageFromGui(int32_t id, int32_t size, void* messageData)
    {
        if (id == 223322)
        {
            // Trigger the pulse
            triggerCounter = triggerDuration;
            pinTrigger.setValue(true, getBlockPosition());  // Set trigger high
            setSubProcess(&DH_ControlTrigger::subProcess);    // Start processing to turn off pulse
            return 1; // handled
        }
        return 0; // not handled
    }

    void subProcess(int sampleFrames)
    {
        for (int s = 0; s < sampleFrames; ++s)
        {
            if (triggerCounter > 0)
            {
                triggerCounter--;
                if (triggerCounter == 0)
                {
                    // End of pulse
                    pinTrigger.setValue(false, getBlockPosition() + s);
                }
            }
            else
            {
                // Pulse is done, switch to idle
                setSubProcess(&DH_ControlTrigger::subProcessIdle);
                break; // Exit early to prevent unnecessary processing
            }
        }
    }

    void subProcessIdle(int sampleFrames)
    {
        // Do nothing, idle state
    }
};

namespace
{
    auto r = Register<DH_ControlTrigger>::withId(L"My DH_ControlTrigger");
}
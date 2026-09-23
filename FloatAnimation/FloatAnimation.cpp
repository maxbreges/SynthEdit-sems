#include "mp_sdk_audio.h"

using namespace gmpi;

class FloatAnimation final : public MpBase2
{
    int sampleRate = 44100;//todo: get from host
    bool onOff = false;
    int targetSamples = 0;
    int count = 0;

    float speed = 0.5f;
    int direction = 1; // 1 for forward, -1 for backward
    int frequency = 240; //Hz
    float mult = 3.f;

    int32_t receiveMessageFromGui(int32_t id, int32_t size, const void* msg)
    {
        if (id == 31253 && size == sizeof(bool))
        {
            // Cast msg to bool pointer and assign
            onOff = *(const bool*)msg;
        }
        pinBool.setValue(onOff,getBlockPosition());
        setSleep(false);

        if (id == 31254) //get time value from gui
        {           
           speed = *(const float*)msg;
           mult = (-5.f * speed) + 6.f;
           targetSamples = sampleRate / frequency;
        }
        return MP_OK;
    }

    BoolOutPin pinBool;
    IntOutPin pinIntOut;

public:
    FloatAnimation()
    {
        initializePin(pinBool);
        initializePin(pinIntOut);

        setSubProcess(&FloatAnimation::subProcess);
    }

    int sampleCnt = 0;

    void subProcess(int frames)
    {
        if (!onOff)
        {
            count = 0;
            setSleep(true);
            return;
        }

        sampleCnt += frames;

        if (sampleCnt >= targetSamples)
        {
            sampleCnt = 0;

            // Implement oscillation logic
            if (count >= 60 * mult)
            {
                // Reverse direction
                direction = -1;
            }
            else if (count <= 0)
            {
                // Reverse direction
                direction = 1;
            }

            // Update count based on direction
            count += direction;

            // Send the current count to GUI
            getHost()->sendMessageToGui(312, sizeof(count), &count);
        }

        // Set output pin to current count (or other desired value)
        //pinIntOut.setValue(count, getBlockPosition());
    }
};


namespace
{
    auto r = Register<FloatAnimation>::withId(L"My FloatAnimation");
}



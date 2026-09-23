#include "FloatAnimationGui.h"

REGISTER_GUI_PLUGIN(FloatAnimationGui, L"My FloatAnimation");

FloatAnimationGui::FloatAnimationGui(IMpUnknown* host) : MpGuiBase(host)
, speed(0.5f), mult(3)
    {
        initializePin(pinOnOff, static_cast<MpGuiBaseMemberPtr>(&FloatAnimationGui::onSetOnOff));
        initializePin(pinSpeed, static_cast<MpGuiBaseMemberPtr>(&FloatAnimationGui::onSetSpeed));
        initializePin(pinAnimPos);
    }

    void FloatAnimationGui::onSetOnOff()
    {
        bool onOff = false;
        onOff = pinOnOff;
        getHost()->sendMessageToAudio(31253, sizeof(onOff), &onOff);
        onSetSpeed();
    }

    void FloatAnimationGui::onSetSpeed()
    {
        if (pinSpeed > 1.f)
        {
            pinSpeed = 1.f;
        }
        if (pinSpeed <= 0.f)
        {
            pinSpeed = 0.f;
        }

        speed = pinSpeed;

        getHost()->sendMessageToAudio(31254, sizeof(speed), &speed);
    }

    int sampleCnt = 0; // member or global variable

    int32_t FloatAnimationGui::receiveMessageFromAudio(int32_t id, int32_t size, void* messageData)
    {
        if (id == 312)
        {
            // Cast messageData to int pointer and dereference with explicit cast
            sampleCnt = static_cast<int>(*(const int*)messageData);
        }
        mult = (-5 * speed) + 6;
        float divider = 1.f / (60 * mult);
        pinAnimPos = sampleCnt * divider;

        return gmpi::MP_OK;
    }

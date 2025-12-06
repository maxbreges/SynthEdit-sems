#include "mp_sdk_gui2.h"
#include <chrono>
#include <thread>

using namespace gmpi;

class DH_ControlTriggerGui final : public gmpi_gui::MpGuiInvisibleBase
{
    // Previous state variables
    float previousFloat = 0.0f;
    int previousInt = 0;
    std::wstring previousText;

    // Called when blob pin changes

    void onSetBool()
    {
        if (pinBool)
        {
            onSetTrigger();
        }
    }

    void onSetFloat()
    {
        if (pinFloat != previousFloat)
        {
            onSetTrigger();
            previousFloat = pinFloat;
        }
    }

    void onSetInt()
    {
        if (pinInt != previousInt)
        {
            onSetTrigger();
            previousInt = pinInt;
        }
    }

    void onSetText()
    {
        if (pinText.getValue() != previousText)
        {
            onSetTrigger();
            previousText = pinText.getValue();
        }
    }

    // Pins
    BoolGuiPin pinBool;
    FloatGuiPin pinFloat;
    IntGuiPin pinInt;
    StringGuiPin pinText;
    BoolGuiPin pinTrigger;

public:
    DH_ControlTriggerGui()
    {
        // Initialize pins with callback functions
        initializePin(pinBool, static_cast<MpGuiBaseMemberPtr2>(&DH_ControlTriggerGui::onSetBool));
        initializePin(pinFloat, static_cast<MpGuiBaseMemberPtr2>(&DH_ControlTriggerGui::onSetFloat));
        initializePin(pinInt, static_cast<MpGuiBaseMemberPtr2>(&DH_ControlTriggerGui::onSetInt));
        initializePin(pinText, static_cast<MpGuiBaseMemberPtr2>(&DH_ControlTriggerGui::onSetText));
        initializePin(pinTrigger);
    }

    void onSetTrigger()
    {
        pinTrigger = true;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        pinTrigger = false;
    }
};

namespace
{
    auto r = Register<DH_ControlTriggerGui>::withId(L"My DH_ControlTrigger");
}
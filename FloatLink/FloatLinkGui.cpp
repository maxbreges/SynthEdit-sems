#include "mp_sdk_gui2.h"

using namespace gmpi;

class FloatLinkGui final : public SeGuiInvisibleBase
{
public:
    // Pins
    BoolGuiPin pinLink;                  // Toggle switch for linked/not linked
    BoolGuiPin mDown1;                   // Mouse down for slider 1
    BoolGuiPin mDown2;                   // Mouse down for slider 2
    FloatGuiPin pinAnimationPosition1;   // Slider 1
    FloatGuiPin pinAnimationPosition2;   // Slider 2

private:
    float storedDifference = 0.0f;       // Store difference when not linked
    bool isUpdating = false;

    void onSetMDown1()
    {
        // When slider 1 is touched
        if (!pinLink.getValue()) // Not linked
        {
            // Store the difference
            float pos1 = pinAnimationPosition1.getValue();
            float pos2 = pinAnimationPosition2.getValue();
            storedDifference = pos2 - pos1;
        }
    }

    void onSetMDown2()
    {
        // When slider 2 is touched
        if (!pinLink.getValue()) // Not linked
        {
            // Store the difference
            float pos1 = pinAnimationPosition1.getValue();
            float pos2 = pinAnimationPosition2.getValue();
            storedDifference = pos2 - pos1;
        }
    }

    void onSetAnimationPosition1()
    {
        if (isUpdating) return;

        if (pinLink.getValue()) // If linked
        {
            // Keep difference consistent
            float pos1 = pinAnimationPosition1.getValue();
            float newPos2 = pos1 + storedDifference;
            newPos2 = std::min<float>(std::max<float>(newPos2, 0.0f), 1.0f);
            isUpdating = true;
            pinAnimationPosition2=(newPos2);
            isUpdating = false;
        }
        // Else, do nothing; wait for user interaction
    }

    void onSetAnimationPosition2()
    {
        if (isUpdating) return;

        if (pinLink.getValue()) // If linked
        {
            // Keep difference consistent
            float pos2 = pinAnimationPosition2.getValue();
            float newPos1 = pos2 - storedDifference;
            newPos1 = std::min<float>(std::max<float>(newPos1, 0.0f), 1.0f);
            isUpdating = true;
            pinAnimationPosition1=(newPos1);
            isUpdating = false;
        }
        // Else, do nothing
    }

public:
    FloatLinkGui()
    {
        initializePin(pinLink, nullptr); // PinLink is controlled externally (switch)
        initializePin(mDown1, static_cast<MpGuiBaseMemberPtr2>(&FloatLinkGui::onSetMDown1));
        initializePin(mDown2, static_cast<MpGuiBaseMemberPtr2>(&FloatLinkGui::onSetMDown2));
        initializePin(pinAnimationPosition1, static_cast<MpGuiBaseMemberPtr2>(&FloatLinkGui::onSetAnimationPosition1));
        initializePin(pinAnimationPosition2, static_cast<MpGuiBaseMemberPtr2>(&FloatLinkGui::onSetAnimationPosition2));
    }
};

namespace
{
    auto r = Register<FloatLinkGui>::withId(L"Float Link");
}
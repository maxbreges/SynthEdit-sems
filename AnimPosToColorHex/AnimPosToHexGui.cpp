// Copyright 2025 Max Brezhestovski

#include "mp_sdk_gui2.h"
#include <iomanip>
#include <sstream>
#include <cmath> // for roundf

using namespace gmpi;
using namespace std;

class AnimPosToHex final : public SeGuiInvisibleBase
{
    void onSetAnimationPosition()
    {
        int x = static_cast<int>(roundf(pinAnimationPosition * 1535));
        int R = 0, G = 0, B = 0; // Initialize all to zero

        // Handle G component
        if (x >= 255 && x < 768)
            G = 255;
        else if (x > 1023)
            G = 0; // When x > 1023, G is reset to 0
        else if (x >= 0 && x <= 255)
            G = x; // Fade in from 0 to 255 between 0 and 255

        // Handle R component
        if (x >= 1279 || x < 255)
            R = 255;
        else if (x > 510 && x <= 1024)
            R = 0; // Reset R to 0 between 510 and 1024
        else if (x >= 256 && x <= 511)
            R = 255 - (x - 256); // Fade out from 255 to 0 between 256 and 511
        else if (x >= 1024 && x <= 1279)
            R = x - 1024; // Fade in from 0 to 255 between 1024 and 1279

        // Handle B component
        if (x >= 767 && x < 1280)
            B = 255;
        else if (x <= 511)
            B = 0; // Reset B to 0 below 512
        else if (x >= 512 && x <= 767)
            B = x - 512; // Fade in from 0 to 255 between 512 and 767
        else if (x >= 1280 && x <= 1535)
            B = 255 - (x - 1280); // Fade out from 255 to 0 between 1280 and 1535

        // Convert to hex string
        std::stringstream ssR, ssG, ssB;
        ssR << std::setfill('0') << std::setw(2) << std::hex << R;
        ssG << std::setfill('0') << std::setw(2) << std::hex << G;
        ssB << std::setfill('0') << std::setw(2) << std::hex << B;

        pinHex = "ff" + ssR.str() + ssG.str() + ssB.str();
    }

    FloatGuiPin pinAnimationPosition;
    StringGuiPin pinHex;

public:
    AnimPosToHex()
    {
        initializePin(pinAnimationPosition, static_cast<MpGuiBaseMemberPtr2>(&AnimPosToHex::onSetAnimationPosition));
        initializePin(pinHex);
    }
};

namespace
{
    auto r = Register<AnimPosToHex>::withId(L"AnimPosToHex");
}
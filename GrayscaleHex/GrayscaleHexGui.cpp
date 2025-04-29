#include "mp_sdk_gui2.h"
#include <iomanip> // For std::setw and std::setfill
#include <sstream> // For std::ostringstream
#include <cstdint> // For int32_t

using namespace gmpi;

class GrayscaleHexGui final : public SeGuiInvisibleBase
{
    float AnimPos = 0;
    std::string HexOut;

    void onSetAnimPos()
    {
        AnimPos = pinAnimPos;

        // Clamp AnimPos to [0, 1]
        AnimPos = std::max<float>(0.0f, std::min<float>(1.0f, AnimPos));

        // Calculate the grayscale value (0-255)
        int32_t grayValue = static_cast<int32_t>(AnimPos * 255); // Ensure it stays within the int range

        // Convert to hexadecimal format
        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << std::hex << grayValue;

        // Create the full hex color string
        HexOut = "ff" + oss.str() + oss.str() + oss.str(); // #RRGGBB

        // Set the output pin
        pinHexOut = HexOut;
    }

    void onSetHexOut()
    {
        // This function can be used for output-related actions, if needed.
    }

    FloatGuiPin pinAnimPos;
    StringGuiPin pinHexOut;


public:
    GrayscaleHexGui()
    {
        initializePin(pinAnimPos, static_cast<MpGuiBaseMemberPtr2>(&GrayscaleHexGui::onSetAnimPos));
        initializePin(pinHexOut, static_cast<MpGuiBaseMemberPtr2>(&GrayscaleHexGui::onSetHexOut));
    }

};

namespace
{
    auto r = Register<GrayscaleHexGui>::withId(L"GrayscaleHex");
}
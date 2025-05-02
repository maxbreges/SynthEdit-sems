#include "mp_sdk_gui2.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace gmpi;

class BrightnessGui final : public SeGuiInvisibleBase
{
    StringGuiPin pinHexIn;
    FloatGuiPin pinOpacity;
    FloatGuiPin pinBrightness; // brightness control (0..1)
    StringGuiPin pinHexOut;

    // Helper to convert hex string to uint32_t
    uint32_t hexStringToUint32(const std::string& hexStr)
    {
        uint32_t value = 0;
        std::stringstream ss;
        ss << std::hex << hexStr;
        ss >> value;
        return value;
    }

    // Helper to convert uint32_t to hex string
    std::string uint32ToHexString(uint32_t value, size_t width = 6)
    {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(width) << std::hex << value;
        return ss.str();
    }

    // Extract R, G, B components
    void extractRGB(uint32_t color, uint8_t& r, uint8_t& g, uint8_t& b)
    {
        r = (color >> 16) & 0xFF;
        g = (color >> 8) & 0xFF;
        b = color & 0xFF;
    }

    // Combine R, G, B into uint32
    uint32_t combineRGB(uint8_t r, uint8_t g, uint8_t b)
    {
        return (r << 16) | (g << 8) | b;
    }

    // Adjust color brightness based on brightness parameter (0..1)
    uint32_t adjustBrightness(uint32_t color, float brightness)
    {
        uint8_t r, g, b;
        extractRGB(color, r, g, b);

        // To make color darker based on brightness:
        // brightness = 1.0 -> original color
        // brightness = 0.0 -> black
        r = static_cast<uint8_t>(r * brightness);
        g = static_cast<uint8_t>(g * brightness);
        b = static_cast<uint8_t>(b * brightness);

        return combineRGB(r, g, b);
    }

    void updateColor()
    {
        // Read input color
        std::string inputHex = pinHexIn;
        if (inputHex.size() >= 2 && inputHex[0] == '0' && (inputHex[1] == 'x' || inputHex[1] == 'X'))
            inputHex = inputHex.substr(2);

        uint32_t color = hexStringToUint32(inputHex);

        // Read brightness (0..1)
        float brightness = pinBrightness;

        // Clamp brightness
        if (brightness < 0.0f) brightness = 0.0f;
        if (brightness > 1.0f) brightness = 1.0f;

        // Adjust color brightness
        uint32_t adjustedColor = adjustBrightness(color, brightness);

        // Convert adjusted color to hex string (8 hex digits)
        std::string resultHex = uint32ToHexString(adjustedColor, 8);

        // Map opacity (pinOpacity) from [0,1] to [00,FF]
        uint8_t opacityHex = static_cast<uint8_t>(pinOpacity * 255.0f);
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(opacityHex);
        std::string opacityStr = ss.str();

        // Replace first two characters with opacity
        if (resultHex.size() >= 8)
        {
            resultHex.replace(0, 2, opacityStr);
        }

        // Output
        pinHexOut = resultHex;
    }

    void onSetHexIn()
    {
        updateColor();
    }

    void onSetOpacity()
    {
        updateColor();
    }

    void onSetBrightness()
    {
        updateColor();
    }

public:
    BrightnessGui()
    {
        initializePin(pinHexIn, static_cast<MpGuiBaseMemberPtr2>(&BrightnessGui::onSetHexIn));
        initializePin(pinOpacity, static_cast<MpGuiBaseMemberPtr2>(&BrightnessGui::onSetOpacity));
        initializePin(pinBrightness, static_cast<MpGuiBaseMemberPtr2>(&BrightnessGui::onSetBrightness));
        initializePin(pinHexOut);
    }
};

namespace
{
    auto r = Register<BrightnessGui>::withId(L"Brightness");
}
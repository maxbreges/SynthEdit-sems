#include "mp_sdk_gui2.h"
#include <iomanip> // For std::setw and std::setfill
#include <sstream> // For std::ostringstream
#include <cstdint> // For int32_t
#include <algorithm> // For std::max and std::min
#include <cmath> // For std::fmod

using namespace gmpi;

class GrayscaleHexGui final : public SeGuiInvisibleBase
{
    float AnimPos = 0;
    bool GrayScale = false;
    std::string HexOut;

    void updateHexOutput()
    {
        AnimPos = pinAnimPos;
        GrayScale = pinGrayScale;

        // Clamp AnimPos to [0, 1]
        AnimPos = std::max<float>(0.0f, std::min<float>(1.0f, AnimPos));

        std::ostringstream oss;

        if (GrayScale)
        {
            // Calculate the grayscale value (0-255)
            int32_t grayValue = static_cast<int32_t>(AnimPos * 255.0f);

            // Convert to hexadecimal format
            oss << std::setw(2) << std::setfill('0') << std::hex << grayValue;

            // Create the full hex color string (with alpha FF)
            HexOut = "ff" + oss.str() + oss.str() + oss.str(); // #RRGGBB
        }
        else
        {
            // Generate a color hex based on AnimPos (including purple)
            // We can use a piecewise linear interpolation through key colors.
            // Let's define key points:
            // 0.0: Red (255, 0, 0)
            // 0.2: Purple (128, 0, 128) - approximately
            // 0.4: Blue (0, 0, 255)
            // 0.6: Cyan (0, 255, 255)
            // 0.8: Green (0, 255, 0)
            // 1.0: Yellow (255, 255, 0) - or back to red for a cycle

            float segment = 1.0f / 6.0f; // Divide the 0-1 range into segments

            int32_t r = 0, g = 0, b = 0;

            if (AnimPos >= 0.0f && AnimPos < segment) // Red to Purple
            {
                float t = AnimPos / segment; // Normalized position within this segment
                r = static_cast<int32_t>((1.0f - t) * 255.0f + t * 128.0f);
                b = static_cast<int32_t>(t * 128.0f);
            }
            else if (AnimPos >= segment && AnimPos < segment * 2.0f) // Purple to Blue
            {
                float t = (AnimPos - segment) / segment;
                r = static_cast<int32_t>((1.0f - t) * 128.0f);
                b = static_cast<int32_t>((1.0f - t) * 128.0f + t * 255.0f);
            }
            else if (AnimPos >= segment * 2.0f && AnimPos < segment * 3.0f) // Blue to Cyan
            {
                float t = (AnimPos - segment * 2.0f) / segment;
                b = static_cast<int32_t>((1.0f - t) * 255.0f + t * 255.0f);
                g = static_cast<int32_t>(t * 255.0f);
            }
            else if (AnimPos >= segment * 3.0f && AnimPos < segment * 4.0f) // Cyan to Green
            {
                float t = (AnimPos - segment * 3.0f) / segment;
                b = static_cast<int32_t>((1.0f - t) * 255.0f);
                g = static_cast<int32_t>((1.0f - t) * 255.0f + t * 255.0f);
            }
            else if (AnimPos >= segment * 4.0f && AnimPos < segment * 5.0f) // Green to Yellow
            {
                float t = (AnimPos - segment * 4.0f) / segment;
                g = static_cast<int32_t>((1.0f - t) * 255.0f + t * 255.0f);
                r = static_cast<int32_t>(t * 255.0f);
            }
            else // segment * 5.0f to 1.0f (Yellow to Red - completes the cycle)
            {
                float t = (AnimPos - segment * 5.0f) / segment;
                r = static_cast<int32_t>((1.0f - t) * 255.0f + t * 255.0f);
                g = static_cast<int32_t>((1.0f - t) * 255.0f);
            }


            // Ensure values are within 0-255
            r = std::max<int32_t>(0, std::min<int32_t>(255, r));
            g = std::max<int32_t>(0, std::min<int32_t>(255, g));
            b = std::max<int32_t>(0, std::min<int32_t>(255, b));

            // Convert to hexadecimal format
            oss << std::setw(2) << std::setfill('0') << std::hex << r
                << std::setw(2) << std::setfill('0') << std::hex << g
                << std::setw(2) << std::setfill('0') << std::hex << b;

            // Create the full hex color string (with alpha FF)
            HexOut = "ff" + oss.str();
        }

        // Set the output pin
        pinHexOut = HexOut;
    }

    void onSetAnimPos()
    {
        updateHexOutput();
    }

    void onSetGrayScale()
    {
        updateHexOutput();
    }

    FloatGuiPin pinAnimPos;
    BoolGuiPin pinGrayScale;
    StringGuiPin pinHexOut;


public:
    GrayscaleHexGui()
    {
        initializePin(pinAnimPos, static_cast<MpGuiBaseMemberPtr2>(&GrayscaleHexGui::onSetAnimPos));
        initializePin(pinGrayScale, static_cast<MpGuiBaseMemberPtr2>(&GrayscaleHexGui::onSetGrayScale));
        initializePin(pinHexOut); // Initialize the output pin
    }

};

namespace
{
    auto r = Register<GrayscaleHexGui>::withId(L"GrayscaleHex");
}
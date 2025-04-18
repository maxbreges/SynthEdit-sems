#include "mp_sdk_gui2.h"
#include <iomanip>
#include <sstream>
#include <array>
#include <unordered_map>
#include <string>

using namespace gmpi;

class MidiToHexGui final : public SeGuiInvisibleBase
{
    struct Color {
        int alpha, red, green, blue;
    };

    Color defaultColors[12] = {
    {255, 255, 0, 0},    // C     - Red
    {255, 255, 127, 0},  // C#    - Orange
    {255, 255, 255, 0},  // D     - Yellow
    {255, 127, 255, 0},  // D#    - Yellow-Green
    {255, 0, 255, 0},    // E     - Green
    {255, 0, 255, 127},  // F     - Cyan-Green
    {255, 0, 255, 255},  // F#    - Cyan
    {255, 0, 127, 255},  // G     - Blue-Cyan
    {255, 0, 0, 255},    // G#    - Blue
    {255, 127, 0, 255},  // A     - Violet
    {255, 255, 0, 255},  // A#    - Magenta
    {255, 255, 0, 127}   // B     - Pink
    };

    std::unordered_map<int, Color> customColors; // Store custom colors for notes

    void hexToRgb(const std::string& hex, int& alpha, int& red, int& green, int& blue)
    {
        if (hex.length() == 8) // Expect format AARRGGBB
        {
            alpha = std::stoi(hex.substr(0, 2), nullptr, 16);
            red = std::stoi(hex.substr(2, 2), nullptr, 16);
            green = std::stoi(hex.substr(4, 2), nullptr, 16);
            blue = std::stoi(hex.substr(6, 2), nullptr, 16);
        }
        else
        {
            alpha = 255;
            red = green = blue = 0; // Default to black
        }
    }

    Color getColorForPitch(int pitch)
    {
        auto it = customColors.find(pitch);
        return (it != customColors.end())
            ? it->second
            : defaultColors[pitch];
    }

    void setHexOutputFromColor(const Color& color)
    {
        // Create the hex string
        std::ostringstream hexStream;
        hexStream << std::uppercase << std::hex;
        hexStream << std::setw(2) << std::setfill('0') << color.alpha; // Alpha
        hexStream << std::setw(2) << std::setfill('0') << color.red;   // Red
        hexStream << std::setw(2) << std::setfill('0') << color.green; // Green
        hexStream << std::setw(2) << std::setfill('0') << color.blue;  // Blue        

        // Set the output to the hex pin
        pinHex = hexStream.str();
    }

    void onSetPitchIn()
    {
        // Get the input pitch
        int pitch = pinPitchIn % 12;
        pinNote = pinPitchIn;
        pinDegree = pitch;
        // Retrieve and set the appropriate color
        Color color = getColorForPitch(pitch);
        setHexOutputFromColor(color);
    }

    void onSetLearn()
    {
        if (pinLearn && pinGateIn)
        {
            // Get the pitch number to store the custom color
            int pitch = pinPitchIn % 12;
            std::string customColor = pinColorPicker;

            // Store the custom color for this pitch
            if (!customColor.empty())
            {
                int alpha, red, green, blue;
                hexToRgb(customColor, alpha, red, green, blue);
                customColors[pitch] = { alpha, red, green, blue }; // Store in customColors map
            }

            // Call to update pinHex based on the new color
            onSetPitchIn(); // Recalculate pinHex after setting the custom color

            pinLearn = false; // Reset learning state
        }
    }

    IntGuiPin pinPitchIn;
    BoolGuiPin pinGateIn;
    FloatGuiPin pinLearn;
    StringGuiPin pinHex;
    IntGuiPin pinNote;
    IntGuiPin pinDegree;
    StringGuiPin pinColorPicker;

public:
    MidiToHexGui()
    {
        initializePin(pinPitchIn, static_cast<MpGuiBaseMemberPtr2>(&MidiToHexGui::onSetPitchIn));
        initializePin(pinGateIn, static_cast<MpGuiBaseMemberPtr2>(&MidiToHexGui::onSetLearn));
        initializePin(pinLearn, static_cast<MpGuiBaseMemberPtr2>(&MidiToHexGui::onSetLearn));
        initializePin(pinHex, static_cast<MpGuiBaseMemberPtr2>(&MidiToHexGui::onSetPitchIn));
        initializePin(pinNote, static_cast<MpGuiBaseMemberPtr2>(&MidiToHexGui::onSetPitchIn));
        initializePin(pinDegree, static_cast<MpGuiBaseMemberPtr2>(&MidiToHexGui::onSetPitchIn));
        initializePin(pinColorPicker, static_cast<MpGuiBaseMemberPtr2>(&MidiToHexGui::onSetPitchIn));

        // Optionally initialize pinHex here or through a default method.
    }
};

namespace
{
    auto r = Register<MidiToHexGui>::withId(L"MidiToHex");
}
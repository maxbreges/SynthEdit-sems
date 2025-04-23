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
        {255, 255, 0, 0},   // C     - Red
        {255, 255, 127, 0}, // C#    - Orange
        {255, 255, 255, 0}, // D     - Yellow
        {255, 127, 255, 0}, // D#    - Yellow-Green
        {255, 0, 255, 0},   // E     - Green
        {255, 0, 255, 127}, // F     - Cyan-Green
        {255, 0, 255, 255}, // F#    - Cyan
        {255, 0, 127, 255}, // G     - Blue-Cyan
        {255, 0, 0, 255},   // G#    - Blue
        {255, 127, 0, 255}, // A     - Violet
        {255, 255, 0, 255}, // A#    - Magenta
        {255, 255, 0, 127}  // B     - Pink
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
                customColors[pitch] = { alpha, red, green, blue };

                // Serialize and save to patch memory
                pinCustomColor = serializeColors();
            }

            // Call to update pinHex based on the new color
            onSetPitchIn(); // Recalculate pinHex after setting the custom color
            pinLearn = false; // Reset learning state
        }
    }

    std::string serializeColors() const
    {
        std::ostringstream oss;

        for (const auto& pair : customColors) {
            int pitch = pair.first;
            Color color = pair.second;
            oss << pitch << "," << color.alpha << ","
                << color.red << "," << color.green << ","
                << color.blue << ";";
        }

        return oss.str();
    }

    void deserializeColors(const std::string& data)
    {
        customColors.clear(); // Clear any existing colors before loading new ones
        std::istringstream iss(data);
        std::string segment;

        while (std::getline(iss, segment, ';')) {
            if (segment.empty()) continue;

            std::istringstream colorStream(segment);
            std::string pitchStr, alphaStr, redStr, greenStr, blueStr;

            // Check if we can read each component safely
            if (!std::getline(colorStream, pitchStr, ',') ||
                !std::getline(colorStream, alphaStr, ',') ||
                !std::getline(colorStream, redStr, ',') ||
                !std::getline(colorStream, greenStr, ',') ||
                !std::getline(colorStream, blueStr, ',')) {
                // Handle error
                continue; // Skip this segment
            }

            try {
                int pitch = std::stoi(pitchStr);
                int alpha = std::stoi(alphaStr);
                int red = std::stoi(redStr);
                int green = std::stoi(greenStr);
                int blue = std::stoi(blueStr);

                // Validation check to prevent out-of-range values
                if (pitch < 0 || pitch >= 12) continue; // Only allow pitches 0 to 11
                customColors[pitch] = { alpha, red, green, blue };
            }
            catch (const std::invalid_argument& e) {
                // Handle conversion error
                continue; // Skip this entry if conversion fails
            }
            catch (const std::out_of_range& e) {
                // Handle out-of-range error
                continue; // Skip this entry if out of range
            }
        }
    }

    void onLoadPatch()
    {
        std::string customColorValue = pinCustomColor;
        if (!customColorValue.empty())
        {
            //std::cout << "Loading custom colors from pin." << std::endl;
            deserializeColors(customColorValue);
            onSetPitchIn(); // Update outputs after loading
        }
        else
        {
           // std::cout << "No custom color data to load." << std::endl;
        }
    }

    IntGuiPin pinPitchIn;
    BoolGuiPin pinGateIn;
    FloatGuiPin pinLearn;
    StringGuiPin pinHex;
    IntGuiPin pinNote;
    IntGuiPin pinDegree;
    StringGuiPin pinColorPicker;
    StringGuiPin pinCustomColor;

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
        initializePin(pinCustomColor, static_cast<MpGuiBaseMemberPtr2>(&MidiToHexGui::onLoadPatch));

        // Load the patch state if applicable
        onLoadPatch();
    }
};

namespace
{
    auto r = Register<MidiToHexGui>::withId(L"MidiToHex");
}
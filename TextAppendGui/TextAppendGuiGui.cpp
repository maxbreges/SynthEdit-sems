#include "mp_sdk_gui2.h"
#include <vector>
#include <string>

using namespace std;
using namespace gmpi;

class TextAppendGuiGui final : public SeGuiInvisibleBase
{
    static const int nonRepeatingPinCount = 1; // Only one output pin
    MpGuiPin<std::string> pinTextOut; // Output pin
    std::vector<std::string> inValues; // Input values

public:
    TextAppendGuiGui()
    {
        initializePin(pinTextOut, static_cast<MpGuiBaseMemberPtr2>(&TextAppendGuiGui::onSetTextIn));
    }

    virtual int32_t MP_STDCALL setPin(int32_t pinId, int32_t voice, int32_t size, const void* data) override
    {
        // Call the base implementation to handle the pin setting
        int32_t r = SeGuiInvisibleBase::setPin(pinId, voice, size, data);

        // Determine the index for the input pin
        int inputIdx = pinId - nonRepeatingPinCount;
        if (inputIdx >= 0)
        {
            // Ensure we have enough space in inValues
            std::string defaultValue;
            while (inputIdx >= inValues.size())
            {
                inValues.push_back(defaultValue);
            }

            // Convert incoming raw data to std::string
            std::string value;
            VariableFromRaw(size, data, value);

            // Store the input string
            inValues[inputIdx] = value;

            // Call onSetTextIn to update output whenever an input is changed
            onSetTextIn();
        }

        return r; // Return the result
    }

private:
    void onSetTextIn()
    {
        // Clear and build a new concatenated output string
        std::string result;

        for (const auto& str : inValues)
        {
            result += str; // Concatenate input strings without any delimiter
        }

        // Transmit the concatenated output string
        getHost()->pinTransmit(pinTextOut.getId(), result.size(), result.data());
    }

    int32_t initialize() override
    {
        // Initial setup if needed can be added here
        return SeGuiInvisibleBase::initialize();
    }
};

namespace
{
    auto r = Register<TextAppendGuiGui>::withId(L"TextAppendGui");
}
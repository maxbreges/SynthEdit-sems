#ifndef TEXTAPPENDGUIR_H_INCLUDED
#define TEXTAPPENDGUIR_H_INCLUDED

#include <vector>
#include <string>
#include "../se_sdk3/mp_sdk_gui2.h"

template<typename T>

class TextAppend : public SeGuiInvisibleBase
{
    static const int nonRepeatingPinCount = 1; // Only one output pin

public:
    TextAppend()
    {
        initializePin(pinOutput, static_cast<MpGuiBaseMemberPtr2>(&TextAppend::onSetOutput));
    }

    virtual int32_t MP_STDCALL setPin(int32_t pinId, int32_t voice, int32_t size, const void* data) override
    {
        int32_t r = SeGuiInvisibleBase::setPin(pinId, voice, size, data);

        // Change on one Input pin.
        int inputIdx = pinId - nonRepeatingPinCount;
        if (inputIdx >= 0)
        {
            // In case we haven't counted pins yet.
            // May need to grow input value array to hold this value.
            std::string defaultValue;

            while (inputIdx >= inValues.size())
            {
                inValues.push_back(defaultValue);
            }

            // Convert raw value to string.
            std::string value;
            VariableFromRaw(size, data, value);

            // Store the input string.
            inValues[inputIdx] = value;

            // Call onSetOutput to update output whenever an input is changed
            onSetOutput();
        }

        return r;
    }

private:
    void onSetOutput()
    {
        // Clear previous output and build a new concatenated string
        std::string result;

        for (const auto& str : inValues)
        {
            // Ensure proper handling of each input
            result += str; // Concatenate without any delimiter
        }

        // Transmit the concatenated string
        getHost()->pinTransmit(pinOutput.getId(), result.size(), result.data());
    }

    MpGuiPin<std::string> pinOutput;

    std::vector<std::string> inValues;
};

#endif

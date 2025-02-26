#include "mp_sdk_gui2.h"
#include <vector>
#include <sstream>

using namespace gmpi;

class BooleanORGui final : public SeGuiInvisibleBase
{
    void onSetOut()
    {
        // Calculate the OR of all input values
        bool orResult = false;
        for (bool value : inValues)
        {
            orResult |= value; // Logical OR
        }
        pinOut = orResult; // Update the output pin with the OR result

      //  std::stringstream ss;
      // ss << "Output Pin Set: " << (orResult ? "TRUE" : "FALSE") << std::endl; // Debug output
       // pinDebugOut = ss.str();

        getHost()->pinTransmit(0, sizeof(bool), &pinOut); // Assume pinOut is the first output pin
    }

    void onSetSpare()
    {
        // Process spare input if needed
    }
    StringGuiPin pinDummy;
    StringGuiPin pinDebugOut;
    IntGuiPin pinIndex;
    BoolGuiPin pinOut; // Output pin that sends the OR result
    BoolGuiPin pinSpare; // This is just a placeholder; we'll treat it as an additional input pin

public:
    BooleanORGui()
    {
        // Initialize pins
        initializePin(pinDummy, static_cast<MpGuiBaseMemberPtr2>(&BooleanORGui::onSetOut));
        initializePin(pinDebugOut, static_cast<MpGuiBaseMemberPtr2>(&BooleanORGui::onSetOut));
        initializePin(pinIndex, static_cast<MpGuiBaseMemberPtr2>(&BooleanORGui::onSetOut));
        initializePin(pinOut, static_cast<MpGuiBaseMemberPtr2>(&BooleanORGui::onSetOut));
        initializePin(pinSpare, static_cast<MpGuiBaseMemberPtr2>(&BooleanORGui::onSetSpare)); // treat as additional input

        // Start with an empty vector and will resize dynamically later
    }

    virtual int32_t MP_STDCALL setPin(int32_t pinId, int32_t voice, int32_t size, const void* data) override
    {
        int32_t r = SeGuiInvisibleBase::setPin(pinId, voice, size, data);

        // Store the boolean value in the input vector
        bool value;
        VariableFromRaw<bool>(size, data, value);

        // Adjust the index based on connected pins
        int inputIdx = pinId - 4; // Assuming pinOut (1) and pinDebugOut (0)

        // Resize inValues if the current index exceeds its capacity
        if (inputIdx >= inValues.size())
        {
            inValues.resize(inputIdx + 1, false); // Resize and initialize new slots as FALSE
        }

        // Set the value
        inValues[inputIdx] = value;

        std::stringstream ss;
        ss << "Input Pin " << inputIdx << " Set: " << (value ? "TRUE" : "FALSE") << std::endl; // Debug output
        pinDebugOut = ss.str();
        pinIndex = inputIdx;
        onSetOut(); // Recalculate the output whenever a pin is set
        return r;
    }

private:
    std::vector<bool> inValues; // Start with an empty dynamic vector
};

namespace
{
    auto r = Register<BooleanORGui>::withId(L"BooleanOR");
}

/*
#include "mp_sdk_gui2.h"
#include <vector>
#include <algorithm>
#include <iostream> // For debugging prints
#include <sstream>

using namespace gmpi;

class BooleanORGui final : public SeGuiInvisibleBase
{
    void onSetOut()
    {
        // Calculate the OR of all input values
        bool orResult = std::any_of(inValues.begin(), inValues.end(), [](bool value) {
            return value; // TRUE if any input is TRUE
            });

        // Update the output pin with the OR result
        pinOut = orResult;
        std::stringstream ss;
        ss << "Output Pin Set: " << (orResult ? "TRUE" : "FALSE") << std::endl; // Debug output
        pinDebugOut = ss.str();
        getHost()->pinTransmit(0, sizeof(bool), &pinOut); // Assume pinOut is the first output pin
    }

    void onSetSpare()
    {
        // Process spare input if needed
    }

    StringGuiPin pinDebugOut;
    StringGuiPin pinDebugInput;
    BoolGuiPin pinOut; // Output pin that sends the OR result
    BoolGuiPin pinSpare; // Example of an additional input pin

public:
    BooleanORGui()
    {
        initializePin(pinDebugOut, static_cast<MpGuiBaseMemberPtr2>(&BooleanORGui::onSetOut));
        initializePin(pinDebugInput, static_cast<MpGuiBaseMemberPtr2>(&BooleanORGui::onSetOut));
        initializePin(pinOut, static_cast<MpGuiBaseMemberPtr2>(&BooleanORGui::onSetOut));
        initializePin(pinSpare, static_cast<MpGuiBaseMemberPtr2>(&BooleanORGui::onSetSpare)); // Autoduplicating pin
        inValues.resize(2, false); // Initialize with size for two inputs
    }

    virtual int32_t MP_STDCALL setPin(int32_t pinId, int32_t voice, int32_t size, const void* data) override
    {
        int32_t r = SeGuiInvisibleBase::setPin(pinId, voice, size, data);

        // Process the boolean value received from the pins
        bool value;
        VariableFromRaw<bool>(size, data, value);

        int inputIdx = pinId - 1; // This assumes pinSpare is at index 1
        if (inputIdx >= 0 && inputIdx < inValues.size())
        {
            inValues[inputIdx] = value; // Store the current pin value
            std::stringstream ss;
            ss << "Input Pin " << inputIdx << " Set: " << (value ? "TRUE" : "FALSE") << std::endl; // Debug output
            pinDebugInput = ss.str();
            onSetOut(); // Update the output whenever a pin changes
        }
        return r;
    }

private:
    std::vector<bool> inValues; // Dynamic storage for input values
};

namespace
{
    auto r = Register<BooleanORGui>::withId(L"BooleanOR");
}*/
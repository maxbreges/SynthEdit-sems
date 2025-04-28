#include "mp_sdk_gui2.h"

using namespace std;
using namespace gmpi;

class StepCounterGuiGui final : public SeGuiInvisibleBase
{
private:
    int currentPin = 1;

    void onSetClock()
    {
        // Detect rising edge: going from LOW (false) to HIGH (true)
        if (pinClock)
        {
            // Set the current pin to true, others to false
            for (size_t i = 0; i < pinOut.size(); ++i)
            {
                pinOut[i] = (i == currentPin);
            }

            // Move to the next pin
            currentPin++; // Increment first

            // Wrap around to the first pin if we've exceeded the number of pins
            if (currentPin >= pinOut.size())
            {
                currentPin = 0; // Wrap around to the first pin
            }
        }
    }

    void onSetReset()
    {
        // Reset the current pin to 0
        for (auto& pin : pinOut)
        {
            pin = false; // Turn off all output pins
        }
        currentPin = 1; // Reset to the first pin
        if (!pinOut.empty())
        {
            pinOut[0] = true; // Optionally set the first pin to true
        }
    }

    void onSetOut()
    {     
    }

    BoolGuiPin pinClock;
    BoolGuiPin pinReset;
    vector<BoolGuiPin> pinOut;
    
public:
    StepCounterGuiGui()
    {
        initializePin(pinClock, static_cast<MpGuiBaseMemberPtr2>(&StepCounterGuiGui::onSetClock));
        initializePin(pinReset, static_cast<MpGuiBaseMemberPtr2>(&StepCounterGuiGui::onSetReset));
    }

    int32_t initialize() override // for initializing autoduplicating pins
    {
        // Retrieve total number of pins from the host
        int32_t pinCount = 0;
        gmpi::IMpUserInterfaceHost* host = nullptr;

        if (getHost()->queryInterface(gmpi::MP_IID_UI_HOST, (void**)&host) == gmpi::MP_OK)
        {
            host->getPinCount(pinCount); // Safely call getPinCount          
            host->release(); // Release the host interface when done
        }

        const int numInputPins = max(0, pinCount - 2);
        pinOut.resize(numInputPins); // Resize vector to hold input pins

        // Initialize each input pin in the vector
        for (size_t i = 0; i < numInputPins; ++i)
        {
            initializePin(pinOut[i], static_cast<MpGuiBaseMemberPtr2>(&StepCounterGuiGui::onSetOut));
        }

        onSetReset();
        return SeGuiInvisibleBase::initialize(); // Ensure to call base class's initialize method
    }
};

namespace
{
    auto r = Register<StepCounterGuiGui>::withId(L"StepCounterGui");
}
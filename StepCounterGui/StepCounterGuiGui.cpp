#include "mp_sdk_gui2.h"

using namespace std;
using namespace gmpi;

class StepCounterGuiGui final : public SeGuiInvisibleBase
{
    void onSetClock()
    {
        static int currentPin = 0;

        // Check if the clock is received
        if (pinClock)
        {
            if (currentPin >= pinOut.size()) currentPin = 0; // Wrap around
            for (size_t i = 0; i < pinOut.size(); ++i)
            {
                pinOut[i] = (i == currentPin); // Set current pin to true, others to false
            }
            currentPin++; // Prepare for next clock pulse
        }
    }

    void onSetReset()
    {
        // Reset the current pin to 0
        for (auto& pin : pinOut)
        {
            pin = false; // Turn off all output pins
        }
        if (!pinOut.empty())
        {
            pinOut[0] = true; // Set the first pin to true
        }
    }

    void onSetOut()
    {
        // This can be updated based on your specific output needs
        // May just be a placeholder if already managed in the clock handling
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

    int32_t initialize() override //for initializing autoduplicating pins
    {
        // Retrieve total number of pins from the host
        int32_t pinCount = 0;
        // Access the base host interface (IMpUserInterfaceHost) to obtain pin count
        gmpi::IMpUserInterfaceHost* host = nullptr;
        if (getHost()->queryInterface(gmpi::MP_IID_UI_HOST, (void**)&host) == gmpi::MP_OK)
        {
            host->getPinCount(pinCount); // Safely call getPinCount          
            host->release(); // Release the host interface when done
        }

        // Assuming that we reserve at least 2 pins (one output and one internal)
        const int numInputPins = max(0, pinCount - 2);

        try
        {
            pinOut.resize(numInputPins); // Resize vector to hold input pins

            // Initialize each input pin in the vector
            for (size_t i = 0; i < numInputPins; ++i)
            {
                initializePin(pinOut[i], static_cast<MpGuiBaseMemberPtr2>(&StepCounterGuiGui::onSetOut));
            }
        }
        catch (const std::exception& e)
        {
            // Log or handle the exception if pin initialization fails
            return MP_FAIL; // Indicate failure
        }    
        onSetReset();
        return SeGuiInvisibleBase::initialize(); // Ensure to call base class's initialize method
    }
};

namespace
{
	auto r = Register<StepCounterGuiGui>::withId(L"StepCounterGui");
}

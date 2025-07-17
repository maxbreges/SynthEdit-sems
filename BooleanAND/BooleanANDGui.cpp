#include "mp_sdk_gui2.h"

using namespace std;
using namespace gmpi;

class BooleanANDxGui final : public SeGuiInvisibleBase
{
   // IntGuiPin pinIntIn;             // This pin allows user input (can be ignored for auto allocation)
    BoolGuiPin pinBoolOut;          // Output pin
    vector<BoolGuiPin> pinSpareIn;  // Vector to hold input pins

public:
    BooleanANDxGui()
    {
        // Initialize output pin
     //   initializePin(pinIntIn, static_cast<MpGuiBaseMemberPtr2>(&BooleanANDxGui::onSetSpareIn));
        initializePin(pinBoolOut, static_cast<MpGuiBaseMemberPtr2>(&BooleanANDxGui::onSetSpareIn));
    }

    void onSetSpareIn()
    {
        // Perform AND operation when any input pin changes
        bool result = true; // Start with true for AND operation

        // Check the value of all input pins
        for (size_t i = 0; i < pinSpareIn.size(); i++)
        {
            bool inputValue = pinSpareIn[i]; // Get the value from the current input pin
            result = result && inputValue; // AND all inputs together
        }

        // Transmit the result to the output pin
        pinBoolOut = result;
    }

    int32_t initialize() override
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
        const int numInputPins = max(0, pinCount - 1);

        try
        {
            pinSpareIn.resize(numInputPins); // Resize vector to hold input pins

            // Initialize each input pin in the vector
            for (size_t i = 0; i < numInputPins; ++i)
            {
                initializePin(pinSpareIn[i], static_cast<MpGuiBaseMemberPtr2>(&BooleanANDxGui::onSetSpareIn));
            }
        }
        catch (const std::exception& e)
        {
            // Log or handle the exception if pin initialization fails
            return MP_FAIL; // Indicate failure
        }

        return SeGuiInvisibleBase::initialize(); // Ensure to call base class's initialize method
    }

    int32_t setPin(int32_t pinId, int32_t voice, int32_t size, const void* data) override
    {
        // Call the base class's setPin method
        int32_t r = SeGuiInvisibleBase::setPin(pinId, voice, size, data);

        // If an input pin is changed, update and perform the AND operation
        if (pinId >= 0 && pinId < pinSpareIn.size())
        {
            onSetSpareIn();
        }

        return r;
    }
};

namespace
{
    auto r = Register<BooleanANDxGui>::withId(L"BooleanAND");
    auto r = Register<BooleanANDxGui>::withId(L"BooleanANDL");
}
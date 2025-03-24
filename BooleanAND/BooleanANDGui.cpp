#include "mp_sdk_gui2.h"

using namespace std;
using namespace gmpi;

class BooleanANDGui final : public SeGuiInvisibleBase
{
    IntGuiPin pinIntIn;
  //  IntGuiPin pinIntOut;
    BoolGuiPin pinBoolOut;               // Output pin
    vector<BoolGuiPin> pinSpareIn;       // Vector to hold input pins

    void onSetSpareIn()
    {
        // Perform AND operation when any input pin changes
        bool result = true; // Start with true for AND operation

        // Check the value of all input pins
        for (size_t i = 0; i < pinSpareIn.size(); i++)
        {
            bool inputValue = pinSpareIn[i]; // Get the value from the current input pin

            // Perform AND operation
            result = result && inputValue; // AND all inputs together
           // pinIntOut = i+1;
        }

        // Transmit the result to the output pin
        pinBoolOut = result;        
    }

public:
    BooleanANDGui()
    {
        // Initialize output pin
        initializePin(pinIntIn, static_cast<MpGuiBaseMemberPtr2>(&BooleanANDGui::onSetSpareIn));
     //   initializePin(pinIntOut, static_cast<MpGuiBaseMemberPtr2>(&BooleanANDGui::onSetSpareIn));
        initializePin(pinBoolOut, static_cast<MpGuiBaseMemberPtr2>(&BooleanANDGui::onSetSpareIn));
    }

    int32_t initialize() override
    {
        // Define how many input pins to allocate. Assume we'll use 2 as an example.
        const int numInputPins = pinIntIn;
       
        try
        {
            pinSpareIn.resize(numInputPins); // Resize vector to hold input pins

            // Initialize each input pin
            for (size_t i = 0; i < numInputPins; ++i)
            {
                initializePin(pinSpareIn[i], static_cast<MpGuiBaseMemberPtr2>(&BooleanANDGui::onSetSpareIn));
                
            }
        }
        catch (const std::exception& e)
        {
            // Log or handle the exception if pin initialization fails
            return MP_FAIL; // Indicate failure
        }

        return MP_OK; // Indicate successful initialization
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
    auto r = Register<BooleanANDGui>::withId(L"BooleanAND");
}
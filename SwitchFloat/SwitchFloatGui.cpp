#include "mp_sdk_gui2.h"

using namespace std;
using namespace gmpi;

class SwitchFloatGui final : public SeGuiInvisibleBase
{
    vector<FloatGuiPin> pinSpare; // Vector to hold spare pins
    FloatGuiPin pinValue;          // The value that will be assigned to pinSpare
    IntGuiPin pinChoice;           // The pin that hold the index for selection

public:
    SwitchFloatGui() 
    {
        initializePin(pinChoice, static_cast<MpGuiBaseMemberPtr2>(&SwitchFloatGui::onSetChoice));
        initializePin(pinValue, static_cast<MpGuiBaseMemberPtr2>(&SwitchFloatGui::onSetValue));
    }

    void onSetChoice()
    {
        int choiceIndex = pinChoice; // Get the current index from pinChoice
        
        // Check for a valid index
        if (choiceIndex >= 0 && choiceIndex < static_cast<int>(pinSpare.size()))
        {
            pinSpare[choiceIndex] = pinValue; // Assign pinValue to the selected pinSpare
        }
    }

    void onSetValue()
    {
        // Optional: You might want to update all spare pins
        // Uncomment below if you want to set them all to pinValue when it changes
        //for (size_t i = 0; i < pinSpare.size(); ++i)
        //{
         //  pinSpare[i] = pinValue;
        //}
        onSetChoice();
    }

    virtual int32_t MP_STDCALL setPin(int32_t pinId, int32_t voice, int32_t size, const void* data) override
    {
        int32_t result = SeGuiInvisibleBase::setPin(pinId, voice, size, data);

        // Assuming pinId 0 is for pinChoice and pinId 1 is for pinValue, adjust as needed
        if (pinId == pinChoice) 
        {
            // Assuming this signals a change in pinChoice
            onSetChoice(); // Update spare pin based on choice
        } 
        else if (pinId == pinValue)
        {
            // Update pinValue
            // Convert raw value and set pinValue using your data method
            float value; // Assuming FloatGuiPin holds float values
            VariableFromRaw<float>(size, data, value);
            pinValue = value; // Set the internal pinValue

            // Optionally propagate to all spare pins if applicable
            onSetValue(); // Optional and depends on your desired behavior
        }

        return result;
    }

    int32_t initialize() override
    {
        int32_t pinCount = 0;
        gmpi::IMpUserInterfaceHost* host = nullptr;
        if (getHost()->queryInterface(gmpi::MP_IID_UI_HOST, (void**)&host) == gmpi::MP_OK)
        {
            host->getPinCount(pinCount);
            host->release();
        }

        try 
        {
            pinSpare.resize(max(0, pinCount - 2)); // Ensure the pinSpare vector is resized
            for (size_t i = 0; i < pinSpare.size(); ++i)
            {
                initializePin(pinSpare[i], nullptr); // Initialize without callback
            }
        } 
        catch (const std::exception& e)
        {
            return MP_FAIL; // Handle exceptions
        }

        return SeGuiInvisibleBase::initialize();
    }
};

namespace
{
    auto r = Register<SwitchFloatGui>::withId(L"SwitchFloat");
}
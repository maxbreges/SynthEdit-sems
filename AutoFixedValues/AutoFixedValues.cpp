#include "mp_sdk_audio.h"
#include <vector>

using namespace gmpi;

class AutoFixedValues final : public MpBase2
{
public:
    std::vector<FloatOutPin> outputPins; // Vector to hold multiple output pins

    AutoFixedValues()
    {
        // This will be filled in the open() method
    }

    int32_t MP_STDCALL open() override
    {
        // Create pin iterator to check number of pins
        gmpi_sdk::mp_shared_ptr<gmpi::IMpPinIterator> it;
        if (gmpi::MP_OK == getHost()->createPinIterator(it.getAddressOf()))
        {
            int32_t pinCount;
            it->getCount(pinCount); // Get count of pins

            outputPins.resize(pinCount); // Resize the vector to hold the desired number of pins

            for (auto& pin : outputPins)
            {
                initializePin(pin); // Initialize each output pin
            }
        }
        return MpBase2::open();
    }

    void onSetPins() override
    {
        int32_t k = static_cast<int32_t>(outputPins.size());

        if (k > 1) // Ensure that we have more than one pin to avoid division by zero
        {
            // Calculate and set the float values for each output pin
            for (size_t i = 0; i < outputPins.size(); ++i)
            {
                float value = (static_cast<float>(i) * 10.0f) / (k - 1); // Calculate value based on the formula
                outputPins[i] = value; // Assign the calculated value to the output pin
            }
        }
        else if (k == 1) // If there is only one pin, it should output 10
        {
            outputPins[0] = 10.0f; // The only pin should output 10 when k is 1
        }
    }

    void process(int32_t count, const gmpi::MpEvent* events) override
    {
        // Ensure pin values are updated during processing
        onSetPins();
    }
};

namespace
{
    auto r = Register<AutoFixedValues>::withId(L"AutoFixedValues");
}
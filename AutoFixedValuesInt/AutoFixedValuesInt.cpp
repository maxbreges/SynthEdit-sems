#include "mp_sdk_audio.h"
#include <vector>

using namespace gmpi;

class AutoFixedValuesInt final : public MpBase2
{
public:
    std::vector<IntOutPin> outputPins; // Vector to hold multiple output pins

    AutoFixedValuesInt()
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
        // Set the value of each pin to its index
        for (size_t i = 0; i < outputPins.size(); ++i)
        {
            outputPins[i] = static_cast<int>(i); // Set the integer value to its index
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
    auto r = Register<AutoFixedValuesInt>::withId(L"AutoFixedValuesInt");
}
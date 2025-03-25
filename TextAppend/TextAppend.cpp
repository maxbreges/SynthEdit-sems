// Sasha Radojevich for Rob Herder
#include "../se_sdk3/mp_sdk_audio.h"

using namespace std;
using namespace gmpi;

class Rhdsptextappend : public MpBase2
{
    StringOutPin pinTextOut;
    vector<StringInPin> pinInputs; // not StringInPin*, just StringInPin

public:
    Rhdsptextappend()
    {
        initializePin(pinTextOut);        
    }

    int32_t MP_STDCALL open() override
    {
        // initialise auto-duplicate pins.
        const int fixedPinCount = 1; // Instead of "nonRepeatingPinCount" class variable
        gmpi_sdk::mp_shared_ptr<gmpi::IMpPinIterator> it;

        if (getHost()->createPinIterator(it.getAddressOf()) == gmpi::MP_OK)
        {
            int32_t inPinCount; // Moved here, no need to store it in parent class 
            it->getCount(inPinCount);
            inPinCount -= fixedPinCount; // calc number of input values (not counting "output")

            //wstring initVal = L""; // Not needed, and it should be "StringInPin" not wstring
            pinInputs.assign(inPinCount, {}); // Initial value can be empty array: {}

            for (auto& p : pinInputs)
            {
                initializePin(p);
            }
        }

        return MpBase2::open();
    }

    void onSetPins(void) override
    {
        if (pinInputs.size()) // No need to write "> 0" since 0 is false, and everything above is true
        {
            wstring out;

            for (int i = 0; i < pinInputs.size(); i++)
            {
                out += pinInputs[i].getValue() + L' '; // You can use '+' to append strings
            }

            pinTextOut = out;
        }
    }
};

namespace
{
    auto r = Register<Rhdsptextappend>::withId(L"TextAppend");
}


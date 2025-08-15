#include "mp_sdk_audio.h"

using namespace gmpi;

class IntChange final : public MpBase2
{
    int ms = 0;
    int lastIntVal = -1;
    bool isInitialized = false;
    int triggerDuration = 0;

    IntInPin pinIntVal;
    BoolOutPin pinBoolVal;
    IntInPin pinPulseLength;
    IntInPin pinParameter;

public:
	IntChange()
	{
		initializePin( pinIntVal );
		initializePin( pinBoolVal );
        initializePin(pinPulseLength);
        initializePin(pinParameter);

       // lastIntVal = pinIntVal.getValue();
	}

    int32_t open() override
    {
        MpBase2::open();	// always call the base class

        triggerDuration = static_cast<int>(getSampleRate() * 0.001f); // 1 ms trigger pulse.

        return gmpi::MP_OK;
    }

	void onSetPins() override
	{
        if (pinParameter.isUpdated() || pinIntVal.isUpdated())
        {
          //  int currentVal = pinParameter.getValue();
            ms = static_cast<int>(pinPulseLength);

            // Send pulse only if initialized and value has changed
            if (isInitialized)// && currentVal != lastIntVal)
            {
                pinBoolVal.setValue(true, getBlockPosition());
                pinBoolVal.setValue(false, getBlockPosition() + triggerDuration * ms);
            }

            // Update lastIntVal
         //   lastIntVal = currentVal;

            // Mark as initialized after first update
            if (!isInitialized)
            {
                isInitialized = true;
            }
        }        
	}
};

namespace
{
	auto r = Register<IntChange>::withId(L"My IntChange");
}

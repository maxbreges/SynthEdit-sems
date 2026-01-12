#include "mp_sdk_audio.h"

using namespace gmpi;

class SampleHoldInt final : public MpBase2
{
	IntInPin pinAudio;
	BoolInPin pinHold;
	IntOutPin pinOutput;

private:
	bool gate_state = false;
	float hold_audio = pinAudio.getValue();
	bool hold = pinHold.getValue();
public:
	SampleHoldInt()
	{
		initializePin( pinAudio );
		initializePin( pinHold );
		initializePin(pinOutput);
	}

/*	void subProcess( int sampleFrames )
	{
		// get pointers to in/output buffers.
		auto audio = getBuffer(pinAudio);
		auto hold = getBuffer(pinHold);
		auto output = getBuffer(pinOutput);

	
		for (int s = sampleFrames; s > 0; s--)
		{


			*output++ = hold_audio;
			audio++;
		}		
	}*/

	void onSetPins() override
	{
		// Check which pins are updated.
		if( pinAudio.isUpdated() )
		{
		}
		if( pinHold.isUpdated() )
		{	
			bool new_gate = (hold > false);
			if (new_gate != gate_state)
			{
				gate_state = new_gate;
				if (gate_state)
				{
					pinOutput = audio;
				}
			}
		}

		// Set state of output audio pins.
		pinOutput.setUpdated();
		// Set processing method.
		//setSubProcess(&SampleHold::subProcess);
	}
};

namespace
{
	auto r = Register<SampleHoldInt>::withId(L"SampleHoldInt");
}

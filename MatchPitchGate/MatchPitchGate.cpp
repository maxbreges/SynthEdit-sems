#include "mp_sdk_audio.h"

using namespace gmpi;

class MatchPitch final : public MpBase2
{
	AudioInPin pinGateIn;
	AudioInPin pinInput;
	AudioInPin pinValue;
	AudioOutPin pinGate;

public:
	MatchPitch()
	{
		initializePin(pinGateIn);
		initializePin( pinInput );
		initializePin( pinValue );
		initializePin( pinGate );
	}

	void subProcess( int sampleFrames )
	{
		// get pointers to in/output buffers.
		//auto gateinput = getBuffer(pinGateIn);
		auto input = getBuffer(pinInput);
		auto value = getBuffer(pinValue);
		auto gate = getBuffer(pinGate);

		const float epsilon = 0.0001f; // Define a small tolerance for comparison

		for( int s = sampleFrames; s > 0; --s )
		{
			// TODO: Signal processing goes here.
			//bool gatein = *gateinput;
			float in = *input; 
			float val = *value;
			
			        // Use epsilon for comparison
        float gat = (fabs(in - val) < epsilon && pinGateIn) ? 1.f : 0.f;

        // Store the result in the output gate buffer
        *gate++ = gat;
			// Increment buffer pointers.
			++input;
			++value;
		}
	}

	void onSetPins() override
	{
		// Check which pins are updated.
		if( pinGateIn.isStreaming() )
		{
		}		
		if( pinInput.isStreaming() )
		{
		}
		if( pinValue.isStreaming() )
		{
		}
	

		// Set state of output audio pins.
		pinGate.setStreaming(true);

		// Set processing method.
		setSubProcess(&MatchPitch::subProcess);
	}
};

namespace
{
	auto r = Register<MatchPitch>::withId(L"MatchPitchGate");
}

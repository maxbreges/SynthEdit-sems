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

		for( int s = sampleFrames; s > 0; --s )
		{
			// TODO: Signal processing goes here.
			//bool gatein = *gateinput;
			int in = *input * 1000;
			int val = *value * 1000;
			float gat = *gate;

			if ((in == val) && (pinGateIn))
			{
			 gat = 1.f;
			}
			else
			{
				gat = 0.f;
			}
			// Increment buffer pointers.
			++input;
			++value;
			*gate++ = gat;
		}
	}

	void onSetPins() override
	{
		// Check which pins are updated.
		
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

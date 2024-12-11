#include "mp_sdk_audio.h"

using namespace gmpi;

class MatchPitch final : public MpBase2
{
	AudioInPin pinInput;
	AudioInPin pinValue;
	AudioOutPin pinGate;

public:
	MatchPitch()
	{
		initializePin( pinInput );
		initializePin( pinValue );
		initializePin( pinGate );
	}

	void subProcess( int sampleFrames )
	{
		// get pointers to in/output buffers.
		auto input = getBuffer(pinInput);
		auto value = getBuffer(pinValue);
		auto gate = getBuffer(pinGate);

		for( int s = sampleFrames; s > 0; --s )
		{
			// TODO: Signal processing goes here.

			int in = *input * 100;
			int val = *value * 100;
			float gat = *gate;

			if (in == val)
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
	auto r = Register<MatchPitch>::withId(L"MatchPitch");
}

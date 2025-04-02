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
    auto input = getBuffer(pinInput);
    auto value = getBuffer(pinValue);
    auto gate = getBuffer(pinGate);

    const float epsilon = 0.001f; // Define a small tolerance

    for( int s = sampleFrames; s > 0; --s )
    {
        float in = *input; // Keep them as floats
        float val = *value;

        // Use epsilon for comparison
        float gat = (fabs(in - val) < epsilon) ? 1.f : 0.f;

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

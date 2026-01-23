#include "mp_sdk_audio.h"

using namespace gmpi;

class MouseUpTrigger final : public MpBase2
{
	AudioInPin pinInput;
	AudioOutPin pinOutput;

	bool prevInputState = false;

public:
	MouseUpTrigger()
	{
		initializePin( pinInput );
		initializePin( pinOutput );
	}

	void subProcess( int sampleFrames )
	{
		// get pointers to in/output buffers.
		auto input = getBuffer(pinInput);
		auto output = getBuffer(pinOutput);

		for( int s = sampleFrames; s > 0; --s )
		{
			// TODO: Signal processing goes here.
				bool currentInputState = (*input > 0);

				if (prevInputState && !currentInputState)
				{
					*output = 1.f;					
				}
				else
				{
					*output = 0.f;
				}

				prevInputState = currentInputState;

			// Increment buffer pointers.
			++input;
			++output;
		}
	}

	void onSetPins() override
	{
		// Check which pins are updated.
		if( pinInput.isStreaming() )
		{
		}

		// Set state of output audio pins.
		pinOutput.setStreaming(true);

		// Set processing method.
		setSubProcess(&MouseUpTrigger::subProcess);
	}
};

namespace
{
	auto r = Register<MouseUpTrigger>::withId(L"MouseUpTrigger");
}

#include "mp_sdk_audio.h"

using namespace gmpi;

class Modulus final : public MpBase2
{
	AudioInPin pinInput1;
	
	AudioOutPin pinOutput;

public:
	Modulus()
	{
		initializePin( pinInput1 );
		
		initializePin( pinOutput );
	}

	void subProcess( int sampleFrames )
	{
		// get pointers to in/output buffers.
		auto input1 = getBuffer(pinInput1);
		auto input2 = getBuffer(pinInput1);
		auto output = getBuffer(pinOutput);

		for( int s = sampleFrames; s > 0; --s )
		{
			// TODO: Signal processing goes here.

			double a = *input1 * 10;
			double b = *input2 * 10;
			int c = b;

			float result = (a - c)*1.2f;
			*output = result;

			// Increment buffer pointers.
			++input1;
			++input2;
			++output;
		}
	}

	void onSetPins() override
	{
		// Check which pins are updated.
		if( pinInput1.isStreaming() )
		{
		}
		

		// Set state of output audio pins.
		pinOutput.setStreaming(true);

		// Set processing method.
		setSubProcess(&Modulus::subProcess);
	}
};

namespace
{
	auto r = Register<Modulus>::withId(L"My Modulus");
}

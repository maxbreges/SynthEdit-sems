#include "mp_sdk_audio.h"

using namespace gmpi;

class GUIBoolGateVolts final : public MpBase2
{
	BoolInPin pinFromGUI;
	AudioInPin pinIn;
	AudioOutPin pinOut;

public:
	GUIBoolGateVolts()
	{
		initializePin( pinFromGUI );
		initializePin(pinIn);
		initializePin( pinOut );
	}

	void subProcess( int sampleFrames )
	{
		// get pointers to in/output buffers.
		auto in = getBuffer(pinIn);
		auto out = getBuffer(pinOut);

		for( int s = sampleFrames; s > 0; --s )
		{
			// TODO: Signal processing goes here.
			if (pinFromGUI == true)
			{
				*out = *in;
			}
			else
				*out = 0.0f;
			// Increment buffer pointers.
			++in;
			++out;
		}
	}

	void onSetPins() override
	{
		// Check which pins are updated.
		if( pinFromGUI.isUpdated())
		{
		}

		if (pinIn.isUpdated())
		{
		}

		// Set state of output audio pins.
		
		pinOut.setStreaming(true);

		// Set processing method.
		setSubProcess(&GUIBoolGateVolts::subProcess);
	}
};

namespace
{
	auto r = Register<GUIBoolGateVolts>::withId(L"GUIBoolGateVolts");
}

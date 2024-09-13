#include "mp_sdk_audio.h"

using namespace gmpi;

class FloatToVolts final : public MpBase2
{
	FloatInPin pinChannelM1;
	FloatInPin pinChannelM2;
	FloatInPin pinChannelM3;
	FloatInPin pinChannelM4;
	FloatInPin pinChannelM5;
	FloatInPin pinChannelM6;
	FloatInPin pinChannelM7;
	FloatInPin pinChannelM8;
	
	AudioInPin pinIn1;
	AudioInPin pinIn2;
	AudioInPin pinIn3;
	AudioInPin pinIn4;
	AudioInPin pinIn5;
	AudioInPin pinIn6;
	AudioInPin pinIn7;
	AudioInPin pinIn8;

	AudioOutPin pinOut1;
	AudioOutPin pinOut2;
	AudioOutPin pinOut3;
	AudioOutPin pinOut4;
	AudioOutPin pinOut5;
	AudioOutPin pinOut6;
	AudioOutPin pinOut7;
	AudioOutPin pinOut8;
	
public:
	FloatToVolts()
	{
		initializePin( pinChannelM1 );
		initializePin(pinChannelM2);
		initializePin(pinChannelM3);
		initializePin(pinChannelM4);
		initializePin(pinChannelM5);
		initializePin(pinChannelM6);
		initializePin(pinChannelM7);
		initializePin(pinChannelM8);

		initializePin(pinIn1);
		initializePin(pinIn2);
		initializePin(pinIn3);
		initializePin(pinIn4);
		initializePin(pinIn5);
		initializePin(pinIn6);
		initializePin(pinIn7);
		initializePin(pinIn8);

		initializePin( pinOut1 );	
		initializePin(pinOut2);
		initializePin(pinOut3);
		initializePin(pinOut4);
		initializePin(pinOut5);
		initializePin(pinOut6);
		initializePin(pinOut7);
		initializePin(pinOut8);
	}

	void subProcess(int sampleFrames)
	{
		// get pointers to in/output buffers.
		auto in1 = getBuffer(pinIn1);
		auto in2 = getBuffer(pinIn2);
		auto in3 = getBuffer(pinIn3);
		auto in4 = getBuffer(pinIn4);
		auto in5 = getBuffer(pinIn5);
		auto in6 = getBuffer(pinIn6);
		auto in7 = getBuffer(pinIn7);
		auto in8 = getBuffer(pinIn8);

		auto out1 = getBuffer(pinOut1);
		auto out2 = getBuffer(pinOut2);
		auto out3 = getBuffer(pinOut3);
		auto out4 = getBuffer(pinOut4);
		auto out5 = getBuffer(pinOut5);
		auto out6 = getBuffer(pinOut6);
		auto out7 = getBuffer(pinOut7);
		auto out8 = getBuffer(pinOut8);

		float value = pinChannelM1;
		float value2 = pinChannelM2;
		float value3 = pinChannelM3;
		float value4 = pinChannelM4;
		float value5 = pinChannelM5;
		float value6 = pinChannelM6;
		float value7 = pinChannelM7;
		float value8 = pinChannelM8;

		for( int s = sampleFrames; s > 0; --s )
		{		
	
			// TODO: Signal processing goes here.

			*out1 = *in1 * value;
			*out2 = *in2 * value2;
			*out3 = *in3 * value3;
			*out4 = *in4 * value4;
			*out5 = *in5 * value5;
			*out6 = *in6 * value6;
			*out7 = *in7 * value7;
			*out8 = *in8 * value8;
	
			// Increment buffer pointers.
			++in1;
			++out1;	
			++in2;
			++out2;
			++in3;
			++out3;
			++in4;
			++out4;
			++in5;
			++out5;
			++in6;
			++out6;
			++in7;
			++out7;
			++in8;
			++out8;
		}
	}

	void onSetPins() override
	{
	
		// Set processing method.
		setSubProcess(&FloatToVolts::subProcess);
		pinOut1.setStreaming(false);
		pinOut2.setStreaming(false);
		pinOut3.setStreaming(false);
		pinOut4.setStreaming(false);
		pinOut5.setStreaming(false);
		pinOut6.setStreaming(false);
		pinOut7.setStreaming(false);
		pinOut8.setStreaming(false);
	}
};

namespace
{
	auto r = Register<FloatToVolts>::withId(L"mxSoloMute4");
}

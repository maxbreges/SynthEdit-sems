#include "mp_sdk_audio.h"

using namespace gmpi;

class Solomute final : public MpBase2
{

	FloatInPin pinChannelM1;
	FloatInPin pinChannelM2;
	FloatInPin pinChannelM3;
	FloatInPin pinChannelM4;
	FloatOutPin pinChannel1;
	FloatOutPin pinChannel2;
	FloatOutPin pinChannel3;
	FloatOutPin pinChannel4;

	AudioInPin pinInputA;
	AudioInPin pinInputA2;
	AudioInPin pinInputA3;
	AudioInPin pinInputA4;
	AudioOutPin pinSignalOut;
	AudioOutPin pinSignalOut2;
	AudioOutPin pinSignalOut3;
	AudioOutPin pinSignalOut4;

public:
	Solomute()
	{
		initializePin(pinChannelM1);
		initializePin(pinChannelM2);
		initializePin(pinChannelM3);
		initializePin(pinChannelM4);
		initializePin(pinChannel1);
		initializePin(pinChannel2);
		initializePin(pinChannel3);
		initializePin(pinChannel4);

		initializePin(pinInputA);
		initializePin(pinInputA2);
		initializePin(pinInputA3);
		initializePin(pinInputA4);
		initializePin(pinSignalOut);
		initializePin(pinSignalOut2);
		initializePin(pinSignalOut3);
		initializePin(pinSignalOut4);

	}

	void subProcess(int sampleFrames)
	{
		// get pointers to in/output buffers.
		auto inputA = getBuffer(pinInputA);
		auto inputA2 = getBuffer(pinInputA2);
		auto inputA3 = getBuffer(pinInputA3);
		auto inputA4 = getBuffer(pinInputA4);
		auto signalOut = getBuffer(pinSignalOut);
		auto signalOut2 = getBuffer(pinSignalOut2);
		auto signalOut3 = getBuffer(pinSignalOut3);
		auto signalOut4 = getBuffer(pinSignalOut4);

		for (int s = sampleFrames; s > 0; --s)
		{
			// TODO: Signal processing goes here.


			*signalOut = *inputA * pinChannelM1;

			*signalOut2 = *inputA2 * pinChannelM2;

			*signalOut3 = *inputA3 * pinChannelM3;

			*signalOut4 = *inputA4 * pinChannelM4;

			// Increment buffer pointers.
			++inputA;
			++inputA2;
			++inputA3;
			++inputA4;
			++signalOut;
			++signalOut2;
			++signalOut3;
			++signalOut4;
		}
	}

	void onSetPins() override
	{
		if (pinChannelM1.isUpdated())
		{
			pinChannel1 = pinChannelM1;
		}
		if (pinChannelM2.isUpdated())
		{
			pinChannel2 = pinChannelM2;
		}
		if (pinChannelM3.isUpdated())
		{
			pinChannel3 = pinChannelM3;
		}
		if (pinChannelM4.isUpdated())
		{
			pinChannel4 = pinChannelM4;
		}

		// Check which pins are updated.
		if (pinInputA.isStreaming())
		{
		}
		if (pinInputA2.isStreaming())
		{
		}
		if (pinInputA3.isStreaming())
		{
		}
		if (pinInputA4.isStreaming())
		{
		}

		// Set state of output audio pins.
		pinSignalOut.setStreaming(true);

		pinSignalOut2.setStreaming(true);

		pinSignalOut3.setStreaming(true);

		pinSignalOut4.setStreaming(true);


		// Set processing method.
		setSubProcess(&Solomute::subProcess);
	}
};

namespace
{
	auto r = Register<Solomute>::withId(L"mxSOLOMUTE");
}

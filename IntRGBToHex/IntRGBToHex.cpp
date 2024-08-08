#include "mp_sdk_audio.h"

using namespace gmpi;

class RGBToHex final : public MpBase2
{
	IntInPin pinAlpha;
	IntInPin pinR;
	IntInPin pinG;
	IntInPin pinB;

	IntOutPin pinRout;
	IntOutPin pinGout;
	IntOutPin pinBout;
	IntOutPin pinAlpha2Gui;	

public:
	RGBToHex()
	{	
		initializePin(pinAlpha);
		initializePin( pinR );
		initializePin( pinG );
		initializePin( pinB );	
		initializePin(pinRout);
		initializePin(pinGout);
		initializePin(pinBout);
		initializePin(pinAlpha2Gui);
	}

	void onSetPins() override
	{	
		if (pinAlpha.isUpdated())
		{
			pinAlpha2Gui = pinAlpha;
		}
		if( pinR.isUpdated() )
		{
			pinRout = pinR;
		}
		if( pinG.isUpdated() )
		{
			pinGout = pinG;
		}
		if( pinB.isUpdated() )
		{
			pinBout = pinB;
		}
	}
};

namespace
{
	auto r = Register<RGBToHex>::withId(L"IntRGB to Hex");
}

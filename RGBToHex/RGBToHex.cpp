#include "mp_sdk_audio.h"

using namespace gmpi;

class RGBToHex final : public MpBase2
{
	FloatInPin pinAlpha;
	FloatInPin pinR;
	FloatInPin pinG;
	FloatInPin pinB;

	FloatOutPin pinRout;
	FloatOutPin pinGout;
	FloatOutPin pinBout;
	FloatOutPin pinAlpha2Gui;	

public:
	RGBToHex()
	{
		initializePin(pinAlpha);
		initializePin(pinR);
		initializePin(pinG);
		initializePin(pinB);
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
	auto r = Register<RGBToHex>::withId(L"RGB to Hex");
}

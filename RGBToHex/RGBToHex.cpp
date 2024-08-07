#include "mp_sdk_audio.h"

using namespace gmpi;

class RGBToHex final : public MpBase2
{
	
	FloatInPin pinR;
	FloatInPin pinG;
	FloatInPin pinB;

	FloatOutPin pinRout;
	FloatOutPin pinGout;
	FloatOutPin pinBout;
	

public:
	RGBToHex()
	{
		initializePin( pinR );
		initializePin( pinG );
		initializePin( pinB );	
		initializePin(pinRout);
		initializePin(pinGout);
		initializePin(pinBout);
	}

	//void subProcess( int sampleFrames ){}


	void onSetPins() override
	{		
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
		pinRout.isUpdated();
		
		pinGout.isUpdated();
		
		pinBout.isUpdated();
		

		// Set processing method.
		//setSubProcess(&RGBToHex::subProcess);
	}
};

namespace
{
	auto r = Register<RGBToHex>::withId(L"RGB to Hex");
}

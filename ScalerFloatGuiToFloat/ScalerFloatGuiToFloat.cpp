#include "mp_sdk_audio.h"

using namespace gmpi;

class PatchMemoryFloat final : public MpBase2
{
	FloatInPin pinValueIn;
	FloatOutPin pinValueOut;

	FloatInPin pinMax;
	FloatInPin pinMin;

public:
	PatchMemoryFloat()
	{
		initializePin( pinValueIn );
		initializePin( pinValueOut );
		initializePin(pinMax);
		initializePin(pinMin);		
	}

	void onSetPins() override
	{
		// Check which pins are updated.
		if (pinMin.isUpdated())
		{
		}

		if (pinMax.isUpdated())
		{
		}

		if( pinValueIn.isUpdated() )
		{
			pinValueOut = (pinValueIn * (pinMax - pinMin)) + pinMin;			 
		}
	}
};

namespace
{
	auto r = Register<PatchMemoryFloat>::withId(L"PatchMemoryFloat");
}

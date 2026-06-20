#include "mp_sdk_audio.h"

using namespace gmpi;

class ValueBox final : public MpBase2
{
	FloatInPin pinValueIn;
	FloatOutPin pinFloatOut;
	IntOutPin pinIntOut;

public:
	ValueBox()
	{
		initializePin( pinValueIn );
		initializePin( pinFloatOut );
		initializePin( pinIntOut );
	}

	void onSetPins() override
	{
		// Check which pins are updated.
		if( pinValueIn.isUpdated() )
		{
		}
	}
};

namespace
{
	auto r = Register<ValueBox>::withId(L"My ValueBox");
}

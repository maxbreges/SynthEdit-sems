#include "mp_sdk_audio.h"

using namespace gmpi;

class Float2FloatGui final : public MpBase2
{
	FloatOutPin pinChannelM1;
	FloatInPin pinFloatIn;

public:
	Float2FloatGui()
	{
		initializePin( pinChannelM1 );
		initializePin( pinFloatIn );
	}

	void onSetPins() override
	{
		// Check which pins are updated.
		if( pinFloatIn.isUpdated() )
		{
			pinChannelM1 = pinFloatIn;
		}
	}
};

namespace
{
	auto r = Register<Float2FloatGui>::withId(L"Float2FloatGui");
}

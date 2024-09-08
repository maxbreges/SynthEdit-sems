#include "mp_sdk_audio.h"

using namespace gmpi;

class FloatGui2Float final : public MpBase2
{
	FloatInPin pinChannelM1;

	FloatOutPin pinChannel1;

public:
	FloatGui2Float()
	{
		initializePin( pinChannelM1 );

		initializePin( pinChannel1 );
	}	

	void onSetPins() override
	{
		// Check which pins are updated.
		if( pinChannelM1.isUpdated() )
		{
			pinChannel1 = pinChannelM1;
		}
	}
};

namespace
{
	auto r = Register<FloatGui2Float>::withId(L"FloatGui2Float");
}

#include "mp_sdk_audio.h"

using namespace gmpi;

class GuiTextToText final : public MpBase2
{
	StringInPin pinValueIn;
	StringOutPin pinValueOut;

public:
	GuiTextToText()
	{
		initializePin( pinValueIn );
		initializePin( pinValueOut );
	}

	void onSetPins() override
	{
		// Check which pins are updated.
		if( pinValueIn.isUpdated() )
		{
			pinValueOut = pinValueIn;
		}
	}
};

namespace
{
	auto r = Register<GuiTextToText>::withId(L"GuiTextToText");
}

#include "mp_sdk_audio.h"

using namespace gmpi;

class onSetTextVal final : public MpBase2
{
	bool isInitialized = false;

	StringInPin pinString;
	BoolOutPin pinBool;

public:
	onSetTextVal()
	{
		initializePin( pinString );
		initializePin( pinBool );		
	}

	void onSetPins() override
	{
		// Check which pins are updated.
		if( (pinString.isUpdated() && isInitialized ) )
		{
			pinBool.setValue(true, getBlockPosition());
			pinBool.setValue(false, getBlockPosition()+45);
		}
		isInitialized = true;
	}
};

namespace
{
	auto r = Register<onSetTextVal>::withId(L"My onSetTextVal");
}

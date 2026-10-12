#include "mp_sdk_audio.h"

using namespace gmpi;

class onSetTextVal final : public MpBase2
{
	bool isInitialized = false;
	bool guiOpen = false;

	StringInPin pinString;
	BoolOutPin pinBool;
	BoolOutPin pinToParam;

public:
	onSetTextVal()
	{
		initializePin( pinString );
		initializePin( pinBool );
		initializePin(pinToParam);
	}

	void onSetPins() override
	{
		if (isInitialized)
		{
			pinToParam.setValue(true, getBlockPosition());

			if (guiOpen)
			{
				if (pinString.isUpdated())
				{
					pinBool.setValue(true, getBlockPosition());
					pinBool.setValue(false, getBlockPosition() + 45);
				}
			}
		}

		isInitialized = true;
	}

	int32_t receiveMessageFromGui(int32_t id, int32_t size, void* msg)
	{
		if (id=2232322 && msg && size >= sizeof(int32_t))
		{
			auto value = *static_cast<const int32_t*>(msg);
			guiOpen = value;

			if (value > 1)
			{
				onSetPins();
			}			
		}

		return gmpi::MP_OK;
	}

};

namespace
{
	auto r = Register<onSetTextVal>::withId(L"My onSetTextVal");
}

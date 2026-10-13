#include "mp_sdk_audio.h"

using namespace gmpi;

class onSetTextVal final : public MpBase2
{
	bool isInitialized = false;
	bool guiOpen = false;
	bool folderChanged = false;

	BoolOutPin pinToParam;
	StringInPin pinString;
	BoolOutPin pinBool;
	//BoolOutPin pinBoolRef;//debug


public:
	onSetTextVal()
	{
		initializePin(pinToParam);
		initializePin( pinString );
		initializePin( pinBool );
		//initializePin(pinBoolRef);//debug
	}

	void onSetPins() override
	{
		//pinBoolRef.setValue(true, getBlockPosition());//debug
		//pinBoolRef.setValue(false, getBlockPosition() + 45);//debug

		if (isInitialized)
		{
			pinToParam.setValue(true, getBlockPosition());

			if (guiOpen)
			{
				if (pinString.isUpdated() && (!folderChanged))
				{
					pinBool.setValue(true, getBlockPosition());
					pinBool.setValue(false, getBlockPosition() + 45);
				}
				if (folderChanged)
				{
					folderChanged = false;
				}
			}
		}
		isInitialized = true;
	}

	int32_t receiveMessageFromGui(int32_t id, int32_t size, void* msg)
	{
		if (id==2232322 && msg && size >= sizeof(int32_t))
		{
			auto value = *static_cast<const int32_t*>(msg);

			guiOpen = value;

			if (value >= 1)
			{
				onSetPins();
			}			
		}
		if (id == 2232323)
		{
			folderChanged = true;
		}
		return gmpi::MP_OK;
	}
};

namespace
{
	auto r = Register<onSetTextVal>::withId(L"My onSetTextVal");
}

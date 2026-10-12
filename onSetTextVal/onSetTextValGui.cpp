#include "mp_sdk_gui2.h"

using namespace gmpi;

class GUI_open_countGui final : public SeGuiInvisibleBase
{
	// Unique ID for each instance
	uint64_t instanceId = {};

	// Static counter to generate unique IDs
	static uint64_t nextInstanceId;

 	void onSetBoolVal()
	{
		if (pinBoolVal)
		{
			retrievePluginID(static_cast<int32_t>(pinBoolVal));
		}
		else{}
	}

	BoolGuiPin pinBoolVal;

public:
	GUI_open_countGui()
	{
		initializePin(pinBoolVal, static_cast<MpGuiBaseMemberPtr2>(&GUI_open_countGui::onSetBoolVal) );
		instanceId = ++nextInstanceId;	
	}

	int32_t retrievePluginID(int value)
	{
		auto handle = getHost()->getHandle(value);
		
		if (instanceId >= 1)
		{
			getHost()->sendMessageToAudio(2232322, sizeof(instanceId), &instanceId);
		}
		
		return handle; // or return value, depending on intent
	}
};

uint64_t GUI_open_countGui::nextInstanceId = 0;

namespace
{
	auto r = Register<GUI_open_countGui>::withId(L"My onSetTextVal");
}
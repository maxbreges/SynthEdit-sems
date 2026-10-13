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
	BoolGuiPin pinFolderChanged;

public:
	GUI_open_countGui()
	{
		initializePin(pinBoolVal, static_cast<MpGuiBaseMemberPtr2>(&GUI_open_countGui::onSetBoolVal) );
		initializePin(pinFolderChanged, static_cast<MpGuiBaseMemberPtr2>(&GUI_open_countGui::onSetFolderChanged));
		instanceId = ++nextInstanceId;	
	}

	void onSetFolderChanged()
	{
		bool folderChanged = pinFolderChanged;
		getHost()->sendMessageToAudio(2232323, sizeof(folderChanged), &folderChanged);
	}

	int32_t retrievePluginID(int value)
	{		
		if (instanceId >= 1)
		{
			getHost()->sendMessageToAudio(2232322, sizeof(instanceId), &instanceId);
		}		
		return value; 
	}
};

uint64_t GUI_open_countGui::nextInstanceId = 0;

namespace
{
	auto r = Register<GUI_open_countGui>::withId(L"My onSetTextVal");
}
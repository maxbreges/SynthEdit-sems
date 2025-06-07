#include "MidiLearnGui.h"
#include "mp_sdk_gui2.h"

using namespace gmpi;

REGISTER_GUI_PLUGIN(MidiLearnGui, L"MidiLearn");

MidiLearnGui::MidiLearnGui(IMpUnknown* host) : MpGuiBase(host)
, MESSAGE_ID_GATE(22323), MESSAGE_ID_NOTE(22324)

{
	initializePin(pinAnimationPosition, static_cast<MpGuiBaseMemberPtr>(&MidiLearnGui::onSetAnimationPosition));
	initializePin(pinNote, static_cast<MpGuiBaseMemberPtr>(&MidiLearnGui::onSetAnimationPosition));
	initializePin(pinNoteToDsp, static_cast<MpGuiBaseMemberPtr>(&MidiLearnGui::onSetAnimationPosition));
}
void MidiLearnGui::onSetAnimationPosition()
{
	if (pinAnimationPosition)
	{
		int messageId = 22322;
		int myData = pinAnimationPosition.getValue() ? 1 : 0;
		int totalDataBytes = sizeof(myData);
		getHost()->sendMessageToAudio(messageId, totalDataBytes, &myData);
	}
}

int32_t MidiLearnGui::receiveMessageFromAudio(int32_t id, int32_t size, void* messageData)
{
	if (id == MESSAGE_ID_GATE && size >= sizeof(bool))
	{
		pinAnimationPosition = 0;
	}

	if (id == MESSAGE_ID_NOTE && size >= sizeof(int))
	{
		int receivedValue = *(reinterpret_cast<int*>(messageData));
		// Update your GUI pin or indicator here
		pinNote = receivedValue;
		pinNoteToDsp = receivedValue;// 
	}
	return gmpi::MP_OK;
}
;
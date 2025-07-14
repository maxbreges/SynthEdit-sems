#include "MidiLearnGui.h"
#include "mp_sdk_gui2.h"
#include <sstream>

using namespace gmpi;

REGISTER_GUI_PLUGIN(MidiLearnGui, L"MidiLearn");

MidiLearnGui::MidiLearnGui(IMpUnknown* host) : MpGuiBase(host)
, MESSAGE_ID_GATE(22323), MESSAGE_ID_NOTE(22324)

{
	initializePin(pinAnimationPosition, static_cast<MpGuiBaseMemberPtr>(&MidiLearnGui::onSetAnimationPosition));
	initializePin(pinNote, static_cast<MpGuiBaseMemberPtr>(&MidiLearnGui::onPatchChange));
	initializePin(pinNoteToDsp, static_cast<MpGuiBaseMemberPtr>(&MidiLearnGui::onSetAnimationPosition));
	initializePin(pinNoteName, static_cast<MpGuiBaseMemberPtr>(&MidiLearnGui::onPatchChange));
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

void MidiLearnGui::onPatchChange()
{
	pinNoteToDsp = pinNote.getValue();
	int mi = pinNote.getValue();
	signed int oct_v = mi / 12 - 1;

	std::string nt3;
	std::string acc = "#";
	// with switch
	switch (mi % 12)
	{
	case 0: nt3 = "C"; break;
	case 1: nt3 = "C" + acc; break;
	case 2: nt3 = "D"; break;
	case 3: nt3 = "D" + acc; break;
	case 4: nt3 = "E"; break;
	case 5: nt3 = "F"; break;
	case 6: nt3 = "F" + acc; break;
	case 7: nt3 = "G"; break;
	case 8: nt3 = "G" + acc; break;
	case 9: nt3 = "A"; break;
	case 10: nt3 = "A" + acc; break;
	case 11: nt3 = "B"; break;
	}

	std::stringstream oct;
	oct << std::fixed << oct_v;
	std::string oct_(oct.str());

	pinNoteName = nt3 + oct_;
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
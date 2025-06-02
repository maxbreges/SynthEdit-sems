#include "MidiLearnGui.h"
#include "mp_sdk_gui2.h"

using namespace gmpi;

REGISTER_GUI_PLUGIN(MIDILearnDSPGui, L"MidiLearnDSP");

MIDILearnDSPGui::MIDILearnDSPGui(IMpUnknown* host) : MpGuiBase(host)
, MESSAGE_ID(2244), receivedValue(false)
{
	initializePin(pinAnimationPosition, static_cast<MpGuiBaseMemberPtr>(&MIDILearnDSPGui::onSetAnimationPosition));
	initializePin(pinGate, static_cast<MpGuiBaseMemberPtr>(&MIDILearnDSPGui::onSetAnimationPosition));
	initializePin(pinGateIn, static_cast<MpGuiBaseMemberPtr>(&MIDILearnDSPGui::onSetGateIn));

	initializePin(pinStored, static_cast<MpGuiBaseMemberPtr>(&MIDILearnDSPGui::onSetStored));
	initializePin(pinNote, static_cast<MpGuiBaseMemberPtr>(&MIDILearnDSPGui::onSetStored));
	onSetStored();
}

	void MIDILearnDSPGui::onSetAnimationPosition()
	{
		pinGate = pinAnimationPosition;
	}

	void MIDILearnDSPGui::onSetGateIn()
	{
		if (pinGateIn)
			pinAnimationPosition = false;
		onSetAnimationPosition();
	}	

	void MIDILearnDSPGui::onSetStored()
	{		
		pinStored = pinNote.getValue();
	}

	int32_t MIDILearnDSPGui::receiveMessageFromAudio(int32_t id, int32_t size, void* messageData)
	{
		if (id == MESSAGE_ID && size >= sizeof(int))
		{
			int receivedValue = *(reinterpret_cast<int*>(messageData));
			// Update your GUI pin or indicator here
			pinNote = receivedValue; // 
			pinStored = pinNote;
			//pinAnimationPosition = false;
		}
		return gmpi::MP_OK;
	}
;
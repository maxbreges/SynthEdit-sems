#pragma once

#include "mp_sdk_gui.h"

class MidiLearnGui : public MpGuiBase

{
public:
	MidiLearnGui(IMpUnknown* host);

	// overrides
	virtual int32_t MP_STDCALL receiveMessageFromAudio(int32_t id, int32_t size, void* messageData);
	void onSetAnimationPosition();

	FloatGuiPin pinAnimationPosition;
	IntGuiPin pinNote;
	IntGuiPin pinNoteToDsp;

	const int MESSAGE_ID_GATE;
	const int MESSAGE_ID_NOTE;
};

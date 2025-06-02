#pragma once

#include "mp_sdk_gui.h"

class MIDILearnDSPGui : public MpGuiBase

{
public:
	MIDILearnDSPGui(IMpUnknown* host);

	// overrides
	virtual int32_t MP_STDCALL receiveMessageFromAudio(int32_t id, int32_t size, void* messageData);
	void onSetAnimationPosition();
	void onSetGateIn();

	void onSetStored();

	FloatGuiPin pinAnimationPosition;
	BoolGuiPin pinGate;
	BoolGuiPin pinGateIn;

	IntGuiPin pinStored;
	IntGuiPin pinNote;

	const int MESSAGE_ID;
	int receivedValue;

};
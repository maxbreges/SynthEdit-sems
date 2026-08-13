#ifndef MIDICC_H_INCLUDED
#define MIDICC_H_INCLUDED

#include "../se_sdk3/mp_sdk_audio.h"

class MidiCc : public MpBase
{
	uint32_t currentCcValue = 8; // an out-of-band value to force initial update. (becuase floatToU32 duplicate lower 2 bits).

public:
	MidiCc(IMpUnknown* host);
	void subProcess(int bufferOffset, int sampleFrames);
	virtual void onSetPins() override;

private:
	BoolInPin pinGate;
	AudioInPin pinValue;
	IntInPin pinChannel;
	MidiOutPin pinMIDIOut;
	IntInPin pinMidiCc;

	void SendValue(int bufferPosition = -1);
	bool initState;
};

#endif


#include "MidiCc.h"
#include <algorithm>
#include "mp_midi.h"

REGISTER_PLUGIN(MidiCc, L"MIDI CC");
SE_DECLARE_INIT_STATIC_FILE(MidiCc);

MidiCc::MidiCc(IMpUnknown* host) : MpBase(host),
initState(false)
{
	// Register pins.
	initializePin(0, pinGate);
	initializePin(1, pinValue);
	initializePin(2, pinChannel);
	initializePin(3, pinMidiCc);
	initializePin(4, pinMIDIOut);
}

void MidiCc::subProcess(int bufferOffset, int sampleFrames)
{
	// get pointers to in/output buffers.
	float* val = bufferOffset + pinValue.getBuffer();

	int bufferPos = bufferOffset + sampleFrames - 1;
	SendValue(bufferPos);
}

void MidiCc::onSetPins()
{
	if (pinGate.isUpdated())
	{	
		if (pinGate)
		{initState = true;}
	}

	if (pinChannel.isUpdated() || pinMidiCc.isUpdated())
	{
		currentCcValue = 8;
		SendValue();
	}

	// Check which pins are updated.
	if (pinValue.isStreaming())
	{
		// Set processing method.
		SET_PROCESS(&MidiCc::subProcess);
	}
	else
	{
		SET_PROCESS(&MidiCc::subProcessNothing);
		SendValue();
	}
}

void MidiCc::SendValue(int bufferPosition)
{
	if (!initState)
	{
		return;
	}
	const float v = pinValue.getValue(bufferPosition);
#if 0
	int newMidiValue = (int)(v * 127.0f);
	newMidiValue = (std::min)(newMidiValue, 127);
	newMidiValue = (std::max)(newMidiValue, 0);

	if (!pinGate && (newMidiValue == currentCcValue))
	{
		return;
	}

	currentCcValue = newMidiValue;

	const int size = 3;
	unsigned char data[3];

	data[0] = 0xB0; // Control Change.
	data[1] = pinMidiCc;
	data[2] = currentCcValue;

	pinMIDIOut.send(data, size, bufferPosition);
#else

	const uint32_t newMidiValue = gmpi::midi::utils::floatToU32(v);

	if (!pinGate && (newMidiValue == currentCcValue))
	{
		return;
	}

	currentCcValue = newMidiValue;

	// MIDI 2.0
	const auto msgout = gmpi::midi_2_0::makeController(
		pinMidiCc.getValue(),
		v,
		pinChannel.getValue()
	);

	pinMIDIOut.send(
		msgout.m,
		static_cast<int>(std::size(msgout.m)),
		bufferPosition
	);
#endif
	initState = true;
}

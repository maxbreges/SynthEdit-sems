#include "mp_sdk_audio.h"
#include "mp_midi.h"

using namespace gmpi;

class MIDIDelta final : public MpBase2
{
	MidiInPin pinMIDIIn;
	IntOutPin pinDelta;

	gmpi::midi_2_0::MidiConverter2 midiConverter;

	int NoteOn = 0;
	int NoteOff = 0;
	int lastNote = 0;

public:
	MIDIDelta() :
		// init the midi converter
		midiConverter(
			// provide a lambda to accept converted MIDI 2.0 messages
			[this](const midi::message_view& msg, int offset)
			{
				onMidi2Message(msg);
			}
		)
	{
		initializePin( pinMIDIIn );
		initializePin( pinDelta );
	}

	void onMidiMessage(int pin, unsigned char* midiMessage, int size) override
	{
		midi::message_view msg((const uint8_t*)midiMessage, size);

		// convert everything to MIDI 2.0
		midiConverter.processMidi(msg, -1);
	}

	// put your midi handling code in here.
	void onMidi2Message(const midi::message_view& msg)
	{
		const auto header = gmpi::midi_2_0::decodeHeader(msg);

		// only 8-byte messages supported.
		if (header.messageType != gmpi::midi_2_0::ChannelVoice64)
			return;

		switch (header.status)
		{

		case gmpi::midi_2_0::NoteOn:
		{
			const auto note = gmpi::midi_2_0::decodeNote(msg);
			NoteOn = (int)(uint8_t)note.noteNumber;
			if (NoteOn > NoteOff)
			{
				pinDelta = 1;
				lastNote = pinDelta;
			}
			if (NoteOn < NoteOff)
			{
				pinDelta = -1;
				lastNote = pinDelta;
			}
			if (NoteOn == NoteOff)
			{
				pinDelta = lastNote;
			}
		}
		break;

		case gmpi::midi_2_0::NoteOff:
		{
			const auto note = gmpi::midi_2_0::decodeNote(msg);
			NoteOff = (int)(uint8_t)note.noteNumber;
	
		}
		break;

		default:
			break;
		};
	}

	void onSetPins() override
	{
	}
};

namespace
{
	auto r = Register<MIDIDelta>::withId(L"MIDIDelta");
}

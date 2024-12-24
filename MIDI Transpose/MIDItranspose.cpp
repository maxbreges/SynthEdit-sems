#include "mp_sdk_audio.h"
#include "mp_midi.h"
#include <vector>

SE_DECLARE_INIT_STATIC_FILE(MIDITranspose);

#define NOTE_ON     0x90
#define NOTE_OFF    0x80

using namespace gmpi;
using namespace GmpiMidi;
using namespace midi_2_0;

class MIDItranspose final : public MpBase2
{
	MidiInPin pinMIDIIn;
	EnumInPin pinOctave;
	EnumInPin pinSemi;
	MidiOutPin pinMIDIOut;

	int messageSize = 0;
	int chan = 0;
	int Note = 0;
	float Velocity = 0.f;

	struct SimpleMidiMessage
	{
		unsigned char data[3];
	};

	std::vector< SimpleMidiMessage > buffer;

	gmpi::midi_2_0::MidiConverter2 midiConverter;

public:
	MIDItranspose() :
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
		initializePin(pinOctave);
		initializePin(pinSemi);
		initializePin( pinMIDIOut );
	}

	int32_t open() override
	{
		MpBase2::open();    // always call the base class
		return gmpi::MP_OK;
	}

	// passes all MIDI to the converter.
	void onMidiMessage(int pin, unsigned char* midiMessage, int size) override
	{
		midi::message_view msg((const uint8_t*)midiMessage, size);

		// convert everything to MIDI 2.0
		midiConverter.processMidi(msg, -1);
	}

	void onMidi2Message(const midi::message_view& msg)
	{
		const auto header = gmpi::midi_2_0::decodeHeader(msg);

		//messageSize = (int)(size_t)msg.size();
		// only 8-byte messages supported. only 16 channels supported
		
		const auto note = gmpi::midi_2_0::decodeNote(msg);
		chan = header.channel;
		Note = (int)(uint8_t)note.noteNumber;
		Velocity = (float)(255.f * note.velocity);

		if (header.messageType != gmpi::midi_2_0::ChannelVoice64)
			return;
		
		switch (pinOctave)
		{
		case 0: Note = Note - 24;  break;
		case 1: Note = Note -12; break;
		case 2: Note = Note; break;
		case 3: Note = Note + 12; break;
		case 4: Note = Note + 24; break;
		case 5: Note = Note + 36; break;
		case 6: Note = Note + 48; break;
		}

		switch (pinSemi)
		{
		case 0: Note = Note - 11;  break;
		case 1: Note = Note - 10; break;
		case 2: Note = Note - 9; break;
		case 3: Note = Note - 8; break;
		case 4: Note = Note - 7; break;
		case 5: Note = Note - 6; break;
		case 6: Note = Note - 5 ; break;
		case 7: Note = Note - 4; break;
		case 8: Note = Note - 3; break;
		case 9: Note = Note - 2; break;
		case 10: Note = Note - 1; break;
		case 11: Note = Note; break;
		case 12: Note = Note + 1; break;
		case 13: Note = Note + 2; break;
		case 14: Note = Note + 3; break;
		case 15: Note = Note + 4; break;
		case 16: Note = Note + 5; break;
		case 17: Note = Note + 6; break;
		case 18: Note = Note + 7; break;
		case 19: Note = Note + 8; break;
		case 20: Note = Note + 9; break;
		case 21: Note = Note + 10; break;
		case 22: Note = Note + 11; break;
		}

		switch (header.status)
		{
		case gmpi::midi_2_0::NoteOn:
		{
			const auto out = gmpi::midi_2_0::makeNoteOnMessage
			(
				Note,
				Velocity,
				chan
			);
			pinMIDIOut.send(out.m);
		} break;

		case gmpi::midi_2_0::NoteOff:
		{
			const auto out = gmpi::midi_2_0::makeNoteOffMessage
			(
				Note,
				Velocity,
				chan
			);
			pinMIDIOut.send(out.m);
		}break;

		}

	};

	void onSetPins() override
	{
	}
};

namespace
{
	auto r = Register<MIDItranspose>::withId(L"MIDItranspose");
}

#include "mp_sdk_audio.h"
#include "../se_sdk3/mp_midi.h"

using namespace gmpi;

class MidiChannelShift final : public MpBase2
{
	int messageSize = 0;
	int chan = 0;
	int Note = 0;
	float Velocity = 0.f;
	int Add = 0;

	struct SimpleMidiMessage
	{
		unsigned char data[3];
	};

	std::vector< SimpleMidiMessage > buffer;

	MidiInPin pinMIDIIn;
	IntInPin pinAdd;
	MidiOutPin pinMIDIOut;

	gmpi::midi_2_0::MidiConverter2 midiConverter;

public:
	MidiChannelShift() :
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
		initializePin( pinAdd );
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

	// put your midi handling code in here.
	void onMidi2Message(const midi::message_view& msg)
	{
		const auto header = gmpi::midi_2_0::decodeHeader(msg);

		// only 8-byte messages supported. only 16 channels supported
		if (header.messageType != gmpi::midi_2_0::ChannelVoice64)
			return;
		
		const auto note = gmpi::midi_2_0::decodeNote(msg);
		chan = header.channel;
		Note = (int)(uint8_t)note.noteNumber;
		Velocity = (float)(255.f * note.velocity);
		Add = pinAdd;

		switch (header.status)
		{
		case gmpi::midi_2_0::NoteOn:
		{
			const auto out = gmpi::midi_2_0::makeNoteOnMessage
			(
				Note,
				Velocity,
				chan + Add
			);
			pinMIDIOut.send(out.m);
		} break;

		case gmpi::midi_2_0::NoteOff:
		{
			const auto out = gmpi::midi_2_0::makeNoteOffMessage
			(
				Note,
				Velocity,
				chan+Add
			);
			pinMIDIOut.send(out.m);
		}break;

		}
		
	}

	void onSetPins() override
	{
		// Check which pins are updated.

		if( pinAdd.isUpdated() )
		{
		}
	}
};

namespace
{
	auto r = Register<MidiChannelShift>::withId(L"MidiChannelShift");
}

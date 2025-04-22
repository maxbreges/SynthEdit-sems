#include "mp_sdk_audio.h"
#include "mp_midi.h"

using namespace gmpi;
using namespace GmpiMidi;
using namespace midi_2_0;

class MidiToHex final : public MpBase2
{
	MidiInPin pinMIDIIn;
	IntInPin pinChannel;
	IntOutPin pinPitch;
	BoolOutPin pinGate;

	gmpi::midi_2_0::MidiConverter2 midiConverter;

public:
	MidiToHex() :
		midiConverter(
			// provide a lambda to accept converted MIDI 2.0 messages
			[this](const midi::message_view& msg, int offset)
			{
				onMidi2Message(msg);
			}
		)
	{
		initializePin( pinMIDIIn );
		initializePin( pinChannel );
		initializePin(pinPitch);
		initializePin(pinGate);
	}

	int32_t open() override
	{
		MpBase2::open();	// always call the base class

		return gmpi::MP_OK;
	}

	// passes all MIDI to the converter.
	void onMidiMessage(int pin, unsigned char* midiMessage, int size) override
	{
		midi::message_view msg((const uint8_t*)midiMessage, size);

		// convert everything to MIDI 2.0
		midiConverter.processMidi(msg, -1);
	}

/*	void onMidiMessage(int pin, unsigned char* midiMessage, int size) override
	{
		midiConverter.processMidi(midi::message_view((const uint8_t*)midiMessage, size), -1);
	}*/

	void onMidi2Message(const midi::message_view& msg)
	{
		const auto header = gmpi::midi_2_0::decodeHeader(msg);
		int chan = static_cast<uint8_t>(msg[1] & 0x0f); // Extract the channel number
		int noteNumber = static_cast<int>(gmpi::midi_2_0::decodeNote(msg).noteNumber);

		if (header.messageType != gmpi::midi_2_0::ChannelVoice64) // Support only Channel Voice messages
			return;
		if (chan == pinChannel.getValue() || (pinChannel == -1)) // Check against pinChannel's current value
		{
			switch (header.status) // Check the type of MIDI message
			{
			case gmpi::midi_2_0::NoteOn:
			
				pinPitch = noteNumber;
				pinGate = true;
			
			break;

			case gmpi::midi_2_0::NoteOff:
			
				pinGate = false;
			
				break;
			}
		}
	}
};

namespace
{
	auto r = Register<MidiToHex>::withId(L"MidiToHex");
}

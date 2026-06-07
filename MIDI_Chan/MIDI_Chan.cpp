#include "mp_sdk_audio.h"
#include "mp_midi.h"

using namespace gmpi;

class MIDI_Chan final : public MpBase2
{
	MidiInPin pinMIDIIn;
	IntOutPin pinChannel;

	int chan = 0;

	gmpi::midi_2_0::MidiConverter2 midiConverter;

public:
	MIDI_Chan():
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

		// only 8-byte messages supported. only 16 channels supported
		if (header.messageType != gmpi::midi_2_0::ChannelVoice64)
			return;

		chan = header.channel;
		pinChannel = chan + 1;
	}
};

namespace
{
	auto r = Register<MIDI_Chan>::withId(L"My MIDI_Chan");
}

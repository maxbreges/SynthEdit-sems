#include "mp_sdk_audio.h"
#include "mp_midi.h"
#include <array>
//#include <climits>

SE_DECLARE_INIT_STATIC_FILE(mxPitchDisplay);

#define NOTE_ON     0x90
#define NOTE_OFF    0x80

using namespace gmpi;
using namespace GmpiMidi;
using namespace midi_2_0;

class PitchDisplay final : public MpBase2
{
	MidiInPin pinMIDIIn;
	IntOutPin pinNote; //parameterId=0 sends Note Number to gui
	BoolOutPin pinNoteOff; //parameterId=1 sends true when NoteOff to gui
	BoolInPin pinTransportRun;
	IntInPin pinChannel; //if channel is not the same then Reset

	gmpi::midi_2_0::MidiConverter2 midiConverter;

	int messageSize = 0;
	int chan = 0;

public:
	PitchDisplay() :
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
		initializePin( pinNote );
		initializePin(pinNoteOff);
		initializePin(pinTransportRun);
		initializePin(pinChannel);
	}

	/*int32_t open() override
	{
		MpBase2::open();    // always call the base class
		return gmpi::MP_OK;
	}*/

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
	
		pinNoteOff = true;

		const auto header = gmpi::midi_2_0::decodeHeader(msg);

		messageSize = (int)(size_t)msg.size();
		// only 8-byte messages supported. only 16 channels supported

		chan = static_cast<uint8_t>(msg[1] & 0x0f);

		if (header.messageType != gmpi::midi_2_0::ChannelVoice64)
			return;
		if (chan == (pinChannel-1))
		{
			switch (header.status)
			{
			case gmpi::midi_2_0::NoteOn:
			{
				const auto note = gmpi::midi_2_0::decodeNote(msg);
				pinNote = (int)(uint8_t)note.noteNumber;
				pinNoteOff = false;
			}
			break;

			case gmpi::midi_2_0::NoteOff:
			{
				const auto note = gmpi::midi_2_0::decodeNote(msg);
				pinNote = (int)(uint8_t)note.noteNumber;

				if (!pinTransportRun)
				{

					pinNoteOff = false;
				}
			}
			break;
			}
		}
	}

	void onSetPins() override
	{
		// Check which pins are updated.

		pinTransportRun.isUpdated();
		{
			if (pinTransportRun)
			{
				pinNoteOff = true;
			}
		}
	}
};

namespace
{
	auto r = Register<PitchDisplay>::withId(L"NoteName");
}

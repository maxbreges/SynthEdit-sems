#include "mp_sdk_audio.h"
#include "mp_midi.h"

SE_DECLARE_INIT_STATIC_FILE(MIDIToInt);

#define NOTE_ON     0x90
#define NOTE_OFF    0x80

using namespace gmpi;
using namespace GmpiMidi;
using namespace midi_2_0;

class MIDI2Fret final : public MpBase2
{
	MidiInPin pinMIDIIn;
	EnumInPin pinString;
	BoolOutPin pin1;
	BoolOutPin pin2;
	BoolOutPin pin3;
	BoolOutPin pin4;
	BoolOutPin pin5;
	BoolOutPin pin6;
	BoolOutPin pin7;
	BoolOutPin pin8;
	BoolOutPin pin9;
	BoolOutPin pin10;
	BoolOutPin pin11;
	BoolOutPin pin12;
	BoolOutPin pin13;
	BoolOutPin pin14;
	BoolOutPin pin15;
	BoolOutPin pin16;
	BoolOutPin pin17;
	BoolOutPin pin18;
	BoolOutPin pin19;
	BoolOutPin pin20;
	BoolInPin pinTransportRun;
	EnumInPin pinChannel;

	gmpi::midi_2_0::MidiConverter2 midiConverter;

	int messageSize = 0;

public:
	MIDI2Fret() :
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
		initializePin(pinString);
		initializePin( pin1 );
		initializePin( pin2 );
		initializePin(pin3);
		initializePin(pin4);
		initializePin(pin5);
		initializePin(pin6);
		initializePin(pin7);
		initializePin(pin8);
		initializePin(pin9);
		initializePin(pin10);
		initializePin(pin11);
		initializePin(pin12);
		initializePin(pin13);
		initializePin(pin14);
		initializePin(pin15);
		initializePin(pin16);
		initializePin(pin17);
		initializePin(pin18);
		initializePin(pin19);
		initializePin(pin20);
		initializePin(pinTransportRun);
		initializePin(pinChannel);
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
		int chan = static_cast<uint8_t>(msg[1] & 0x0f);

		messageSize = (int)(size_t)msg.size();
		// only 8-byte messages supported. only 16 channels supported
		if (header.messageType != gmpi::midi_2_0::ChannelVoice64)
			return;
		//=====================================

		if (pinChannel == -1 || chan == pinChannel)
		{
			if (pinString == 0)
			{
				if (!pinTransportRun)
				{
					int NoteS = 0;

					switch (header.status)
					{
					case gmpi::midi_2_0::NoteOn:
					{
						const auto note = gmpi::midi_2_0::decodeNote(msg);
						int Note = (int)(uint8_t)note.noteNumber;

						NoteS = Note;

						if (Note == NoteS)
						{
							pin1 = false;
							pin2 = false;
							pin3 = false;
							pin4 = false;
							pin5 = false;
							pin6 = false;
							pin7 = false;
							pin8 = false;
							pin9 = false;
							pin10 = false;
							pin11 = false;
							pin12 = false;
							pin13 = false;
							pin14 = false;
							pin15 = false;
							pin16 = false;
							pin17 = false;
							pin18 = false;
							pin19 = false;
							pin20 = false;

							switch (Note)
							{
							case 43: pin1 = true; break;
							case 44: pin2 = true; break;
							case 45: pin3 = true; break;
							case 46: pin4 = true; break;
							case 47: pin5 = true; break;
							case 48: pin6 = true; break;
							case 49: pin7 = true; break;
							case 50: pin8 = true; break;
							case 51: pin9 = true; break;
							case 52: pin10 = true; break;
							case 53: pin11 = true; break;
							case 54: pin12 = true; break;
							case 55: pin13 = true; break;
							case 56: pin14 = true; break;
							case 57: pin15 = true; break;
							case 58: pin16 = true; break;
							case 59: pin17 = true; break;
							case 60: pin18 = true; break;
							case 61: pin19 = true; break;
							case 62: pin20 = true; break;
							}
						}

					}
					break;
					}
				}
				//=========================================
				if (pinTransportRun)
				{
					switch (header.status)
					{
					case gmpi::midi_2_0::NoteOn:
					{
						const auto note = gmpi::midi_2_0::decodeNote(msg);
						int Note = (int)(uint8_t)note.noteNumber;

						switch (Note)
						{
						case 43: pin1 = true; break;
						case 44: pin2 = true; break;
						case 45: pin3 = true; break;
						case 46: pin4 = true; break;
						case 47: pin5 = true; break;
						case 48: pin6 = true; break;
						case 49: pin7 = true; break;
						case 50: pin8 = true; break;
						case 51: pin9 = true; break;
						case 52: pin10 = true; break;
						case 53: pin11 = true; break;
						case 54: pin12 = true; break;
						case 55: pin13 = true; break;
						case 56: pin14 = true; break;
						case 57: pin15 = true; break;
						case 58: pin16 = true; break;
						case 59: pin17 = true; break;
						case 60: pin18 = true; break;
						case 61: pin19 = true; break;
						case 62: pin20 = true; break;
						}
					}
					break;

					case gmpi::midi_2_0::NoteOff:
					{
						const auto note = gmpi::midi_2_0::decodeNote(msg);
						int Note = (int)(uint8_t)note.noteNumber;

						switch (Note)
						{
						case 43: pin1 = false; break;
						case 44: pin2 = false; break;
						case 45: pin3 = false; break;
						case 46: pin4 = false; break;
						case 47: pin5 = false; break;
						case 48: pin6 = false; break;
						case 49: pin7 = false; break;
						case 50: pin8 = false; break;
						case 51: pin9 = false; break;
						case 52: pin10 = false; break;
						case 53: pin11 = false; break;
						case 54: pin12 = false; break;
						case 55: pin13 = false; break;
						case 56: pin14 = false; break;
						case 57: pin15 = false; break;
						case 58: pin16 = false; break;
						case 59: pin17 = false; break;
						case 60: pin18 = false; break;
						case 61: pin19 = false; break;
						case 62: pin20 = false; break;
						}
					}
					break;
					}

				}
			}

			//====================================
			if (pinString == 1)
			{
				if (!pinTransportRun)
				{
					int NoteS = 0;

					switch (header.status)
					{
					case gmpi::midi_2_0::NoteOn:
					{
						const auto note = gmpi::midi_2_0::decodeNote(msg);
						int Note = (int)(uint8_t)note.noteNumber;

						NoteS = Note;

						if (Note == NoteS)
						{
							pin1 = false;
							pin2 = false;
							pin3 = false;
							pin4 = false;
							pin5 = false;
							pin6 = false;
							pin7 = false;
							pin8 = false;
							pin9 = false;
							pin10 = false;
							pin11 = false;
							pin12 = false;
							pin13 = false;
							pin14 = false;
							pin15 = false;
							pin16 = false;
							pin17 = false;
							pin18 = false;
							pin19 = false;
							pin20 = false;

							switch (Note)
							{
							case 38: pin1 = true; break;
							case 39: pin2 = true; break;
							case 40: pin3 = true; break;
							case 41: pin4 = true; break;
							case 42: pin5 = true; break;
							case 43: pin6 = true; break;
							case 44: pin7 = true; break;
							case 45: pin8 = true; break;
							case 46: pin9 = true; break;
							case 47: pin10 = true; break;
							case 48: pin11 = true; break;
							case 49: pin12 = true; break;
							case 50: pin13 = true; break;
							case 51: pin14 = true; break;
							case 52: pin15 = true; break;
							case 53: pin16 = true; break;
							case 54: pin17 = true; break;
							case 55: pin18 = true; break;
							case 56: pin19 = true; break;
							case 57: pin20 = true; break;
							}
						}
						break;
					}
					}
				}

				if (pinTransportRun)
				{
					switch (header.status)
					{
					case gmpi::midi_2_0::NoteOn:
					{
						const auto note = gmpi::midi_2_0::decodeNote(msg);
						int Note = (int)(uint8_t)note.noteNumber;

						switch (Note)
						{
						case 38: pin1 = true; break;
						case 39: pin2 = true; break;
						case 40: pin3 = true; break;
						case 41: pin4 = true; break;
						case 42: pin5 = true; break;
						case 43: pin6 = true; break;
						case 44: pin7 = true; break;
						case 45: pin8 = true; break;
						case 46: pin9 = true; break;
						case 47: pin10 = true; break;
						case 48: pin11 = true; break;
						case 49: pin12 = true; break;
						case 50: pin13 = true; break;
						case 51: pin14 = true; break;
						case 52: pin15 = true; break;
						case 53: pin16 = true; break;
						case 54: pin17 = true; break;
						case 55: pin18 = true; break;
						case 56: pin19 = true; break;
						case 57: pin20 = true; break;
						}
					}

					break;

					case gmpi::midi_2_0::NoteOff:
					{
						const auto note = gmpi::midi_2_0::decodeNote(msg);
						int Note = (int)(uint8_t)note.noteNumber;

						switch (Note)
						{
						case 38: pin1 = false; break;
						case 39: pin2 = false; break;
						case 40: pin3 = false; break;
						case 41: pin4 = false; break;
						case 42: pin5 = false; break;
						case 43: pin6 = false; break;
						case 44: pin7 = false; break;
						case 45: pin8 = false; break;
						case 46: pin9 = false; break;
						case 47: pin10 = false; break;
						case 48: pin11 = false; break;
						case 49: pin12 = false; break;
						case 50: pin13 = false; break;
						case 51: pin14 = false; break;
						case 52: pin15 = false; break;
						case 53: pin16 = false; break;
						case 54: pin17 = false; break;
						case 55: pin18 = false; break;
						case 56: pin19 = false; break;
						case 57: pin20 = false; break;
						}
					}
					break;

					}

				}

			}

			//====================================

			if (pinString == 2)
			{
				if (!pinTransportRun)
				{
					int NoteS = 0;

					switch (header.status)
					{
					case gmpi::midi_2_0::NoteOn:
					{
						const auto note = gmpi::midi_2_0::decodeNote(msg);
						int Note = (int)(uint8_t)note.noteNumber;

						NoteS = Note;

						if (Note == NoteS)
						{
							pin1 = false;
							pin2 = false;
							pin3 = false;
							pin4 = false;
							pin5 = false;
							pin6 = false;
							pin7 = false;
							pin8 = false;
							pin9 = false;
							pin10 = false;
							pin11 = false;
							pin12 = false;
							pin13 = false;
							pin14 = false;
							pin15 = false;
							pin16 = false;
							pin17 = false;
							pin18 = false;
							pin19 = false;
							pin20 = false;

							switch (Note)
							{
							case 33: pin1 = true; break;
							case 34: pin2 = true; break;
							case 35: pin3 = true; break;
							case 36: pin4 = true; break;
							case 37: pin5 = true; break;
							case 38: pin6 = true; break;
							case 39: pin7 = true; break;
							case 40: pin8 = true; break;
							case 41: pin9 = true; break;
							case 42: pin10 = true; break;
							case 43: pin11 = true; break;
							case 44: pin12 = true; break;
							case 45: pin13 = true; break;
							case 46: pin14 = true; break;
							case 47: pin15 = true; break;
							case 48: pin16 = true; break;
							case 49: pin17 = true; break;
							case 50: pin18 = true; break;
							case 51: pin19 = true; break;
							case 52: pin20 = true; break;
							}
						}

					}
					break;
					}
				}
				//=========================================
				if (pinTransportRun)
				{
					switch (header.status)
					{
					case gmpi::midi_2_0::NoteOn:
					{
						const auto note = gmpi::midi_2_0::decodeNote(msg);
						int Note = (int)(uint8_t)note.noteNumber;

						switch (Note)
						{
						case 33: pin1 = true; break;
						case 34: pin2 = true; break;
						case 35: pin3 = true; break;
						case 36: pin4 = true; break;
						case 37: pin5 = true; break;
						case 38: pin6 = true; break;
						case 39: pin7 = true; break;
						case 40: pin8 = true; break;
						case 41: pin9 = true; break;
						case 42: pin10 = true; break;
						case 43: pin11 = true; break;
						case 44: pin12 = true; break;
						case 45: pin13 = true; break;
						case 46: pin14 = true; break;
						case 47: pin15 = true; break;
						case 48: pin16 = true; break;
						case 49: pin17 = true; break;
						case 50: pin18 = true; break;
						case 51: pin19 = true; break;
						case 52: pin20 = true; break;
						}
					}
					break;

					case gmpi::midi_2_0::NoteOff:
					{
						const auto note = gmpi::midi_2_0::decodeNote(msg);
						int Note = (int)(uint8_t)note.noteNumber;

						switch (Note)
						{
						case 33: pin1 = false; break;
						case 34: pin2 = false; break;
						case 35: pin3 = false; break;
						case 36: pin4 = false; break;
						case 37: pin5 = false; break;
						case 38: pin6 = false; break;
						case 39: pin7 = false; break;
						case 40: pin8 = false; break;
						case 41: pin9 = false; break;
						case 42: pin10 = false; break;
						case 43: pin11 = false; break;
						case 44: pin12 = false; break;
						case 45: pin13 = false; break;
						case 46: pin14 = false; break;
						case 47: pin15 = false; break;
						case 48: pin16 = false; break;
						case 49: pin17 = false; break;
						case 50: pin18 = false; break;
						case 51: pin19 = false; break;
						case 52: pin20 = false; break;
						}
					}
					break;
					}

				}
			}

			//====================================
			if (pinString == 3)
			{
				if (!pinTransportRun)
				{
					int NoteS = 0;

					switch (header.status)
					{
					case gmpi::midi_2_0::NoteOn:
					{
						const auto note = gmpi::midi_2_0::decodeNote(msg);
						int Note = (int)(uint8_t)note.noteNumber;

						NoteS = Note;

						if (Note == NoteS)
						{
							pin1 = false;
							pin2 = false;
							pin3 = false;
							pin4 = false;
							pin5 = false;
							pin6 = false;
							pin7 = false;
							pin8 = false;
							pin9 = false;
							pin10 = false;
							pin11 = false;
							pin12 = false;
							pin13 = false;
							pin14 = false;
							pin15 = false;
							pin16 = false;
							pin17 = false;
							pin18 = false;
							pin19 = false;
							pin20 = false;

							switch (Note)
							{
							case 28: pin1 = true; break;
							case 29: pin2 = true; break;
							case 30: pin3 = true; break;
							case 31: pin4 = true; break;
							case 32: pin5 = true; break;
							case 33: pin6 = true; break;
							case 34: pin7 = true; break;
							case 35: pin8 = true; break;
							case 36: pin9 = true; break;
							case 37: pin10 = true; break;
							case 38: pin11 = true; break;
							case 39: pin12 = true; break;
							case 40: pin13 = true; break;
							case 41: pin14 = true; break;
							case 42: pin15 = true; break;
							case 43: pin16 = true; break;
							case 44: pin17 = true; break;
							case 45: pin18 = true; break;
							case 46: pin19 = true; break;
							case 47: pin20 = true; break;
							}
						}
						break;
					}
					}
				}

				if (pinTransportRun)
				{
					switch (header.status)
					{
					case gmpi::midi_2_0::NoteOn:
					{
						const auto note = gmpi::midi_2_0::decodeNote(msg);
						int Note = (int)(uint8_t)note.noteNumber;

						switch (Note)
						{
						case 28: pin1 = true; break;
						case 29: pin2 = true; break;
						case 30: pin3 = true; break;
						case 31: pin4 = true; break;
						case 32: pin5 = true; break;
						case 33: pin6 = true; break;
						case 34: pin7 = true; break;
						case 35: pin8 = true; break;
						case 36: pin9 = true; break;
						case 37: pin10 = true; break;
						case 38: pin11 = true; break;
						case 39: pin12 = true; break;
						case 40: pin13 = true; break;
						case 41: pin14 = true; break;
						case 42: pin15 = true; break;
						case 43: pin16 = true; break;
						case 44: pin17 = true; break;
						case 45: pin18 = true; break;
						case 46: pin19 = true; break;
						case 47: pin20 = true; break;
						}
					}

					break;

					case gmpi::midi_2_0::NoteOff:
					{
						const auto note = gmpi::midi_2_0::decodeNote(msg);
						int Note = (int)(uint8_t)note.noteNumber;

						switch (Note)
						{
						case 28: pin1 = false; break;
						case 29: pin2 = false; break;
						case 30: pin3 = false; break;
						case 31: pin4 = false; break;
						case 32: pin5 = false; break;
						case 33: pin6 = false; break;
						case 34: pin7 = false; break;
						case 35: pin8 = false; break;
						case 36: pin9 = false; break;
						case 37: pin10 = false; break;
						case 38: pin11 = false; break;
						case 39: pin12 = false; break;
						case 40: pin13 = false; break;
						case 41: pin14 = false; break;
						case 42: pin15 = false; break;
						case 43: pin16 = false; break;
						case 44: pin17 = false; break;
						case 45: pin18 = false; break;
						case 46: pin19 = false; break;
						case 47: pin20 = false; break;
						}
					}
					break;

					}

				}

			}
			//====================================
		}
	}
};

namespace
{
	auto r = Register<MIDI2Fret>::withId(L"MIDI2Fret");
}

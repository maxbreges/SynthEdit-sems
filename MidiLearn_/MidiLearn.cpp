#include "mp_sdk_audio.h"
#include "mp_midi.h"

using namespace gmpi;

class MidiLearn final : public MpBase2
{
	int boolSwitch = 0;
	int learnedNote = 36;

	MidiInPin pinInput;
	MidiOutPin pinMIDIOut;
	IntInPin pinNoteIn;
	IntOutPin pinNoteOut;
	BoolOutPin pinStatus;

	//int triggerDuration = 0;
	gmpi::midi_2_0::MidiConverter2 midiConverter;

public:
	MidiLearn() :
		// init the midi converter
		midiConverter(
			// provide a lambda to accept converted MIDI 2.0 messages
			[this](const midi::message_view& msg, int offset)
			{
				onMidi2Message(msg);
			}
		)
	{
		initializePin(pinInput);
		initializePin(pinMIDIOut);
		initializePin(pinNoteIn);
		initializePin(pinNoteOut);
		initializePin(pinStatus);
	}

	int32_t open() override
	{
		MpBase2::open();	// always call the base class

		//triggerDuration = static_cast<int>(getSampleRate() * 0.0005f); // 0.5 ms trigger pulse.

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
		learnedNote = pinNoteIn;

		const auto header = gmpi::midi_2_0::decodeHeader(msg);

		// only 8-byte messages supported. only 16 channels supported
		if (header.messageType != gmpi::midi_2_0::ChannelVoice64)
			return;
		switch (header.status)
		{

		case gmpi::midi_2_0::NoteOn:
		{
			const auto note = gmpi::midi_2_0::decodeNote(msg);

			int status = true;
			int b2 = note.noteNumber;

			if (b2 == learnedNote)
			{
				const auto out = gmpi::midi_2_0::makeNoteOnMessage(
					b2,
					note.velocity
				);

				pinMIDIOut.send(out.m);
				pinStatus = status;
			}

			setSleep(false);
			//setSubProcess(&MidiLearn::subProcess);
		}
		break;
		case gmpi::midi_2_0::NoteOff:
		{
			const auto note = gmpi::midi_2_0::decodeNote(msg);
			int b2 = note.noteNumber;
			int status = false;

			if (b2 == learnedNote)
			{
				const auto out = gmpi::midi_2_0::makeNoteOffMessage(
					b2,
					note.velocity
				);

				pinMIDIOut.send(out.m);
				pinStatus = status;
			}

			if (boolSwitch)
			{
				learnedNote = b2;
				boolSwitch = false;
				getHost()->sendMessageToGui(22323, sizeof(boolSwitch), &boolSwitch);
				getHost()->sendMessageToGui(22324, sizeof(b2), &b2);
			}
		}	

		break;

		default:
			break;
		};
	}

	int32_t receiveMessageFromGui(int32_t id, int32_t size, void* messageData)
	{
		if (id == 22322 && size == sizeof(int32_t))
		{
			boolSwitch = *(int32_t*)messageData;

			return 1; // handled
		}
		return 0; // not handled
	}

	void subProcess(int sampleFrames)
	{
/*		for (int s = 0; s < sampleFrames; ++s)
		{
			if (triggerCounter-- == 0)
			{
				pinTrigger.setValue(false, getBlockPosition() + s);
				setSleep(true);
				setSubProcess(&MidiLearn::subProcessNothing);
				return;
			}
		}*/
	}

	void onSetPins() override
	{
		if (pinNoteIn)
		{
			pinNoteOut = pinNoteIn;
		}
	}
};

namespace
{
	auto r = Register<MidiLearn>::withId(L"MidiLearn");
}

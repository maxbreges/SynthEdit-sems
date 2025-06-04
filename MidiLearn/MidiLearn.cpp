#include "mp_sdk_audio.h"
#include "mp_midi.h"

using namespace gmpi;

class MIDI final : public MpBase2
{
	int boolSwitch = 0;
	int learnedNote = 36;

	MidiInPin pinInput;
	MidiOutPin pinMIDIOut;
	IntInPin pinNoteIn; 
	IntOutPin pinNoteOut;
	BoolOutPin pinStatus;

public:
	MIDI()
	{
		initializePin( pinInput );
		initializePin( pinMIDIOut );
		initializePin(pinNoteIn);
		initializePin(pinNoteOut);
		initializePin(pinStatus);
	}

	void onMidiMessage(int pin, unsigned char* midiMessage, int size)
	{
		int status = midiMessage[1];
		int b2 = midiMessage[2]; // extracts note number from every midi message

		pinNoteOut = pinNoteIn;
		learnedNote = pinNoteIn;

		if (b2 == learnedNote)
			{
				pinMIDIOut.send(midiMessage, size, getBlockPosition());

				pinStatus = status - 128;
			}	

		if (boolSwitch)
		{
			learnedNote = b2;
			boolSwitch = false;
			getHost()->sendMessageToGui(22323, sizeof(boolSwitch), &boolSwitch);
			getHost()->sendMessageToGui(22324, sizeof(b2), &b2);
		}

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
};

namespace
{
	auto r = Register<MIDI>::withId(L"MIDI_Learn");
}

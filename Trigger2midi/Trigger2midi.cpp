#include "mp_sdk_audio.h"
#include "mp_midi.h"

using namespace gmpi;

class Trigger2midi final : public MpBase2
{
	BoolInPin pinTrigger;
	BoolInPin pinGate;
	AudioInPin pinPitch;
	AudioInPin pinVelocity;
	IntInPin pinChannel;
	MidiOutPin pinMIDIOut;

public:
	Trigger2midi()
	{
		initializePin( pinTrigger );
		initializePin( pinGate );
		initializePin( pinPitch );
		initializePin( pinVelocity );
		initializePin( pinChannel );
		initializePin( pinMIDIOut );
	}

	int MIDDLE_A = 69;
	bool gate_state = false;

	void onSetPins() override
	{
		int chan = pinChannel.getValue();
		bool note_active = false;
		int active_note = -1;
		float val = pinPitch.getValue() * 10.f;
		int midi_note = static_cast<int>(0.5f + MIDDLE_A + (val - 5.f) * 12.f);
		float velocity = pinVelocity.getValue();

		// Check which pins are updated.
		if (pinTrigger.isUpdated())
		{
			if (pinTrigger)
			{	
				if (note_active) //noteOff hasn't happened yet
				{
					if (active_note == midi_note) //if the pitch is the same
					{
						const auto out = gmpi::midi_2_0::makeNoteOffMessage(
							midi_note,
							0.5f,
							chan - 1
						);

						pinMIDIOut.send((const unsigned char*)&out, sizeof(out));
					}					
				}

				const auto out = gmpi::midi_2_0::makeNoteOnMessage(
					midi_note,
					velocity,
					chan - 1
				);

				pinMIDIOut.send((const unsigned char*)&out, sizeof(out));
				note_active = true;
				active_note = midi_note;
			}
		}
		
		if (pinGate.isUpdated())
		{
			if (pinGate != gate_state)
			{
				gate_state = pinGate;

				if (pinGate)
				{
				}
				else
				{					
					const auto out = gmpi::midi_2_0::makeNoteOffMessage(
						midi_note,
						0.5f,
						chan -1
					);

					pinMIDIOut.send((const unsigned char*)&out, sizeof(out));

					note_active = false;
					active_note = -1;
				}
			}
		}		
	}
};

namespace
{
	auto r = Register<Trigger2midi>::withId(L"My trigger2midi");
}

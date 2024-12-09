#include "mp_sdk_audio.h"
#include "mp_midi.h"

using namespace gmpi;
//using namespace GmpiMidi;
//using namespace midi_2_0;

class DrumTrigger final : public MpBase2
{
	MidiInPin pinMIDIIn;
	IntInPin pinChannel;
	FloatInPin pinOpenHHDecay;
	FloatInPin pinPedalHHDecay;
	FloatInPin pinClosedHHDecay;

	BoolOutPin pinKickTrig;
	FloatOutPin pinKickVel;

	BoolOutPin pinSnareTrig;
	FloatOutPin pinSnareVel;

	BoolOutPin pinHHTrig;
	FloatOutPin pinHHVel;
	FloatOutPin pinHHDecay;

	BoolOutPin pinCowbellTrig;
	FloatOutPin pinCowbellVel;

	BoolOutPin pinTom1Trig;
	FloatOutPin pinTom1Vel;

	BoolOutPin pinTom2Trig;
	FloatOutPin pinTom2Vel;

	BoolOutPin pinTom3Trig;
	FloatOutPin pinTom3Vel;

	BoolOutPin pinClapTrig;
	FloatOutPin pinClapVel;

	BoolOutPin pinCrashTrig;
	FloatOutPin pinCrashVel;

	BoolOutPin pinTambTrig;
	FloatOutPin pinTambVel;

	BoolOutPin pinRideTrig;
	FloatOutPin pinRideVel;

	BoolOutPin pinUser1Trig;
	FloatOutPin pinUser1Vel;

	BoolOutPin pinUser2Trig;
	FloatOutPin pinUser2Vel;

	BoolOutPin pinUser3Trig;
	FloatOutPin pinUser3Vel;

	BoolOutPin pinUser4Trig;
	FloatOutPin pinUser4Vel;

	IntInPin pinUser1Note;
	IntInPin pinUser2Note;
	IntInPin pinUser3Note;
	IntInPin pinUser4Note;

	gmpi::midi_2_0::MidiConverter2 midiConverter;

	int messageSize = 0;

	int Note = 0;
	float Velocity = 0.f;

public:
		DrumTrigger() :
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
		initializePin( pinChannel );
		initializePin(pinOpenHHDecay);
		initializePin(pinPedalHHDecay);
		initializePin(pinClosedHHDecay);

		initializePin(pinKickTrig);
		initializePin(pinKickVel);

		initializePin(pinSnareTrig);
		initializePin( pinSnareVel);

		initializePin( pinHHTrig);
		initializePin( pinHHVel);
		initializePin( pinHHDecay);

		initializePin( pinCowbellTrig);
		initializePin( pinCowbellVel);

		initializePin( pinTom1Trig);
		initializePin( pinTom1Vel);

		initializePin( pinTom2Trig);
		initializePin( pinTom2Vel);

		initializePin( pinTom3Trig);
		initializePin( pinTom3Vel);

		initializePin( pinClapTrig);
		initializePin( pinClapVel);

		initializePin( pinCrashTrig);
		initializePin( pinCrashVel);

		initializePin( pinTambTrig);
		initializePin( pinTambVel);

		initializePin( pinRideTrig);
		initializePin( pinRideVel);

		initializePin( pinUser1Trig);
		initializePin( pinUser1Vel);

		initializePin( pinUser2Trig);
		initializePin( pinUser2Vel);

		initializePin( pinUser3Trig);
		initializePin( pinUser3Vel);

		initializePin( pinUser4Trig);
		initializePin( pinUser4Vel);

		initializePin (pinUser1Note);
		initializePin (pinUser2Note);
		initializePin (pinUser3Note);
		initializePin (pinUser4Note);

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
		int chan = 0;
		chan = static_cast<uint8_t>(msg[1] & 0x0f);

		messageSize = (int)(size_t)msg.size();
		// only 8-byte messages supported. only 16 channels supported
		if (header.messageType != gmpi::midi_2_0::ChannelVoice64)
			return;		

		if ((chan == pinChannel) || (pinChannel == -1))

		{
			const auto note = gmpi::midi_2_0::decodeNote(msg);
			Note = (int)(uint8_t)note.noteNumber;
			Velocity = (float)(1.f * note.velocity);

			switch (header.status)
			{
			case gmpi::midi_2_0::NoteOn:
			{
				int i = Note;

				switch (i)
				{
				case 36: pinKickTrig = true; pinKickVel = Velocity; break;
				case 38: pinSnareTrig = true; pinSnareVel = Velocity; break;
				case 42: pinHHTrig = true; pinHHVel = Velocity; pinHHDecay = pinClosedHHDecay; break;
				case 44: pinHHTrig = true; pinHHVel = Velocity; pinHHDecay = pinPedalHHDecay; break;
				case 46: pinHHTrig = true; pinHHVel = Velocity; pinHHDecay = pinOpenHHDecay; break;
				case 45: pinTom1Trig = true; pinTom1Vel = Velocity; break;
				case 47: pinTom2Trig = true; pinTom2Vel = Velocity; break;
				case 50: pinTom3Trig = true; pinTom3Vel = Velocity; break;
				case 39: pinClapTrig = true; pinClapVel = Velocity; break;
				case 56: pinCowbellTrig = true; pinCowbellVel = Velocity; break;
				case 49: pinCrashTrig = true; pinCrashVel = Velocity; break;
				case 51: pinRideTrig = true; pinRideVel = Velocity; break;
				case 54: pinTambTrig = true; pinTambVel = Velocity; break;
				}

				if (Note == pinUser1Note)
				{
					pinUser1Trig = true;
					pinUser1Vel = Velocity;
				}
				if (Note == pinUser2Note)
				{
					pinUser2Trig = true;
					pinUser2Vel = Velocity;
				}
				if (Note == pinUser3Note)
				{
					pinUser3Trig = true;
					pinUser3Vel = Velocity;
				}
				if (Note == pinUser4Note)
				{
					pinUser4Trig = true;
					pinUser4Vel = Velocity;
				}
			}
			break;
			case gmpi::midi_2_0::NoteOff:
			{
				pinKickTrig = false;
				pinSnareTrig = false;
				pinHHTrig = false;
				pinCowbellTrig = false;
				pinTom1Trig = false;
				pinTom2Trig = false;
				pinTom3Trig = false;
				pinClapTrig = false;
				pinCrashTrig = false;
				pinTambTrig = false;
				pinRideTrig = false;
				pinUser1Trig = false;
				pinUser2Trig = false;
				pinUser3Trig = false;
				pinUser4Trig = false;
			}
			break;
			}
		};	
	}

	/*void subProcess(int sampleFrames)
	{
		// get pointers to in/output buffers.

		auto kickVel = getBuffer(pinKickVel);

		for (int s = sampleFrames; s > 0; s--)
		{
			*kickVel++ = Velocity;		
		}
	}*/

	/*void onSetPins() override
	{
		// Check which pins are updated.
		

		// Set state of output audio pins.

		pinKickVel.setStreaming(true);
		
		 pinSnareVel.setStreaming(true);
		 
		 pinHHVel.setStreaming(true);
		 pinHHDecay.setStreaming(true);

		 pinCowbellVel.setStreaming(true);

		 pinTom1Vel.setStreaming(true);

		 pinTom2Vel.setStreaming(true);

		 pinTom3Vel.setStreaming(true);

		 pinClapVel.setStreaming(true);

		 pinCrashVel.setStreaming(true);

		 pinTambVel.setStreaming(true);

		 pinRideVel.setStreaming(true);

		 pinUser1Vel.setStreaming(true);

		 pinUser2Vel.setStreaming(true);

		 pinUser3Vel.setStreaming(true);

		 pinUser4Vel.setStreaming(true);

		 // Set processing method.
		// setSubProcess(&DrumTrigger::subProcess);	
	
	}*/
};

namespace
{
	auto r = Register<DrumTrigger>::withId(L"DrumTrigger");
}

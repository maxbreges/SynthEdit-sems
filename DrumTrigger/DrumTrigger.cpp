#include "mp_sdk_audio.h"
#include "mp_midi.h"

using namespace gmpi;

class DrumTrigger final : public MpBase2
{
    MidiInPin pinMIDIIn;
    IntInPin pinChannel;
    FloatInPin pinOpenHHDecay;
    FloatInPin pinPedalHHDecay;
    FloatInPin pinClosedHHDecay;

    BoolOutPin pinKickTrig;
    AudioOutPin pinKickVel;

    BoolOutPin pinSnareTrig;
    AudioOutPin pinSnareVel;

    BoolOutPin pinHHTrig;
    AudioOutPin pinHHVel;
    FloatInPin pinHHDecay;

    BoolOutPin pinCowbellTrig;
    AudioOutPin pinCowbellVel;

    BoolOutPin pinTom1Trig;
    AudioOutPin pinTom1Vel;

    BoolOutPin pinTom2Trig;
    AudioOutPin pinTom2Vel;

    BoolOutPin pinTom3Trig;
    AudioOutPin pinTom3Vel;

    BoolOutPin pinClapTrig;
    AudioOutPin pinClapVel;

    BoolOutPin pinCrashTrig;
    AudioOutPin pinCrashVel;

    BoolOutPin pinTambTrig;
    AudioOutPin pinTambVel;

    BoolOutPin pinRideTrig;
    AudioOutPin pinRideVel;

    BoolOutPin pinUser1Trig;
    AudioOutPin pinUser1Vel;

    BoolOutPin pinUser2Trig;
    AudioOutPin pinUser2Vel;

    BoolOutPin pinUser3Trig;
    AudioOutPin pinUser3Vel;

    BoolOutPin pinUser4Trig;
    AudioOutPin pinUser4Vel;

    IntInPin pinUser1Note;
    IntInPin pinUser2Note;
    IntInPin pinUser3Note;
    IntInPin pinUser4Note;

    gmpi::midi_2_0::MidiConverter2 midiConverter;

    float VelocityKick = 0.0f;
    float VelocitySnare = 0.0f;
    float VelocityHHat = 0.0f;
    float VelocityTom1 = 0.0f;
    float VelocityTom2 = 0.0f;
    float VelocityTom3 = 0.0f;
    float VelocityClap = 0.0f;
    float VelocityCrash = 0.0f;
    float VelocityCowbell = 0.0f;
    float VelocityTamb = 0.0f;
    float VelocityRide = 0.0f;
    float VelocityUser1 = 0.0f;
    float VelocityUser2 = 0.0f;
    float VelocityUser3 = 0.0f;
    float VelocityUser4 = 0.0f;

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
        initializePin(pinMIDIIn);
        initializePin(pinChannel);
        initializePin(pinOpenHHDecay);
        initializePin(pinPedalHHDecay);
        initializePin(pinClosedHHDecay);

        initializePin(pinKickTrig);
        initializePin(pinKickVel);

        initializePin(pinSnareTrig);
        initializePin(pinSnareVel);

        initializePin(pinHHTrig);
        initializePin(pinHHVel);
        initializePin(pinHHDecay);

        initializePin(pinCowbellTrig);
        initializePin(pinCowbellVel);

        initializePin(pinTom1Trig);
        initializePin(pinTom1Vel);

        initializePin(pinTom2Trig);
        initializePin(pinTom2Vel);

        initializePin(pinTom3Trig);
        initializePin(pinTom3Vel);

        initializePin(pinClapTrig);
        initializePin(pinClapVel);

        initializePin(pinCrashTrig);
        initializePin(pinCrashVel);

        initializePin(pinTambTrig);
        initializePin(pinTambVel);

        initializePin(pinRideTrig);
        initializePin(pinRideVel);

        initializePin(pinUser1Trig);
        initializePin(pinUser1Vel);

        initializePin(pinUser2Trig);
        initializePin(pinUser2Vel);

        initializePin(pinUser3Trig);
        initializePin(pinUser3Vel);

        initializePin(pinUser4Trig);
        initializePin(pinUser4Vel);

        initializePin(pinUser1Note);
        initializePin(pinUser2Note);
        initializePin(pinUser3Note);
        initializePin(pinUser4Note);
    }

    int32_t open() override
    {
        MpBase2::open();	// always call the base class
        return gmpi::MP_OK;
    }

    void subProcess(int sampleFrames)
    {
        auto outputKick = getBuffer(pinKickVel);
        auto outputSnare = getBuffer(pinSnareVel);
        auto outputHHVel = getBuffer(pinHHVel);
        auto outputTom1Vel = getBuffer(pinTom1Vel);
        auto outputTom2Vel = getBuffer(pinTom2Vel);
        auto outputTom3Vel = getBuffer(pinTom3Vel);
        auto outputClapVel = getBuffer(pinClapVel);
        auto outputCrashVel = getBuffer(pinCrashVel);
        auto outputCowbellVel = getBuffer(pinCowbellVel);
        auto outputTambVel = getBuffer(pinTambVel);
        auto outputRideVel = getBuffer(pinRideVel);
        auto outputUser1Vel = getBuffer(pinUser1Vel);
        auto outputUser2Vel = getBuffer(pinUser2Vel);
        auto outputUser3Vel = getBuffer(pinUser3Vel);
        auto outputUser4Vel = getBuffer(pinUser4Vel);

        for (int s = sampleFrames; s > 0; --s)
        {
            *outputKick++ = VelocityKick;
            *outputSnare++ = VelocitySnare;
            *outputHHVel++ = VelocityHHat;
            *outputTom1Vel++ = VelocityTom1;
            *outputTom2Vel++ = VelocityTom2;
            *outputTom3Vel++ = VelocityTom3;
            *outputClapVel++ = VelocityClap;
            *outputCrashVel++ = VelocityCrash;
            *outputCowbellVel++ = VelocityCowbell;
            *outputTambVel++ = VelocityTamb;
            *outputRideVel++ = VelocityRide;
            *outputUser1Vel++ = VelocityUser1;
            *outputUser2Vel++ = VelocityUser2;
            *outputUser3Vel++ = VelocityUser3;
            *outputUser4Vel++ = VelocityUser4;

            // Reset triggers
            pinKickTrig.setValue(false, getBlockPosition() + s);
            pinSnareTrig.setValue(false, getBlockPosition() + s);
            pinHHTrig.setValue(false, getBlockPosition() + s);
            pinTom1Trig.setValue(false, getBlockPosition() + s);
            pinTom2Trig.setValue(false, getBlockPosition() + s);
            pinTom3Trig.setValue(false, getBlockPosition() + s);
            pinClapTrig.setValue(false, getBlockPosition() + s);
            pinCrashTrig.setValue(false, getBlockPosition() + s);
            pinCowbellTrig.setValue(false, getBlockPosition() + s);
            pinTambTrig.setValue(false, getBlockPosition() + s);
            pinRideTrig.setValue(false, getBlockPosition() + s);
            pinUser1Trig.setValue(false, getBlockPosition() + s);
            pinUser2Trig.setValue(false, getBlockPosition() + s);
            pinUser3Trig.setValue(false, getBlockPosition() + s);
            pinUser4Trig.setValue(false, getBlockPosition() + s);
        }
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

        int messageSize = (int)(size_t)msg.size();
        // only 8-byte messages supported. only 16 channels supported
        if (header.messageType != gmpi::midi_2_0::ChannelVoice64)
            return;

        if ((chan == pinChannel) || (pinChannel == -1))
        {
            const auto note = gmpi::midi_2_0::decodeNote(msg);
            int Note = (int)(uint8_t)note.noteNumber;

            switch (header.status)
            {
            case gmpi::midi_2_0::NoteOn:
            {
                int i = Note;

                switch (i)
                {
                case 36:
                    pinKickTrig = true;
                    VelocityKick = (float)(1.f * note.velocity);
                    break;
                case 38:
                    pinSnareTrig = true;
                    VelocitySnare = (float)(1.f * note.velocity);
                    break;
                case 42:
                    pinHHTrig = true;
                    pinHHDecay = pinClosedHHDecay;
                    VelocityHHat = (float)(1.f * note.velocity);
                    break;
                case 44:
                    pinHHTrig = true;
                    pinHHDecay = pinPedalHHDecay;
                    VelocityHHat = (float)(1.f * note.velocity);
                    break;
                case 46:
                    pinHHTrig = true;
                    pinHHDecay = pinOpenHHDecay;
                    VelocityHHat = (float)(1.f * note.velocity);
                    break;
                case 45:
                    pinTom1Trig = true;
                    VelocityTom1 = (float)(1.f * note.velocity);
                    break;
                case 47:
                    pinTom2Trig = true;
                    VelocityTom2 = (float)(1.f * note.velocity);
                    break;
                case 50:
                    pinTom3Trig = true;
                    VelocityTom3 = (float)(1.f * note.velocity);
                    break;
                case 39:
                    pinClapTrig = true;
                    VelocityClap = (float)(1.f * note.velocity);
                    break;
                case 56:
                    pinCowbellTrig = true;
                    VelocityCowbell = (float)(1.f * note.velocity);
                    break;
                case 49:
                    pinCrashTrig = true;
                    VelocityCrash = (float)(1.f * note.velocity);
                    break;
                case 51:
                    pinRideTrig = true;
                    VelocityRide = (float)(1.f * note.velocity);
                    break;
                case 54:
                    pinTambTrig = true;
                    VelocityTamb = (float)(1.f * note.velocity);
                    break;
                }
                // User notes
                if (Note == pinUser1Note)
                {
                    pinUser1Trig = true;
                    VelocityUser1 = (float)(1.f * note.velocity);
                }
                if (Note == pinUser2Note)
                {
                    pinUser2Trig = true;
                    VelocityUser2 = (float)(1.f * note.velocity);
                }
                if (Note == pinUser3Note)
                {
                    pinUser3Trig = true;
                    VelocityUser3 = (float)(1.f * note.velocity);
                }
                if (Note == pinUser4Note)
                {
                    pinUser4Trig = true;
                    VelocityUser4 = (float)(1.f * note.velocity);
                }

                setSleep(false);
                setSubProcess(&DrumTrigger::subProcess);
            }
            break;

            case gmpi::midi_2_0::NoteOff:
            {
                // Handle note off if needed
            }
            break;
            }
        }
    }
};

namespace
{
    auto r = Register<DrumTrigger>::withId(L"DrumTrigger");
}
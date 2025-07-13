//this code consumes more cpu than if using switch
#include "mp_sdk_audio.h"
#include "mp_midi.h"
#include <unordered_map>
#include <functional>

using namespace gmpi;

class DrumTrigger final : public MpBase2
{
    // MIDI input and control pins
    MidiInPin pinMIDIIn;
    IntInPin pinChannel;
    FloatInPin pinOpenHHDecay;
    FloatInPin pinPedalHHDecay;
    FloatInPin pinClosedHHDecay;

    // Trigger output pins
    BoolOutPin pinKickTrig;
    AudioOutPin pinKickVel;

    BoolOutPin pinSnareTrig;
    AudioOutPin pinSnareVel;

    BoolOutPin pinHHTrig;
    AudioOutPin pinHHVel;
    AudioOutPin pinHHDecay;

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

    // User note numbers (to be set accordingly)
    IntInPin pinUser1Note;
    IntInPin pinUser2Note;
    IntInPin pinUser3Note;
    IntInPin pinUser4Note;

    // MIDI converter
    gmpi::midi_2_0::MidiConverter2 midiConverter;

    // Velocities
    float VelocityKick = 0.0f;
    float VelocitySnare = 0.0f;
    float VelocityHHat = 0.0f;
    float currentHhDecay = 0.0f;
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

    // Map for note handling
    std::unordered_map<int, std::function<void(float)>> noteHandlers;

public:
    DrumTrigger() :
        // initialize the MIDI converter with a lambda
        midiConverter(
            [this](const midi::message_view& msg, int offset)
            {
                onMidi2Message(msg);
            }
        )
    {
        // Initialize pins
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

        // Initialize the note handlers map
        noteHandlers = {
            {36, [this](float velocity) { pinKickTrig = true; VelocityKick = velocity; }},
            {38, [this](float velocity) { pinSnareTrig = true; VelocitySnare = velocity; }},
            {42, [this](float velocity) { pinHHTrig = true; currentHhDecay = pinClosedHHDecay; VelocityHHat = velocity; }},
            {44, [this](float velocity) { pinHHTrig = true; currentHhDecay = pinPedalHHDecay; VelocityHHat = velocity; }},
            {46, [this](float velocity) { pinHHTrig = true; currentHhDecay = pinOpenHHDecay; VelocityHHat = velocity; }},
            {45, [this](float velocity) { pinTom1Trig = true; VelocityTom1 = velocity; }},
            {47, [this](float velocity) { pinTom2Trig = true; VelocityTom2 = velocity; }},
            {50, [this](float velocity) { pinTom3Trig = true; VelocityTom3 = velocity; }},
            {39, [this](float velocity) { pinClapTrig = true; VelocityClap = velocity; }},
            {56, [this](float velocity) { pinCowbellTrig = true; VelocityCowbell = velocity; }},
            {49, [this](float velocity) { pinCrashTrig = true; VelocityCrash = velocity; }},
            {51, [this](float velocity) { pinRideTrig = true; VelocityRide = velocity; }},
            {54, [this](float velocity) { pinTambTrig = true; VelocityTamb = velocity; }},
            // User notes
            {pinUser1Note, [this](float velocity) { pinUser1Trig = true; VelocityUser1 = velocity; }},
            {pinUser2Note, [this](float velocity) { pinUser2Trig = true; VelocityUser2 = velocity; }},
            {pinUser3Note, [this](float velocity) { pinUser3Trig = true; VelocityUser3 = velocity; }},
            {pinUser4Note, [this](float velocity) { pinUser4Trig = true; VelocityUser4 = velocity; }}
        };
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
        auto outputHHDecay = getBuffer(pinHHDecay);
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
            *outputHHDecay++ = currentHhDecay * 0.1f;
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

    // Handle incoming MIDI messages
    void onMidiMessage(int pin, unsigned char* midiMessage, int size) override
    {
        midi::message_view msg((const uint8_t*)midiMessage, size);
        midiConverter.processMidi(msg, -1);
    }

    // Process MIDI 2.0 messages
    void onMidi2Message(const midi::message_view& msg)
    {
        const auto header = gmpi::midi_2_0::decodeHeader(msg);
        int chan = static_cast<uint8_t>(msg[1] & 0x0f);

        if (header.messageType != gmpi::midi_2_0::ChannelVoice64)
            return;

        if ((chan == pinChannel) || (pinChannel == -1))
        {
            const auto note = gmpi::midi_2_0::decodeNote(msg);
            int noteNumber = (int)(uint8_t)note.noteNumber;
            float velocity = (float)(1.f * note.velocity);

            if (header.status == gmpi::midi_2_0::NoteOn)
            {
                handleNoteOn(noteNumber, velocity);
            }
            // You can add handling for NoteOff if needed
        }
    }

    // Lookup and handle note on
    void handleNoteOn(int noteNumber, float velocity)
    {
        auto it = noteHandlers.find(noteNumber);
        if (it != noteHandlers.end())
        {
            it->second(velocity);
            setSleep(false);
            setSubProcess(&DrumTrigger::subProcess);
        }
    }
};

// Register class
namespace
{
    auto r = Register<DrumTrigger>::withId(L"DrumTrigger");
}
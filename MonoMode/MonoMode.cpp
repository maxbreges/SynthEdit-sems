#include "mp_sdk_audio.h"
#include "mp_midi.h"

using namespace gmpi;
using namespace GmpiMidi;
using namespace midi_2_0;

class MonoMode final : public MpBase2
{
    MidiInPin pinMIDIIn;
    MidiOutPin pinMIDIOut;

    gmpi::midi_2_0::MidiConverter2 midiConverter;

public:
    MonoMode() :
        midiConverter(
            [this](const midi::message_view& msg, int offset) {
                onMidi2Message(msg);
            }
        )
    {
        initializePin(pinMIDIIn);
        initializePin(pinMIDIOut);
    }

    void onMidiMessage(int pin, unsigned char* midiMessage, int size) override
    {
        midi::message_view msg((const uint8_t*)midiMessage, size);
        midiConverter.processMidi(msg, -1);
    }

    void onMidi2Message(const midi::message_view& msg)
    {
        const auto header = gmpi::midi_2_0::decodeHeader(msg);

        // Decode note and channel data
        const auto note = gmpi::midi_2_0::decodeNote(msg);
        int chan = header.channel;
        int Note = (int)(uint8_t)note.noteNumber;
        float Velocity = (float)(255.f * note.velocity);

        // Logic to play only the last played note
        static int lastNote = -1;

        if (header.messageType != gmpi::midi_2_0::ChannelVoice64)
            return;

        switch (header.status)
        {
        case gmpi::midi_2_0::NoteOn:
        {
            if (lastNote != -1)
            {
                const auto outOff = gmpi::midi_2_0::makeNoteOffMessage(lastNote, 0, chan);
                pinMIDIOut.send(outOff.m);
            }

            lastNote = Note;
            const auto out = gmpi::midi_2_0::makeNoteOnMessage(Note, Velocity, chan);
            pinMIDIOut.send(out.m);
        } break;

        case gmpi::midi_2_0::NoteOff:
        {
            if (lastNote == Note)
            {
                lastNote = -1;
                const auto out = gmpi::midi_2_0::makeNoteOffMessage(Note, Velocity, chan);
                pinMIDIOut.send(out.m);
            }
        } break;
        }
    };
};

namespace
{
    auto r = Register<MonoMode>::withId(L"MonoMode");
}
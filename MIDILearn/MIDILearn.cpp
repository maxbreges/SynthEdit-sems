#include "mp_sdk_audio.h"
#include "mp_midi.h"

using namespace gmpi;

class MidiLearn final : public MpBase2
{
    const int MESSAGE_ID = 2244;

    BoolInPin pinEnable;
    MidiInPin pinMIDIIn;
    MidiOutPin pinMIDIOut;
    BoolOutPin pinStatus;
    BoolOutPin pinGateToGui;
     
    IntInPin pinStoredNote;
    IntOutPin pinNote;

    gmpi::midi_2_0::MidiConverter2 midiConverter;

public:
    MidiLearn()
        :
        midiConverter(
            [this](const midi::message_view& msg, int offset)
            {
                onMidi2Message(msg);
            }
        )
    {
        initializePin(pinEnable);
        initializePin(pinMIDIIn);
        initializePin(pinMIDIOut);
        initializePin(pinStatus);
        initializePin(pinGateToGui);
 
        initializePin(pinStoredNote);
        initializePin(pinNote);

        pinNote = pinStoredNote;
    }

    void onSetPins() override
    {
        if (pinEnable)
        {
            pinStatus = true;
        }

        else
        { 
            pinStatus = false;
            pinNote = pinStoredNote;
        }
    }

    void onMidiMessage(int pin, unsigned char* midiMessage, int size) override
    {
        midi::message_view msg((const uint8_t*)midiMessage, size);
        midiConverter.processMidi(msg, -1); // This calls onMidi2Message internally
    }    

    void onMidi2Message(const midi::message_view& msg) 
    {   
        pinGateToGui = false;
        int learnedNote = pinStoredNote;

        const auto header = gmpi::midi_2_0::decodeHeader(msg);

        switch (header.status)
        {
        case gmpi::midi_2_0::NoteOn:
        {
            auto note = gmpi::midi_2_0::decodeNote(msg);
            uint8_t noteNumber = static_cast<uint8_t>(note.noteNumber);            

            if (pinEnable)
            {                
                learnedNote = noteNumber;
                getHost()->sendMessageToGui(MESSAGE_ID, sizeof(learnedNote), &learnedNote);
                pinGateToGui = true;
            }
            else
            {
               pinGateToGui = false;
            }

            if (noteNumber == learnedNote)
            { 
                const auto out = gmpi::midi_2_0::makeNoteOnMessage(
                    learnedNote,
                    note.velocity);
                pinMIDIOut.send(out.m); // 
            }
            break;
        }

        case gmpi::midi_2_0::NoteOff:
        {
            auto note = gmpi::midi_2_0::decodeNote(msg);
            uint8_t noteNumber = static_cast<uint8_t>(note.noteNumber);

            if (noteNumber == learnedNote)
            {
                const auto out = gmpi::midi_2_0::makeNoteOffMessage(
                    learnedNote,
                    note.velocity);
                pinMIDIOut.send(out.m); // 
            }
        }

        break;
        
        default:
            break;
        }
    }
};

namespace
{
    auto r = Register<MidiLearn>::withId(L"MidiLearnDSP");
}
#include "mp_sdk_audio.h"
#include "mp_midi.h"
#include <vector>
#include <sstream>

using namespace gmpi;
using namespace GmpiMidi;
using namespace midi_2_0;

class MIDI2Frets final : public MpBase2
{
    MidiInPin pinMIDIIn;
    IntInPin pinFretCount;
    IntInPin pinStringsCount;

    IntOutPin pinY1; // Channel 1 output
    IntOutPin pinY2; // Channel 2 output
    IntOutPin pinY3; // Channel 3 output
    IntOutPin pinY4; // Channel 4 output
    IntOutPin pinY5; // Channel 3 output
    IntOutPin pinY6; // Channel 4 output

    gmpi::midi_2_0::MidiConverter2 midiConverter;
    int messageSize = 0;

    static const int MAX_NOTES = 128; // Max possible MIDI notes
    std::vector<bool> activeNotes; // Tracks active MIDI notes

public:
    MIDI2Frets() :
        midiConverter(
            [this](const midi::message_view& msg, int offset) {
                onMidi2Message(msg);
            }
        ),
        activeNotes(MAX_NOTES, false) // Initialize vector for 128 MIDI notes
    {
        initializePin(pinMIDIIn);
        initializePin(pinFretCount);
        initializePin(pinStringsCount);
        initializePin(pinY1);
        initializePin(pinY2);
        initializePin(pinY3);
        initializePin(pinY4);
        initializePin(pinY5);
        initializePin(pinY6);
    }

    void onMidiMessage(int pin, unsigned char* midiMessage, int size) override
    {
        midi::message_view msg((const uint8_t*)midiMessage, size);
        // Convert all MIDI messages to MIDI 2.0
        midiConverter.processMidi(msg, -1);
    }

    void onMidi2Message(const midi::message_view& msg)
{
    const auto header = gmpi::midi_2_0::decodeHeader(msg);
    int chan = static_cast<uint8_t>(msg[1] & 0x0f); // Extract the channel number (0–15)

    // Ensure we only handle channels we care about (1 to 4)
    if (chan >= 0 && chan < 7) // Only handle channel 1 to 4 (0-3)
    {
        const auto note = gmpi::midi_2_0::decodeNote(msg);
        int noteNumber = static_cast<uint8_t>(note.noteNumber); // Get the MIDI note number

        switch (header.status)
        {
            case gmpi::midi_2_0::NoteOn:
                activeNotes[noteNumber] = true; // Mark the note as active
                break;
            case gmpi::midi_2_0::NoteOff:
                activeNotes[noteNumber] = false; // Mark the note as inactive
                break;
            default:
                return; // Skip other message types
        }

        // Find the highest active note for the current channel
        int activeFret = -1; // Initialize as no active fret
        for (int i = 0; i < MAX_NOTES; ++i) {
            if (activeNotes[i]) {
                activeFret = i; // Update active fret
            }
        }

        // If an active fret exists, send it to the appropriate channel output
        if (activeFret >= 0)
        {
            switch (chan)
            {
                case 0: // Channel 1
                    pinY1.setValue(activeFret + 0); // Shift by 1 for 1-based output
                    pinY1.sendPinUpdate(0);
                    break;
                case 1: // Channel 2
                    pinY2.setValue(activeFret + 0); // Shift by 1 for 1-based output
                    pinY2.sendPinUpdate(0);
                    break;
                case 2: // Channel 3
                    pinY3.setValue(activeFret + 0); // Shift by 1 for 1-based output
                    pinY3.sendPinUpdate(0);
                    break;
                case 3: // Channel 4
                    pinY4.setValue(activeFret + 0); // Shift by 1 for 1-based output
                    pinY4.sendPinUpdate(0);
                    break;
                case 4: // Channel 3
                    pinY5.setValue(activeFret + 0); // Shift by 1 for 1-based output
                    pinY5.sendPinUpdate(0);
                    break;
                case 5: // Channel 4
                    pinY6.setValue(activeFret + 0); // Shift by 1 for 1-based output
                    pinY6.sendPinUpdate(0);
                    break;
            }
        }
        else 
        {
            // If no active fret, reset the output (optional)
            switch (chan)
            {
                case 0: pinY1.setValue(0); pinY1.sendPinUpdate(0); break;
                case 1: pinY2.setValue(0); pinY2.sendPinUpdate(0); break;
                case 2: pinY3.setValue(0); pinY3.sendPinUpdate(0); break;
                case 3: pinY4.setValue(0); pinY4.sendPinUpdate(0); break;
                case 4: pinY5.setValue(0); pinY5.sendPinUpdate(0); break;
                case 5: pinY6.setValue(0); pinY6.sendPinUpdate(0); break;
            }
        }
    }
}
};

namespace
{
    auto r = Register<MIDI2Frets>::withId(L"MIDI2Frets");
}
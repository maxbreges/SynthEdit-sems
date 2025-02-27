#include "mp_sdk_audio.h"
#include "mp_midi.h"
#include <vector>
#include <sstream>

using namespace gmpi;
using namespace GmpiMidi;
using namespace midi_2_0;

class MidiToBlobs final : public MpBase2
{
    MidiInPin pinMIDIIn;
    IntInPin pinFretCount;
    IntInPin pinStringsCount;

    BlobOutPin pinFrets;   // Output for frets
    BlobOutPin pinStrings;  // Output for strings

    gmpi::midi_2_0::MidiConverter2 midiConverter;
    static const int MAX_NOTES = 128; // Max possible MIDI notes
    static const int MAX_CHANNELS = 16; // Max possible MIDI channels
    std::vector<bool> activeNotes; // Tracks active MIDI notes
    std::vector<int> activeChannels; // Tracks active MIDI channels

public:
    MidiToBlobs() :
        midiConverter(
            [this](const midi::message_view& msg, int offset) {
                onMidi2Message(msg);
            }
        ),
        activeNotes(MAX_NOTES, false), // Initialize vector for 128 MIDI notes to false
        activeChannels(MAX_NOTES, -1) // Initialize vector for 16 MIDI channels
    {
        initializePin(pinMIDIIn);
        initializePin(pinFretCount);
        initializePin(pinStringsCount);
        initializePin(pinFrets);
        initializePin(pinStrings);
    }

    void onMidiMessage(int pin, unsigned char* midiMessage, int size) override
    {
        midi::message_view msg((const uint8_t*)midiMessage, size);
        midiConverter.processMidi(msg, -1);
    }

    void onMidi2Message(const midi::message_view& msg)
    {
        const auto header = gmpi::midi_2_0::decodeHeader(msg);
        int chan = static_cast<uint8_t>(msg[1] & 0x0f); // Extract the channel number (0–15)

        // Validate the channel range
        if (chan < 0 || chan >= MAX_CHANNELS)
        {
            return; // Exit if the channel is out of range
        }

        const auto note = gmpi::midi_2_0::decodeNote(msg);
        int noteNumber = static_cast<int>(note.noteNumber); // Get the MIDI number of the note

        // Validate the note range
        if (noteNumber < 0 || noteNumber >= MAX_NOTES)
        {
            return; // Exit if the note number is out of range
        }

        // Handle the NoteOn and NoteOff messages
        switch (header.status)
        {
        case gmpi::midi_2_0::NoteOn:
            activeNotes[noteNumber] = true; // Mark the note as active
            activeChannels[noteNumber] = chan; // Store the active channel
            break;
        case gmpi::midi_2_0::NoteOff:
            activeNotes[noteNumber] = false; // Mark the note as inactive
            break;
        default:
            return; // Skip other message types
        }

        std::vector<int> frets;
        std::vector<int> strings;

        // Collect active notes and their corresponding channels
        for (int i = 0; i < MAX_NOTES; ++i) {
            if (activeNotes[i]) {
                frets.push_back(i); // Collect the note number (frets)
                strings.push_back(activeChannels[i] + 1); // Convert to 1-based channel
            }
        }

        // Send data to the output pins if we have active frets to report
        if (!frets.empty()) {
            pinFrets.setValueRaw(frets.size() * sizeof(int), frets.data());
            pinFrets.sendPinUpdate(0); // Send frets to GUI

            pinStrings.setValueRaw(strings.size() * sizeof(int), strings.data());
            pinStrings.sendPinUpdate(0); // Send strings to GUI
        }
    }
};

namespace
{
    auto r = Register<MidiToBlobs>::withId(L"MidiToBlobs");
}
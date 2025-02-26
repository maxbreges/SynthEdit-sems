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
    std::vector<bool> activeNotes; // Tracks active MIDI notes

public:
    MidiToBlobs() :
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

        if (chan >= 0 && chan < 16) // Validate channel range
        {
            const auto note = gmpi::midi_2_0::decodeNote(msg);
            int noteNumber = (int)(uint8_t)note.noteNumber; // Get the MIDI number of the note

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

            std::vector<int> frets;
            std::vector<int> strings;

            // Collect active notes and their corresponding channels
            for (int i = 0; i < MAX_NOTES; ++i) {
                if (activeNotes[i]) {
                    frets.push_back(i);           // Collect the note number (frets)
                    strings.push_back(chan + 1); // Convert to 1-based channel
                }
            }

            // Prepare blobs for output
            sendBlob(pinFrets, frets);
            sendBlob(pinStrings, strings);
        }
    }

    // Helper function to send blobs
    void sendBlob(BlobOutPin& pin, const std::vector<int>& values)
    {
        if (!values.empty()) {
            size_t size = values.size() * sizeof(int);
            pin.setValueRaw(size, values.data());
            pin.sendPinUpdate(0);
        }
        else {
            // If there are no values, send an empty blob
            pin.setValueRaw(0, nullptr);
            pin.sendPinUpdate(0);
        }
    }
};

namespace
{
    auto r = Register<MidiToBlobs>::withId(L"MidiToBlobs");
}
#include "mp_sdk_audio.h"
#include "mp_midi.h"
#include <vector>

using namespace gmpi;
using namespace GmpiMidi;
using namespace midi_2_0;

class MidiToBlobs final : public MpBase2
{
    MidiInPin pinMIDIIn;
    BlobOutPin pinFrets;   // Output for frets
    BlobOutPin pinStrings;  // Output for strings

    gmpi::midi_2_0::MidiConverter2 midiConverter;
    static const int MAX_NOTES = 128; // Max possible MIDI notes
    std::vector<bool> activeNotes; // Tracks active MIDI notes
    std::vector<int> activeChannels; // Tracks active MIDI channels

public:
    MidiToBlobs() :
        midiConverter(
            [this](const midi::message_view& msg, int) {
                onMidi2Message(msg);
            }
        ),
        activeNotes(MAX_NOTES, false),
        activeChannels(MAX_NOTES, -1)
    {
        initializePin(pinMIDIIn);
        initializePin(pinFrets);
        initializePin(pinStrings);
    }

    void onMidiMessage(int pin, unsigned char* midiMessage, int size) override
    {
        midiConverter.processMidi(midi::message_view((const uint8_t*)midiMessage, size), -1);
    }

    void onMidi2Message(const midi::message_view& msg)
    {
        const auto header = gmpi::midi_2_0::decodeHeader(msg);
        int chan = msg[1] & 0x0f; // Extract channel number

        if (chan < 0 || chan >= 16) return; // Validate channel range

        int noteNumber = static_cast<int>(gmpi::midi_2_0::decodeNote(msg).noteNumber);
        if (noteNumber < 0 || noteNumber >= MAX_NOTES) return; // Validate note range

        // Handle NoteOn and NoteOff messages
        if (header.status == gmpi::midi_2_0::NoteOn) {
            activeNotes[noteNumber] = true;
            activeChannels[noteNumber] = chan;
        }
        else if (header.status == gmpi::midi_2_0::NoteOff) {
            activeNotes[noteNumber] = false;
        }
        else {
            return; // Skip other message types
        }

        std::vector<int> frets, strings;
        for (int i = 0; i < MAX_NOTES; ++i) {
            if (activeNotes[i]) {
                frets.push_back(i);
                strings.push_back(activeChannels[i] + 1); // Convert to 1-based channel
            }
        }

        // Send data to the output pins if there are active frets
        if (!frets.empty()) {
            pinFrets.setValueRaw(frets.size() * sizeof(int), frets.data());
            pinFrets.sendPinUpdate(0);
            pinStrings.setValueRaw(strings.size() * sizeof(int), strings.data());
            pinStrings.sendPinUpdate(0);
        }
    }
};

namespace {
    auto r = Register<MidiToBlobs>::withId(L"MidiToBlobs");
}
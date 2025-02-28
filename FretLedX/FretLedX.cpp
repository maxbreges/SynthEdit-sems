#include "mp_sdk_audio.h"
#include "mp_midi.h"
#include <vector>
#include <algorithm>

using namespace gmpi;
using namespace GmpiMidi;
using namespace midi_2_0;

class FretLedX final : public MpBase2
{
    MidiInPin pinMidiIn;
    BlobOutPin pinGuiX;    // Output for active notes
    BlobOutPin pinGuiY;    // Output for active channels

    gmpi::midi_2_0::MidiConverter2 midiConverter;

public:
    FretLedX() :
        midiConverter(
            [this](const midi::message_view& msg, int) {
                onMidi2Message(msg);
            }
        )
    {
        initializePin(pinMidiIn);
        initializePin(pinGuiX);
        initializePin(pinGuiY);
    }

    std::vector<int> activeNotes; // Currently active notes
    std::vector<int> activeChannels; // Currently active channels

    void onMidiMessage(int pin, unsigned char* midiMessage, int size) override
    {
        midiConverter.processMidi(midi::message_view((const uint8_t*)midiMessage, size), -1);
    }

    void onMidi2Message(const midi::message_view& msg)
    {
        const auto header = gmpi::midi_2_0::decodeHeader(msg);
        int chan = msg[1] & 0x0f; // Extract channel number
        int noteNumber = static_cast<int>(gmpi::midi_2_0::decodeNote(msg).noteNumber);

        // Handle NoteOn message
        if (header.status == gmpi::midi_2_0::NoteOn) {
            if (std::find(activeNotes.begin(), activeNotes.end(), noteNumber) == activeNotes.end()) {
                activeNotes.push_back(noteNumber);
                activeChannels.push_back(chan);
                transmitData();
            }
        }
        // Handle NoteOff message
        else if (header.status == gmpi::midi_2_0::NoteOff) {
            auto noteIt = std::find(activeNotes.begin(), activeNotes.end(), noteNumber);
            if (noteIt != activeNotes.end()) {
                int index = std::distance(activeNotes.begin(), noteIt);
                activeNotes.erase(noteIt);
                activeChannels.erase(activeChannels.begin() + index);
                transmitData();
            }
        }
    }

private:
    void transmitData()
    {
        if (!activeNotes.empty() && activeNotes.size() == activeChannels.size()) {
            getHost()->setPin(0, pinGuiX.getId(), activeNotes.size() * sizeof(int), activeNotes.data());
            getHost()->setPin(0, pinGuiY.getId(), activeChannels.size() * sizeof(int), activeChannels.data());
        }
    }
};

namespace {
    auto r = Register<FretLedX>::withId(L"FretLedX");
}
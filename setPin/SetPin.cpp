#include "mp_sdk_audio.h"
#include "mp_midi.h"
#include <vector>
#include <algorithm> // For std::find

using namespace gmpi;
using namespace GmpiMidi;
using namespace midi_2_0;

class setPin final : public MpBase2
{
    struct NoteEvent
    {
        uint8_t note;      // MIDI note number
        uint8_t channel;   // MIDI channel
        float velocity;    // Note velocity
    };

    MidiInPin pinMIDIIn;
    BlobOutPin pinGui;

    gmpi::midi_2_0::MidiConverter2 midiConverter;

public:
    setPin() :
        midiConverter(
            [this](const midi::message_view& msg, int) {
                onMidi2Message(msg);
            }
        )
    {
        initializePin(pinMIDIIn);
        initializePin(pinGui);
    }

    std::vector<NoteEvent> activeNoteEvents;

    void onMidiMessage(int pin, unsigned char* midiMessage, int size) override
    {
        // Process incoming MIDI messages
        midiConverter.processMidi(midi::message_view((const uint8_t*)midiMessage, size), -1);
    }

    void onMidi2Message(const midi::message_view& msg)
    {
        // Decode the MIDI message header
        const auto header = gmpi::midi_2_0::decodeHeader(msg);
        int chan = msg[1] & 0x0f; // Extract channel number
        int noteNumber = static_cast<int>(gmpi::midi_2_0::decodeNote(msg).noteNumber);

        // Handle NoteOn message
        if (header.status == gmpi::midi_2_0::NoteOn)
        {
            handleNoteOn(noteNumber, chan);
        }
        // Handle NoteOff message
        else if (header.status == gmpi::midi_2_0::NoteOff)
        {
            handleNoteOff(noteNumber, chan);
        }
    }

private:
    void handleNoteOn(int noteNumber, int chan)
    {
        auto it = std::find_if(activeNoteEvents.begin(), activeNoteEvents.end(),
            [noteNumber, chan](const NoteEvent& event)
            {
                return event.note == noteNumber && event.channel == chan;
            });

        // If the note is not found in active notes, add it
        if (it == activeNoteEvents.end())
        {
            NoteEvent newEvent = { static_cast<uint8_t>(noteNumber), static_cast<uint8_t>(chan), 1.0f }; // Assuming full velocity
            activeNoteEvents.push_back(newEvent);
            transmitData(); // Transmit if it's a new note
        }
    }

    void handleNoteOff(int noteNumber, int chan)
    {
        auto it = std::find_if(activeNoteEvents.begin(), activeNoteEvents.end(),
            [noteNumber, chan](const NoteEvent& event)
            {
                return event.note == noteNumber && event.channel == chan;
            });

        // If found, erase the note from active notes
        if (it != activeNoteEvents.end())
        {
            activeNoteEvents.erase(it);
            transmitData();
        }
    }

    void transmitData()
    {
        int32_t currentTimestamp = getBlockPosition();

        // Only transmit if there are note events to send
        if (!activeNoteEvents.empty())
        {
            // Calculate size
            size_t dataSize = activeNoteEvents.size() * sizeof(NoteEvent);
            // Transmit data
            getHost()->setPin(currentTimestamp, pinGui.getId(), dataSize, activeNoteEvents.data());
        }
    }
};

namespace
{
    auto r = Register<setPin>::withId(L"setPin");
}
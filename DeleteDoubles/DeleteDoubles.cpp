#include "mp_sdk_audio.h"
#include "mp_midi.h"

using namespace gmpi;
using namespace GmpiMidi;
using namespace midi_2_0;

class DeleteDoubles final : public MpBase2
{
    MidiInPin pinMIDIIn;
    MidiOutPin pinMIDIOut;

    gmpi::midi_2_0::MidiConverter2 midiConverter;

    // Store the last message received
    std::vector<uint8_t> lastReceivedMessage;

public:
    DeleteDoubles() :
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
        initializePin(pinMIDIOut);
        lastReceivedMessage.reserve(256); // reserve some space
    }

    int32_t open() override
    {
        MpBase2::open();    // always call the base class
        return gmpi::MP_OK;
    }

    // Pass all MIDI to the converter
    void onMidiMessage(int pin, unsigned char* midiMessage, int size) override
    {
        midi::message_view msg((const uint8_t*)midiMessage, size);
        midiConverter.processMidi(msg, -1);
    }

    void onMidi2Message(const midi::message_view& msg)
    {
        // Convert message_view to a vector for easy comparison
        std::vector<uint8_t> currentMessage(msg.begin(), msg.end());

        // Check if the current message is the same as the last one
        if (currentMessage == lastReceivedMessage)
        {
            // Same message received, do not pass to MIDI Out
            return;
        }

        // Different message: send it out
        // You can use pinMIDIOut to send the message
        // Example:
        pinMIDIOut.send(currentMessage.data(), (int)currentMessage.size());

        // Update last received message
        lastReceivedMessage = std::move(currentMessage);
    }

    void onSetPins() override
    {
        // Check which pins are updated if needed
    }
};

namespace
{
    auto r = Register<DeleteDoubles>::withId(L"DeleteDoubles");
}
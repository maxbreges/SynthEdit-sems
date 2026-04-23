#include "mp_sdk_audio.h"
#include "mp_midi.h"
#include <cstdint>

using namespace gmpi;

class CC2Note final : public MpBase2
{
    MidiInPin pinMIDIIn;
    MidiOutPin pinMIDIOut;
    IntInPin pinController;
    IntInPin pinChannel;

    gmpi::midi_2_0::MidiConverter2 midiConverter;

    int lastControllerValue = -1; // store last control change value
    int triggerDuration = 0;
    int noteNumber = 0;
    bool initializedState = false;
    
    // Store the last message received
    std::vector<uint8_t> lastReceivedMessage;

public:
    CC2Note() :
        midiConverter(
            [this](const midi::message_view& msg, int offset)
            {
                onMidi2Message(msg);
            }
        )
    {
        initializePin(pinMIDIIn);
        initializePin(pinMIDIOut);
        initializePin(pinController);
        initializePin(pinChannel);
    }

    void onMidiMessage(int pin, unsigned char* midiMessage, int size) override
    {
        midi::message_view msg((const uint8_t*)midiMessage, size);
        midiConverter.processMidi(msg, -1);
    }
    
    void onMidi2Message(const midi::message_view& msg)
    {
        std::vector<uint8_t> currentMessage(msg.begin(), msg.end());

        // Check if the current message is the same as the last one
        if (currentMessage == lastReceivedMessage)
        {
            // Same message received, do not pass to MIDI Out
            return;
        }
        const auto header = gmpi::midi_2_0::decodeHeader(msg);

        if (header.messageType != gmpi::midi_2_0::ChannelVoice64)
            return;
        int selectedChannel = pinChannel.getValue();

        switch (header.status)
        {
        case gmpi::midi_2_0::ControlChange:
        {
            const auto header = gmpi::midi_2_0::decodeHeader(msg);
            const auto chan = header.channel;

            const auto controller = gmpi::midi_2_0::decodeController(msg);
            int cntrType = static_cast<int>(controller.type);
            int newValue = static_cast<int>(controller.value * 127.0001f);
            noteNumber = newValue;

            if ((initializedState && selectedChannel == -1 && cntrType == pinController.getValue() && newValue != lastControllerValue) || initializedState && chan == selectedChannel && cntrType == pinController.getValue())
                // Send MIDI Note On
            {
                auto noteOnMsg = gmpi::midi_2_0::makeNoteOnMessage(noteNumber, 0.5f);
                pinMIDIOut.send(noteOnMsg.m);
               
                if (newValue != lastControllerValue)
                {
                    lastControllerValue = newValue;
                    auto noteOffMsg = gmpi::midi_2_0::makeNoteOffMessage(noteNumber, 0.5f);
                    pinMIDIOut.send(noteOffMsg.m, sizeof(uint64_t), getBlockPosition() + triggerDuration * 100);
                }
            }
            initializedState = true;
        }
        break;
        }
        // Update last received message
        lastReceivedMessage = std::move(currentMessage);
    }

    int32_t open() override
    {
        MpBase2::open();
        triggerDuration = static_cast<int>(getSampleRate() * 0.001f); // 1.0 ms
        return gmpi::MP_OK;
    }
};

namespace
{
    auto r = Register<CC2Note>::withId(L"CC2Note");
}
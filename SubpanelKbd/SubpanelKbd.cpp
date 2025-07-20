#include "mp_sdk_audio.h"
#include "../se_sdk3/mp_midi.h"

using namespace gmpi;

class SubpanelKbd final : public MpBase2
{
    MidiOutPin pinMIDIOut;
    IntOutPin pinIntOut;

    struct noteOnOffMsg
    {
        uint8_t data[3];
    };

    std::vector< noteOnOffMsg > buffer;
    gmpi::midi_2_0::MidiConverter2 midiConverter;

public:
    SubpanelKbd() :
        // init the midi converter
        midiConverter(
            // provide a lambda to accept converted MIDI 2.0 messages
            [this](const midi::message_view& msg, int offset)
            {
                pinMIDIOut.send(msg.begin(), msg.size(), offset);
            }
        )
    {
        initializePin(pinMIDIOut);
        initializePin(pinIntOut);
        buffer.reserve(20);
    }

    struct NoteMessage {
        int32_t noteNumber;
        int32_t noteOn;
    };

    int32_t receiveMessageFromGui(int32_t id, int32_t size, void* messageData)
    {
        if (id == 2213 && size == sizeof(NoteMessage))
        {
            NoteMessage* msg = (NoteMessage*)messageData;
            int noteNumber = msg->noteNumber;
            int noteOn = msg->noteOn;

            pinIntOut.setValue(noteNumber, getBlockPosition());

            // Construct MIDI message as before
            noteOnOffMsg midi;
            unsigned char channel = 0; // channel 1
            unsigned char baseStatus = noteOn ? (0x90 | channel) : (0x80 | channel);
            midi.data[0] = baseStatus;
            midi.data[1] = static_cast<unsigned char>(noteNumber);
            float velocityFloat = 0.5f; // your velocity
            unsigned char velocity = static_cast<unsigned char>(velocityFloat * 127);
            midi.data[2] = velocity;

            buffer.push_back(midi);
            onSetPins();
        }
        return gmpi::MP_OK;
    }

    void subProcess(int sampleFrames)
    {
        int timeStamp = 0;
        for (auto msg : buffer)
        {
            midiConverter.processMidi({ msg.data, sizeof(msg.data) }, getBlockPosition() + timeStamp++);

            if (timeStamp >= sampleFrames)
                return;
        }

        buffer.clear();
    }

    void onSetPins() override
    {
        setSubProcess(&SubpanelKbd::subProcess);

        // Set sleep mode (optional).
        setSleep(false);
    }
};

namespace
{
    auto r = Register<SubpanelKbd>::withId(L"SubpanelKbd");
}
#include "mp_sdk_audio.h"
#include "mp_midi.h"
#include <array>
#include <algorithm>

using namespace gmpi;
using namespace GmpiMidi;
using namespace midi_2_0;

class DisplayFret final : public MpBase2
{
    MidiInPin pinMIDIIn;
    IntOutPin pinString1;
    IntOutPin pinString2;
    IntOutPin pinString3;
    IntOutPin pinString4;
    IntOutPin pinString5;
    IntOutPin pinString6;
    BoolInPin pinTransportRun;

    gmpi::midi_2_0::MidiConverter2 midiConverter;

    std::array<int, 7> lastNote = { 0, 0, 0, 0, 0, 0, 0 }; // Last active note for each channel
    std::array<bool, 7> isActive = { false, false, false, false, false, false, false }; // To track active state

public:
    DisplayFret() :
        midiConverter(
            [this](const midi::message_view& msg, int) {
                onMidi2Message(msg);
            }
        )
    {
        initializePin(pinMIDIIn);
        initializePin(pinString1);
        initializePin(pinString2);
        initializePin(pinString3);
        initializePin(pinString4);
        initializePin(pinString5);
        initializePin(pinString6);
        initializePin(pinTransportRun);
    }

    void onMidiMessage(int pin, unsigned char* midiMessage, int size) override
    {
        midiConverter.processMidi(midi::message_view((const uint8_t*)midiMessage, size), -1);
    }

    void onMidi2Message(const midi::message_view& msg)
    {
        const auto header = gmpi::midi_2_0::decodeHeader(msg);
        int chan = msg[1] & 0x0f; // Extract channel number
        int noteNumber = static_cast<int>(gmpi::midi_2_0::decodeNote(msg).noteNumber);

        if (pinTransportRun) // If transport is running
        {
            // Handle NoteOn message
            if (header.status == gmpi::midi_2_0::NoteOn)
            {
                if (chan < lastNote.size() && (!isActive[chan] || noteNumber != lastNote[chan]))
                {
                    lastNote[chan] = noteNumber;
                    isActive[chan] = true; // Set active state
                    setPinValue(chan, noteNumber); // Call a helper function to set the pin value
                }
            }
            // Handle NoteOff message
            else if (header.status == gmpi::midi_2_0::NoteOff)
            {
                if (chan < lastNote.size() && isActive[chan])
                {
                    lastNote[chan] = 0; // Clear the note
                    isActive[chan] = false; // Update the active state
                    setPinValue(chan, 0); // Set the pin value to 0 on Note Off
                }
            }
        }
        else // If transport is NOT running
        {
            // Handle NoteOn message
            if (header.status == gmpi::midi_2_0::NoteOn)
            {
                // If the incoming channel is different from the last active one
                for (int i = 0; i < lastNote.size(); ++i)
                {
                    if (isActive[i] && i != chan)
                    {
                        // Send NoteOff for the active channel that is not the current one
                        setPinValue(i, 0); // Send NoteOff for the previous channel
                        lastNote[i] = 0;   // Clear the note
                        isActive[i] = false; // Update the active state
                    }
                }

                // Now process the current NoteOn
                if (chan < lastNote.size() && (!isActive[chan] || noteNumber != lastNote[chan]))
                {
                    lastNote[chan] = noteNumber;
                    isActive[chan] = true; // Set active state
                    setPinValue(chan, noteNumber); // Set the pin value for the new NoteOn
                }
            }
            // NoteOff messages will be ignored; no changes to isActive or lastNote should happen here
        }
    }

    void setPinValue(int channel, int noteValue)
    {
        switch (channel)
        {
        case 0: pinString1 = noteValue; break;
        case 1: pinString2 = noteValue; break;
        case 2: pinString3 = noteValue; break;
        case 3: pinString4 = noteValue; break;
        case 4: pinString5 = noteValue; break;
        case 5: pinString6 = noteValue; break;
        }
    }

    void onSetPins()
    {
        if (pinTransportRun.isUpdated())
        {            
                for (int i = 0; i < lastNote.size(); ++i)
                {
                    if (isActive[i])
                    {
                        // Simulate NoteOff for any active notes
                        setPinValue(i, 0); // Send NoteOff, represented by setting the pin value to 0
                        lastNote[i] = 0;   // Reset the last note value
                        isActive[i] = false; // Mark channel as inactive
                    }
                }            
            // If transport run goes false, you can handle it here if necessary
            // You can choose to implement additional logic if needed
        }
    }
};

namespace
{
    auto r = Register<DisplayFret>::withId(L"DisplayFret");
}
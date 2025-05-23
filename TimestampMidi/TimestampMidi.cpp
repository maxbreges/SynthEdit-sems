#include "mp_sdk_audio.h"
#include "mp_midi.h"
#include <chrono>
#include <fstream>

using namespace gmpi;
using namespace GmpiMidi;
using namespace midi_2_0;

class TimestampMidi final : public MpBase2
{
	MidiInPin pinMIDIIn;
	StringOutPin pinTimestamp;

public:

	// Declare static member
	static std::ofstream logFile;

	gmpi::midi_2_0::MidiConverter2 midiConverter;
	//const int MESSAGE_ID = 3322;

	TimestampMidi() :
		midiConverter(
			[this](const midi::message_view& msg, int) {
				onMidi2Message(msg);
			}
		)
	{
		initializePin( pinMIDIIn );
		initializePin( pinTimestamp );
        // Initialize the log file
        if (!logFile.is_open())
        {
            logFile.open("TimestampMidi_log.txt", std::ios::app);
            if (!logFile)
            {
                // handle error if needed
            }
        }
	}

	void onMidiMessage(int pin, unsigned char* midiMessage, int size)
	{
		midiConverter.processMidi(midi::message_view((const uint8_t*)midiMessage, size), -1);
	}

    // Custom handler for MIDI2 messages
    void TimestampMidi::onMidi2Message(const midi::message_view& msg)
    {
        const auto header = gmpi::midi_2_0::decodeHeader(msg);
        int chan = msg[1] & 0x0f; // Extract channel number
        int noteNumber = static_cast<int>(gmpi::midi_2_0::decodeNote(msg).noteNumber);

        // Handle NoteOn message
        if (header.status == gmpi::midi_2_0::NoteOn)
        {
           // int testValue = NoteOn; // or false
            // Send message to GUI
            //getHost()->sendMessageToGui(MESSAGE_ID, sizeof(testValue), &testValue);

            auto now = std::chrono::system_clock::now();
            auto microseconds_since_epoch = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();

            // Convert to string
            std::string timestampMicroseconds = std::to_string(microseconds_since_epoch);
            
            // Log timestamp to the persistent log file
            if (logFile.is_open())
            {
                pinTimestamp = timestampMicroseconds;
                logFile << timestampMicroseconds << "\n"; // Append with newline
                logFile.flush(); // 
            }
            else
            {
                // Handle error if needed
            }
        }
        // Handle NoteOff message
        else if (header.status == gmpi::midi_2_0::NoteOff)
        {
            // Handle NoteOff if needed
        }
    }

	void onSetPins() override
	{
	}
};

// Define static member
std::ofstream TimestampMidi::logFile;

namespace
{
	auto r = Register<TimestampMidi>::withId(L"TimestampMidi");
}

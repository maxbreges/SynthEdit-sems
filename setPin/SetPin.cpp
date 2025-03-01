#include "mp_sdk_audio.h"
#include "mp_midi.h"
#include <vector>

using namespace gmpi;
using namespace GmpiMidi;
using namespace midi_2_0;

class setPin final : public MpBase2
{
	MidiInPin pinMIDIIn;	
	BlobOutPin pinX;
	BlobOutPin pinY;

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
		initializePin(pinX);
		initializePin(pinY);
	}

	std::vector<int> activeChannels; // Currently active channels
	std::vector<int> activeNotes; // Currently active notes	

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
		if (header.status == gmpi::midi_2_0::NoteOn)
		{
			if (std::find(activeNotes.begin(), activeNotes.end(), noteNumber) == activeNotes.end())
			{
				activeChannels.push_back(chan);
				activeNotes.push_back(noteNumber);
				transmitData(); // Only transmit if it's a new note
			}
			/*else
			{
				// Here, you could do something to indicate that the note was retriggered.
				// For example, you might want to send an additional signal or simply log it.
				// If you want to re-trigger the same note visually, you could send a lower priority message.
				getHost()->setPin(int(chan), pinX.getId(), sizeof(int), &noteNumber);
			}*/
		}
		// Handle NoteOff message
		else if (header.status == gmpi::midi_2_0::NoteOff)
		{
			auto noteIt = std::find(activeNotes.begin(), activeNotes.end(), noteNumber);
			if (noteIt != activeNotes.end())
			{
				int index = std::distance(activeNotes.begin(), noteIt);
				activeChannels.erase(activeChannels.begin() + index);
				activeNotes.erase(noteIt);
				transmitData();
			}
		}
	}

private:
	void transmitData()
	{
		int32_t currentTimestamp = getBlockPosition(); // Use getBlockPosition from this instance. Leaving it just as an integer value results in random ~0.015s lags
		// Send data to GUI or whatever the pinGui does getHost()->setPin(currentTimestamp, pinGui.getId(), sizeof(int), &noteNumber); }

		if (!activeNotes.empty() && activeNotes.size() == activeChannels.size()) {
			getHost()->setPin(currentTimestamp, pinY.getId(), activeChannels.size() * sizeof(int), activeChannels.data());
			getHost()->setPin(currentTimestamp, pinX.getId(), activeNotes.size() * sizeof(int), activeNotes.data());
		}
	}
};

namespace
{
	auto r = Register<setPin>::withId(L"setPin");
}

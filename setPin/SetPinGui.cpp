#include "mp_sdk_gui2.h"
#include <sstream>

using namespace gmpi;

struct NoteEvent
{
    uint8_t note;      // MIDI note number
    uint8_t channel;   // MIDI channel
    float velocity;    // Note velocity
};

class SetPinGui final : public SeGuiInvisibleBase
{
    std::vector<NoteEvent> activeNotes; // Store currently active notes
    std::ostringstream oss;

    void onSetPin()
    {
        // Determine how many NoteEvent structs are received
        int n = pinGui.rawSize() / sizeof(NoteEvent);
        if (n <= 0) return; // If no items are received, exit

        // Resize the activeNotes vector to hold the incoming NoteEvent structs
        activeNotes.resize(n);
        NoteEvent* noteData = reinterpret_cast<NoteEvent*>(pinGui.rawData()); // Cast to NoteEvent pointer

        // Store all received note events in activeNotes
        for (int i = 0; i < n; ++i) {
            activeNotes[i] = noteData[i]; // Copy each NoteEvent to activeNotes
        }

        // Clear the ostringstream to prepare for new output
        oss.str(""); // Reset the stream
        oss.clear(); // Clear any error flags

        // Append information about each NoteEvent to the string stream
        for (const auto& note : activeNotes) {
            oss << "Note: " << static_cast<int>(note.note)   // Convert uint8_t to int for display
                << ", String: " << static_cast<int>(note.channel) // Convert to int
                << ", Velocity: " << note.velocity << "\n"; // float values
        }

        // Set the output string to the pinDebug
        pinDebug = oss.str();

        // Optionally: Perform any other processing or communication with GUI here.
    }

    BlobGuiPin pinGui; // Input pin for receiving data
    StringGuiPin pinDebug; 

public:
    SetPinGui()
    {
        // Initialize the pin with the onSetPin function as the callback
        initializePin(pinGui, static_cast<MpGuiBaseMemberPtr2>(&SetPinGui::onSetPin));
        initializePin(pinDebug, static_cast<MpGuiBaseMemberPtr2>(&SetPinGui::onSetPin));
    }
};

namespace
{
    auto r = Register<SetPinGui>::withId(L"setPin");
}
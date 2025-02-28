// FretLedXGui.cpp

#include "mp_sdk_gui2.h"
//#include "Drawing.h"
//#include <sstream>

//using namespace std;
using namespace gmpi;
//using namespace GmpiDrawing;

class FretLedXGui final : public SeGuiInvisibleBase
{
    BlobGuiPin pinX; // Receive the chord note numbers
    BlobGuiPin pinY; // Receive the chord channel numbers
   // StringGuiPin pinDebug;
    BlobGuiPin pinXOut;
    BlobGuiPin pinYOut;
   // StringGuiPin pinDebugOut;
  //  std::vector<int> activeNotes; // Store the currently active notes
  //  std::vector<int> activeChannels; // Store the currently active notes

public:
    FretLedXGui()
    {
        initializePin(pinX, static_cast<MpGuiBaseMemberPtr2>(&FretLedXGui::onSetX));
        initializePin(pinY, static_cast<MpGuiBaseMemberPtr2>(&FretLedXGui::onSetX));
      //  initializePin(pinDebug, static_cast<MpGuiBaseMemberPtr2>(&FretLedXGui::onSetX));
        initializePin(pinXOut, static_cast<MpGuiBaseMemberPtr2>(&FretLedXGui::onSetX));
        initializePin(pinYOut, static_cast<MpGuiBaseMemberPtr2>(&FretLedXGui::onSetX));
      //  initializePin(pinDebugOut, static_cast<MpGuiBaseMemberPtr2>(&FretLedXGui::onSetX));
    }

    void onSetX()
    {
       /* int n_items = pinX.rawSize() / sizeof(int);
        if (n_items <= 0) return;

        // Resize the activeNotes vector to hold the incoming chord notes
        activeNotes.resize(n_items);
        int* noteData = (int*)pinX.rawData();

        // Store all received note numbers in activeNotes
        for (int i = 0; i < n_items; ++i) {
            activeNotes[i] = noteData[i];
        }*/

       /* // Log transmitted notes
        std::ostringstream oss;
        oss << "Transmitting Notes: ";
        for (int note : activeNotes) {
            oss << note << " ";
        }
        pinDebugOut = oss.str();*/ // debugging gui side only

        pinXOut = pinX;
        pinYOut = pinY;
       // pinDebugOut = pinDebug;
    }

/*    int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override
    {
        Graphics g(drawingContext);
        auto r = getRect();
        auto brushOn = g.CreateSolidColorBrush(Color::Green);
        auto brushOff = g.CreateSolidColorBrush(Color::LightGray);                

        Point center = { (r.left + r.right) * 0.5f, (r.top + r.bottom) * 0.5f };
        float radius = (10.0f); // Fixed radius for the LED

        // Draw the LED based on the active notes
        // Example: light up multiple LEDs for each active note
        for (int note : activeNotes) {
            if (note > 0) {
                // Draw an LED based on its position (this is a simple example)
                Point ledPosition = { center.x * (note/note), center.y }; // Map MIDI note number to position
                g.FillEllipse(GmpiDrawing::Ellipse(ledPosition, radius, radius), brushOn); // LED ON
            }
            else {
                g.FillEllipse(GmpiDrawing::Ellipse(center, radius, radius), brushOff); // LED OFF
            }
            invalidateRect();           
        }
        return gmpi::MP_OK;
    }*/
};

namespace {
    auto r = Register<FretLedXGui>::withId(L"FretLedX");
}
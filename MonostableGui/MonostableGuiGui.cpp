#include "mp_sdk_gui2.h"
#include <chrono>
#include <thread>

using namespace gmpi;

class MonostableGuiGui final : public SeGuiInvisibleBase
{
private:
    int count = 0;
    bool processing = false;  // Flag to indicate if processing is ongoing

public:
    void onSetMouseDown()
    {
        if (pinMouseDown)
        {
            // Check if we are already processing
            if (processing)
            {
                return; // Ignore further clicks while processing
            }

            processing = true; // Set flag to indicate processing has started

            // Increment count
            count++;

            // Update pinBoolOut immediately to reflect the new count
            pinBoolOut = count;

            int countDown = pinTime;
            
            // Start countdown in a separate thread
            std::thread([this, countDown]()
                {
                    //for (int i = countDown; i >= 0; --i)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(countDown));
                    }

                    // Reset count and flag when countdown is finished
                    count = 0;
                    pinBoolOut = count; // Reset output pin
     
                    // Reset the processing flag to allow future clicks
                    processing = false;
                }).detach(); // Detach the thread to allow it to run independently
        }
    }

    void onSetTime() { /* pinTimems changed */ }

    BoolGuiPin pinMouseDown;
    IntGuiPin pinTime;
    BoolGuiPin pinBoolOut;
   // IntGuiPin pinCountdown;

public:
    MonostableGuiGui()
    {
        initializePin(pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&MonostableGuiGui::onSetMouseDown));
        initializePin(pinTime, static_cast<MpGuiBaseMemberPtr2>(&MonostableGuiGui::onSetTime));
        initializePin(pinBoolOut, static_cast<MpGuiBaseMemberPtr2>(&MonostableGuiGui::onSetMouseDown));
      //  initializePin(pinCountdown, static_cast<MpGuiBaseMemberPtr2>(&MonostableGuiGui::onSetMouseDown));
    }
};

namespace
{
    auto r = Register<MonostableGuiGui>::withId(L"MonostableGui");
}
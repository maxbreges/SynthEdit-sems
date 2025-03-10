#include "mp_sdk_gui2.h"
#include <chrono>
#include <thread>
#include <atomic>
#include <iostream> // For outputting the countdown
#include <mutex>    // To handle thread safety

using namespace gmpi;

class DoubleClickGui final : public SeGuiInvisibleBase
{
private:
    std::mutex stateMutex; // Mutex to protect shared state
    BoolGuiPin pinMouseDown; // Input for mouse down event
    IntGuiPin pinCountdownNumber; // Input for countdown time in milliseconds
    IntGuiPin pinIntOut; // Output for remaining time    
    FloatGuiPin pinAnimationPosition;
    FloatGuiPin pinResetValue;

    std::atomic<bool> countdownActive{ false }; // Track countdown state
    bool new_state = false; // Track state change

public:
    DoubleClickGui()
    {
        // Initialize pins
        initializePin(pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&DoubleClickGui::onSetMouseDown));
        initializePin(pinCountdownNumber, static_cast<MpGuiBaseMemberPtr2>(&DoubleClickGui::onSetTime));
        initializePin(pinIntOut, static_cast<MpGuiBaseMemberPtr2>(&DoubleClickGui::onSetIntOut));
        initializePin(pinAnimationPosition, static_cast<MpGuiBaseMemberPtr2>(&DoubleClickGui::onSetAnimPos));
        initializePin(pinResetValue, static_cast<MpGuiBaseMemberPtr2>(&DoubleClickGui::onSetMouseDown));
    }

    void onSetAnimPos()
    {
        // Implement animation position handling here if needed
    }

    void onSetMouseDown()
    {
        std::lock_guard<std::mutex> lock(stateMutex); // Protect shared state

        if (pinMouseDown)
        {
            // Start countdown when mouse is pressed
            countdownActive = true;

            // Get the countdown time from pinCountdownNumber
            int countdownNumber = pinCountdownNumber;
            pinIntOut = countdownNumber; // Initialize output with the starting value

            // Start countdown in a separate thread
            std::thread([this, countdownNumber]()
                {
                    for (int remaining = countdownNumber; remaining >= 0; --remaining)
                    {
                        {
                            // Lock for accessing shared state
                            std::lock_guard<std::mutex> lock(stateMutex);
                            pinIntOut = remaining; // Update output pin with remaining time
                        }
                        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Update every 1 ms

                        {
                            // Lock for accessing shared state
                            std::lock_guard<std::mutex> lock(stateMutex);
                            if (!pinMouseDown)
                            {
                                new_state = true;
                            }

                            if (new_state && pinMouseDown)
                            {
                                pinAnimationPosition = pinResetValue;
                            }
                        }
                    }
                    countdownActive = false; // End countdown when done
                    pinIntOut = 0; // Ensure the output is zero at the end 
                    new_state = false;
                }).detach(); // Detach the thread                
        }
    }

    void onSetTime()
    {
        // Placeholder for handling time changes if necessary
    }

    void onSetIntOut()
    {
        // Placeholder for handling int output changes if necessary
    }
};

namespace
{
    auto r = Register<DoubleClickGui>::withId(L"Double-Click");
}
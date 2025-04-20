#include "mp_sdk_gui2.h"
#include <chrono>
#include <thread>

using namespace gmpi;

class BoolTriggerGuiGui final : public SeGuiInvisibleBase
{
private:
    BoolGuiPin pinOnOff; // Pin to enable or disable the output
    IntGuiPin pinRate;   // Pin to set the rate (in Hz)
    BoolGuiPin pinBool;  // Pin for boolean output state

    bool outputState = false; // Current state of the output
    std::chrono::high_resolution_clock::time_point lastUpdateTime; // Last toggle time
    int currentRate = 1;      // Default rate in Hz

    void onSetOnOff()
    {
        if (pinOnOff)
        {
           lastUpdateTime = std::chrono::steady_clock::now(); // Reset the timer
            std::thread(&BoolTriggerGuiGui::outputLoop, this).detach(); // Start toggling in a new thread
        }
        else
        {
            pinBool = false; // Set output to false if turned off
          //  pinBool.sendValue(); // Send the updated state
        }
    }

    void onSetRate()
    {
        currentRate = pinRate; // Update the rate from the pin
        if (currentRate < 1) currentRate = 1; // Enforce minimum of 1 Hz
        if (currentRate > 60) currentRate = 60; // Enforce maximum of 60 Hz (or whatever limit fits your application)
    }

    void outputLoop()
    {
        while (pinOnOff) // Continue while the output is enabled
        {
            auto now = std::chrono::high_resolution_clock::now();
          auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime).count();
            int period = 1000 / currentRate; // Calculate period in milliseconds

         if (elapsedTime >= period) // Check if it's time to toggle
            {
                outputState = !outputState; // Toggle boolean state
                pinBool = outputState; // Update the pin with the new state
               // pinBool.sendValue(); // Send updated pin state
               lastUpdateTime = now; // Reset last toggle time
            }

          std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Brief sleep to prevent busy waiting
        }
    }

public:
    BoolTriggerGuiGui()
    {
        initializePin(pinOnOff, static_cast<MpGuiBaseMemberPtr2>(&BoolTriggerGuiGui::onSetOnOff));
        initializePin(pinRate, static_cast<MpGuiBaseMemberPtr2>(&BoolTriggerGuiGui::onSetRate));
        initializePin(pinBool, static_cast<MpGuiBaseMemberPtr2>(&BoolTriggerGuiGui::outputLoop));
    }
};

namespace
{
    auto r = Register<BoolTriggerGuiGui>::withId(L"BoolTriggerGui"); // Registration of the class in the system
}
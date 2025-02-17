#include "mp_sdk_gui2.h"
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <algorithm> // For std::clamp

using namespace gmpi;

class FloatAnimationGui final : public SeGuiInvisibleBase
{
    // Data Members
    std::atomic<bool> animationRunning{ false };
    std::thread animationThread;
    std::mutex pinMutex; // Protects access to animationPosition and potentially pin values
    std::mutex speedMutex; // Protects access to internalAnimationSpeed
    std::atomic<int> animationDirection{ 1 };

    // Animation Speed (frames to go from 0.0 to 1.0)
    int internalAnimationSpeed = 3; // Default: 3 seconds for full cycle (30 frames)
    float animationPosition = 0.0f; // Current animation position (0.0 - 1.0)
    std::atomic_bool animationStopped{ false };

    // Pin Definitions
    IntGuiPin pinOnOff;
    FloatGuiPin pinFrameNumber;
    IntGuiPin pinSpeed;
    IntGuiPin pinMode;

    // --- Helper Functions ---
    void startAnimation()
    {
        if (!animationRunning && !animationThread.joinable())
        {
            animationRunning = true;
            animationStopped = false; // Reset stopped flag
            animationThread = std::thread(&FloatAnimationGui::animationLoop, this);
        }
    }

    void stopAnimation()
    {
        if (animationRunning)
        {
            animationRunning = false;
            animationStopped = true;
            if (animationThread.joinable())
            {
                animationThread.join(); // Wait for the thread to finish
            }
            // Reset variables AFTER the thread has joined
            std::lock_guard<std::mutex> lock(pinMutex);
            animationPosition = 0.0f;
            pinFrameNumber = animationPosition; // Set Frame Number.
            animationStopped = false; // reset stopped flag.
        }
    }

    // --- Pin Handlers ---
    void onSetOnOff()
    {
        if (pinOnOff.getValue())
        {
            startAnimation();
        }
        else
        {
            stopAnimation();
        }
    }

    void onSetAnimationPosition()
    {
        // Unused
    }

    void onSetSpeed()
    {
        int newSpeed = pinSpeed; // Read the pin's value
        newSpeed = std::clamp(newSpeed, 1, 100); // Frames: 1 = 0.1s, 100 = 10s, to avoid too fast/slow values
        std::lock_guard<std::mutex> lock(speedMutex); // Protect access to internalAnimationSpeed
        internalAnimationSpeed = newSpeed;
    }

    void onSetMode()
    {
        int newMode = pinMode;
        if (newMode == 2)
        {
            animationDirection = 1;
        }
    }

    void animationLoop()
    {
        animationRunning = true;
        animationStopped = false; // Reset the flag

        while (animationRunning)
        {
            auto start = std::chrono::steady_clock::now();

            // Retrieve animation speed safely inside the loop
            int currentSpeed;
            {
                std::lock_guard<std::mutex> lock(speedMutex); // Lock to protect internalAnimationSpeed
                currentSpeed = internalAnimationSpeed;
            }

            std::lock_guard<std::mutex> lock(pinMutex); // Lock to protect animationPosition

            // Read mode
            int mode = pinMode;

            float speedFactor = static_cast<float>(currentSpeed) / 300.0f; // 30 is the FPS rate
            switch (mode)
            {
            case 0: // Loop
                animationPosition += speedFactor;
                if (animationPosition > 1.0f)
                {
                    animationPosition -= 1.0f;
                }
                break;
            case 1: // Reverse Loop
                animationPosition -= speedFactor;
                if (animationPosition < 0.0f)
                {
                    animationPosition += 1.0f;
                }
                break;
            case 2: // Ping Pong
                if (animationDirection == 1) // Forward
                {
                    animationPosition += speedFactor;
                    if (animationPosition > 1.0f)
                    {
                        animationPosition = 1.0f;
                        animationDirection = -1;
                    }
                }
                else // Backward
                {
                    animationPosition -= speedFactor;
                    if (animationPosition < 0.0f)
                    {
                        animationPosition = 0.0f;
                        animationDirection = 1;
                    }
                }
                break;
            }

            pinFrameNumber = animationPosition;

            auto end = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            auto targetDelay = std::chrono::milliseconds(33); // About 30 FPS
            auto sleepDuration = targetDelay - elapsed;

            if (sleepDuration.count() > 0 && !animationStopped)
            {
                std::this_thread::sleep_for(sleepDuration);
            }
        }
        animationRunning = false;
    }

public:
    FloatAnimationGui()
    {
        initializePin(pinOnOff, static_cast<MpGuiBaseMemberPtr2>(&FloatAnimationGui::onSetOnOff));
        initializePin(pinFrameNumber, static_cast<MpGuiBaseMemberPtr2>(&FloatAnimationGui::onSetAnimationPosition));
        initializePin(pinSpeed, static_cast<MpGuiBaseMemberPtr2>(&FloatAnimationGui::onSetSpeed));
        initializePin(pinMode, static_cast<MpGuiBaseMemberPtr2>(&FloatAnimationGui::onSetMode));
    }

    ~FloatAnimationGui() {
        stopAnimation();
    }
};

namespace
{
    auto r = Register<FloatAnimationGui>::withId(L"FloatAnimation");
}
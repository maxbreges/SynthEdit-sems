#include "mp_sdk_gui2.h"
#include <thread>
#include <atomic>

using namespace gmpi;

class FloatAnimationGui final : public SeGuiInvisibleBase
{
    std::atomic<bool> isRunning{ false };
    std::thread animationThread;
    float count = 0;
    bool isEnabled = false;
    int time = 0;

    void animationLoop()
    {
        while (isRunning)
        {
            if (isEnabled)
            {
                // Perform count up
                for (int i = 0; i < 100 && isEnabled; ++i)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(time));
                    count++;
                    pinFloatOut = count*0.01f;
                }
                // Perform count down
                for (int i = 0; i < 100 && isEnabled; ++i)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(time));
                    count--;
                    pinFloatOut = count * 0.01f;
                }
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }

    BoolGuiPin pinOnOff;
    FloatGuiPin pinTime;
    FloatGuiPin pinFloatOut;

public:
    FloatAnimationGui()
    {
        initializePin(pinOnOff, static_cast<MpGuiBaseMemberPtr2>(&FloatAnimationGui::onSetOnOff));
        initializePin(pinTime, static_cast<MpGuiBaseMemberPtr2>(&FloatAnimationGui::onSetTime));
        initializePin(pinFloatOut);

        isRunning = true;
        animationThread = std::thread(&FloatAnimationGui::animationLoop, this);
    }

    ~FloatAnimationGui()
    {
        isRunning = false;
        if (animationThread.joinable())
            animationThread.join();
    }

    void onSetOnOff()
    {
        isEnabled = (pinOnOff == true);
        count = 0;
    }

    void onSetTime()
    {
        time = 10 * (1.f - 1.f * (pinTime)) + 1.f;
    }
};


namespace
{
	auto r = Register<FloatAnimationGui>::withId(L"My FloatAnimation");
}

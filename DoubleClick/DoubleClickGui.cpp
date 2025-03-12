#include "mp_sdk_gui2.h"
#include <chrono>
#include <thread>
#include <mutex>    // To handle thread safety

using namespace gmpi;

class DoubleClickGui final : public SeGuiInvisibleBase
{
private:
    int count = 0;
    std::mutex mtx;
    bool processing = false;  // Flag to indicate if processing is ongoing

public:
    void onSetMouseDown()
    {
        if (pinMouseDown)
        {
            std::lock_guard<std::mutex> lock(mtx);
            ++count;

            // Update pinClickCount immediately to reflect increment
            pinClickCount = count;

            int countDown = pinTime;
            // Start countdown in a separate thread
            std::thread([this, countDown]()
                {
                  //  for (int i = countDown; i >= 0; --i)
                   // {
                        std::this_thread::sleep_for(std::chrono::milliseconds(countDown));
                        {
                            std::lock_guard<std::mutex> lock(mtx);
                           // pinIntOut = i;
                        }
                   // }

                    // Reset count when countdown is finished
                    {
                        std::lock_guard<std::mutex> lock(mtx);
                        count = 0;
                        pinClickCount = count; // Reset pinClickCount
                    }
                   
                }
            ).detach();
        }

        {
            std::lock_guard<std::mutex> lock(mtx); // Lock for thread safety
            if (pinClickCount == 2)
            {
                pinAnimPos = pinResetValue;
                pinClickCount = 0; // Reset to avoid repeated actions

                // Check if we are already processing
                if (processing)
                {
                    return; // Ignore further clicks while processing
                }

                processing = true; // Set flag to indicate processing has started
            }
            // Reset the processing flag to allow future clicks
          processing = false;
        }
    }

 	void onSetTime()	{   }

 	void onSetIntOut()	{	}

 	void onSetAnimPos()	{	}

 	void onSetResetValue()	{	}

 	BoolGuiPin pinMouseDown;
 	IntGuiPin pinTime;
 	IntGuiPin pinIntOut;
	IntGuiPin pinClickCount;
 	FloatGuiPin pinAnimPos;
 	FloatGuiPin pinResetValue;

public:
	DoubleClickGui()
	{
		initializePin( pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&DoubleClickGui::onSetMouseDown) );
		initializePin( pinTime, static_cast<MpGuiBaseMemberPtr2>(&DoubleClickGui::onSetTime) );
		initializePin( pinIntOut, static_cast<MpGuiBaseMemberPtr2>(&DoubleClickGui::onSetIntOut) );
		initializePin(pinClickCount, static_cast<MpGuiBaseMemberPtr2>(&DoubleClickGui::onSetMouseDown));
		initializePin( pinAnimPos, static_cast<MpGuiBaseMemberPtr2>(&DoubleClickGui::onSetAnimPos) );
		initializePin( pinResetValue, static_cast<MpGuiBaseMemberPtr2>(&DoubleClickGui::onSetResetValue) );
	}
};

namespace
{
	auto r = Register<DoubleClickGui>::withId(L"DoubleClick");
}

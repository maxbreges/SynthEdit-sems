#include "mp_sdk_gui2.h"
#include <chrono>
#include <thread>
//#include <mutex>    // To handle thread safety

using namespace gmpi;

class MonostableGuiGui final : public SeGuiInvisibleBase
{
private:
	int count = 0;
	//std::mutex mtx;

public:

 	void onSetMouseDown()
    {
        if (pinMouseDown)
        {
           // std::lock_guard<std::mutex> lock(mtx);
            ++count;

            // Update pinClickCount immediately to reflect increment
            pinBoolOut = count;

            int countDown = pinTime;
            // Start countdown in a separate thread
            std::thread([this, countDown]()
                {
                    for (int i = countDown; i >= 0; --i)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                        {
                           // std::lock_guard<std::mutex> lock(mtx);
                        }
                    }

                    // Reset count when countdown is finished
                    {
                      //  std::lock_guard<std::mutex> lock(mtx);
                        count = 0;
                        pinBoolOut = count; // Reset pinClickCount
                    }
                }
            ).detach();
        }

    }

 	void onSetTime()
	{
		// pinTimems changed
	}


 	BoolGuiPin pinMouseDown;
 	IntGuiPin pinTime;
 	BoolGuiPin pinBoolOut;
 	
public:
	MonostableGuiGui()
	{
		initializePin( pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&MonostableGuiGui::onSetMouseDown) );
		initializePin( pinTime, static_cast<MpGuiBaseMemberPtr2>(&MonostableGuiGui::onSetTime) );
		initializePin(pinBoolOut, static_cast<MpGuiBaseMemberPtr2>(&MonostableGuiGui::onSetMouseDown) );
	}
};

namespace
{
	auto r = Register<MonostableGuiGui>::withId(L"MonostableGui");
}

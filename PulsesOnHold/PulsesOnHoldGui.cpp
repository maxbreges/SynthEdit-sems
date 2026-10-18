#ifndef DRAWING_H
#define DRAWING_H

#include <chrono>
#include "mp_sdk_gui.h"
#include "../se_sdk3/TimerManager.h"

using namespace gmpi;

class PulsesOnHoldGui final : public SeGuiInvisibleBase, public TimerClient
{
	int timeout = 750;
	int ms = 0;
	bool hold = false;
	bool prevBool = false;

	void onSetHold()
	{
		if (pinHold)
		{
			StartTimer(timeout);
		}
		else
		{
			pinBool = false;
			StopTimer();
			hold = false;
		}

		if (pinHold && !prevBool && !pinOnMouseUp)
		{					
			pinBool = true;
			pinBool = false;			
		}

		if (!pinHold && prevBool && pinOnMouseUp)
		{
			pinBool = true;
			pinBool = false;
		}

		prevBool = pinHold;

	}

	void onSetTimeout()
	{
		timeout = pinTimeout.getValue();			
	}

	void onSetSpeed()
	{
		ms = pinSpeed.getValue();
	}

 	BoolGuiPin pinHold;
 	IntGuiPin pinTimeout;
	IntGuiPin pinSpeed;
 	BoolGuiPin pinBool;
	BoolGuiPin pinOnMouseUp;

public:
	PulsesOnHoldGui()
	{
		initializePin( pinHold, static_cast<MpGuiBaseMemberPtr2>(&PulsesOnHoldGui::onSetHold) );
		initializePin( pinTimeout, static_cast<MpGuiBaseMemberPtr2>(&PulsesOnHoldGui::onSetTimeout) );
		initializePin(pinSpeed, static_cast<MpGuiBaseMemberPtr2>(&PulsesOnHoldGui::onSetSpeed));
		initializePin( pinBool );
		initializePin(pinOnMouseUp);
	}

	bool OnTimer() override
	{
		hold = true;

		if (!pinHold)
		{
			hold = false;
			StopTimer();
		}
		
		if(pinHold && hold)
		{
			StartTimer(ms);
			pinBool = true;
			pinBool = false;
		}
		return true;
	}
#endif  // DRAWING_H
};

namespace
{
	auto r = Register<PulsesOnHoldGui>::withId(L"My PulsesOnHold");
}

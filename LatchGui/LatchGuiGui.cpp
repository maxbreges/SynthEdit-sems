#include "mp_sdk_gui2.h"

using namespace gmpi;

class LatchGuiGui final : public SeGuiInvisibleBase
{
	void onSetInput()
	{
		if (pinInput)
		{
			pinOutput = !pinOutput;
		
		pinOutputL = pinOutput;

		}
	}

 	void onSetOutput()
	{
		//pinOutputL = pinOutput;
	}

 	BoolGuiPin pinInput;
 	BoolGuiPin pinOutput;
	BoolGuiPin pinOutputL;

public:
	LatchGuiGui()
	{
		initializePin( pinInput, static_cast<MpGuiBaseMemberPtr2>(&LatchGuiGui::onSetInput) );
		initializePin( pinOutput, static_cast<MpGuiBaseMemberPtr2>(&LatchGuiGui::onSetOutput) );
		initializePin(pinOutputL);
	}

};

namespace
{
	auto r = Register<LatchGuiGui>::withId(L"LatchGui");
}

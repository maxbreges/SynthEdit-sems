#include "mp_sdk_gui2.h"

using namespace gmpi;

class LatchGuiGui final : public SeGuiInvisibleBase
{
 	void onSetInput()
	{
		if (pinInput)
		{
			pinOutput = !pinOutput;
		}
	}

 	void onSetOutput()
	{
		// pinOutput changed
	}

 	BoolGuiPin pinInput;
 	BoolGuiPin pinOutput;

public:
	LatchGuiGui()
	{
		initializePin( pinInput, static_cast<MpGuiBaseMemberPtr2>(&LatchGuiGui::onSetInput) );
		initializePin( pinOutput, static_cast<MpGuiBaseMemberPtr2>(&LatchGuiGui::onSetOutput) );
	}

};

namespace
{
	auto r = Register<LatchGuiGui>::withId(L"LatchGui");
}

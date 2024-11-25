#include "mp_sdk_gui2.h"

using namespace gmpi;

class MIDILearnGui final : public SeGuiInvisibleBase
{ 	

 	void onSetAnimationPosition()
	{
			pinGate = pinAnimationPosition;			
	}

	void onSetGate()
	{ 

	}

	void onSetGateIn()
	{
		if (pinGateIn)
			pinAnimationPosition = false;

		onSetAnimationPosition();
	}
 	
 	FloatGuiPin pinAnimationPosition;
	BoolGuiPin pinGate;
	BoolGuiPin pinGateIn;

public:
	MIDILearnGui()
	{
		initializePin( pinAnimationPosition, static_cast<MpGuiBaseMemberPtr2>(&MIDILearnGui::onSetAnimationPosition) );
		initializePin(pinGate, static_cast<MpGuiBaseMemberPtr2>(&MIDILearnGui::onSetAnimationPosition));
		initializePin(pinGateIn, static_cast<MpGuiBaseMemberPtr2>(&MIDILearnGui::onSetGateIn));
	}
};

namespace
{
	auto r = Register<MIDILearnGui>::withId(L"MIDI Learn");
}

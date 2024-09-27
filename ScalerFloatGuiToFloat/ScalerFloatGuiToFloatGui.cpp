#include "mp_sdk_gui2.h"

using namespace gmpi;

class PatchMemoryFloatGui final : public SeGuiInvisibleBase
{
	void onSetAnimationIn()
	{
		
	}

 	void onSetAnimationPosition()
	{
		pinAnimationIn = pinAnimationPosition;
	}

 	FloatGuiPin pinAnimationIn;
 	FloatGuiPin pinAnimationPosition;

public:
	PatchMemoryFloatGui()
	{
		initializePin( pinAnimationIn, static_cast<MpGuiBaseMemberPtr2>(&PatchMemoryFloatGui::onSetAnimationIn) );
		initializePin( pinAnimationPosition, static_cast<MpGuiBaseMemberPtr2>(&PatchMemoryFloatGui::onSetAnimationPosition) );
	}
};

namespace
{
	auto r = Register<PatchMemoryFloatGui>::withId(L"PatchMemoryFloat");
}

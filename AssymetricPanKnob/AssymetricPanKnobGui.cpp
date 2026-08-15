#include "mp_sdk_gui2.h"

using namespace gmpi;

class AssymetricPanKnobGui final : public SeGuiInvisibleBase
{
 	void onSetAnimationPosition()
	{
		if (pinAnimationPosition == 0.5f)
		{
			pinValue = 0.f;
		}

		if (pinAnimationPosition > 0.5f)
		{
			pinValue = (pinAnimationPosition - 0.5f) * pinHigh * 2;
		}

		if (pinAnimationPosition < 0.5f)
		{
			pinValue = (pinAnimationPosition - 0.5f) * pinLow * -2;
		}
	}

	void onSetValue()
	{
		pinValue = 0.f;
	}

 	void onSetLow()
	{
		// pinFormulaA changed
	}

 	void onSetHigh()
	{
		// pinFormulaB changed
	}

 	FloatGuiPin pinAnimationPosition;
 	FloatGuiPin pinValue;
	FloatGuiPin pinLow;
	FloatGuiPin pinHigh;

public:
	AssymetricPanKnobGui()
	{
		initializePin( pinAnimationPosition, static_cast<MpGuiBaseMemberPtr2>(&AssymetricPanKnobGui::onSetAnimationPosition) );
		initializePin( pinValue);
		initializePin( pinLow, static_cast<MpGuiBaseMemberPtr2>(&AssymetricPanKnobGui::onSetLow) );
		initializePin( pinHigh, static_cast<MpGuiBaseMemberPtr2>(&AssymetricPanKnobGui::onSetHigh) );
	}

};

namespace
{
	auto r = Register<AssymetricPanKnobGui>::withId(L"My AssymetricPanKnob");
}

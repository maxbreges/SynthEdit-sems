#include "mp_sdk_gui2.h"

using namespace gmpi;

class FloatClipperGui final : public SeGuiInvisibleBase
{
 	void onSetValueIn()
	{
		if (pinValueIn < pinMin)
		{
			pinValueOut = pinMin;
		}					
		
		if (pinValueIn > pinMax)
		{
			pinValueOut = pinMax;
		}

		pinValueOut = pinValueIn;
	}
	void onSetMin()
	{
		// pinMultiplyby changed
	}
	void onSetMax()
	{
		// pinMultiplyby changed
	}
 	void onSetValueOut()
	{		
	}

 	FloatGuiPin pinValueIn;
 	FloatGuiPin pinMin;
	FloatGuiPin pinMax;
 	FloatGuiPin pinValueOut;

public:
	FloatClipperGui()
	{
		initializePin( pinValueIn, static_cast<MpGuiBaseMemberPtr2>(&FloatClipperGui::onSetValueIn) );
		initializePin( pinMin, static_cast<MpGuiBaseMemberPtr2>(&FloatClipperGui::onSetMin) );
		initializePin(pinMax, static_cast<MpGuiBaseMemberPtr2>(&FloatClipperGui::onSetMax));
		initializePin( pinValueOut );
	}

};

namespace
{
	auto r = Register<FloatClipperGui>::withId(L"Float Clipper");
}

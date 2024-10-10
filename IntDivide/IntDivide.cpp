#include "mp_sdk_gui2.h"

using namespace gmpi;

class IntDivideGui final : public SeGuiInvisibleBase
{
 	void onSetValueIn()
	{
		pinValueOut = (float) pinValueIn / (float) pinDivide;
	}

 	void onSetDivide()
	{
		if (pinDivide == 0)
		{
			pinValueOut = 0;
		}
		else

			pinValueOut = (float)pinValueIn / (float)pinDivide;
	}

 	void onSetValueOut()
	{
		
	}

 	IntGuiPin pinValueIn;
 	IntGuiPin pinDivide;
 	FloatGuiPin pinValueOut;

public:
	IntDivideGui()
	{
		initializePin( pinValueIn, static_cast<MpGuiBaseMemberPtr2>(&IntDivideGui::onSetValueIn) );
		initializePin( pinDivide, static_cast<MpGuiBaseMemberPtr2>(&IntDivideGui::onSetDivide) );
		initializePin( pinValueOut, static_cast<MpGuiBaseMemberPtr2>(&IntDivideGui::onSetValueOut) );
	}

};

namespace
{
	auto r = Register<IntDivideGui>::withId(L"IntDivide");
}

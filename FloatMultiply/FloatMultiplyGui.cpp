#include "mp_sdk_gui2.h"

using namespace gmpi;

class FloatMultiplyGui final : public SeGuiInvisibleBase
{
	void onSetValueIn()
	{
		pinValueOut = pinValueIn.getValue() * pinMultiplyby.getValue();
	}

 	void onSetValueOut()
	{
		// pinValueOut changed
	}

 	void onSetMultiplyby()
	{
		// pinMultiplyby changed
	}
	 
	FloatGuiPin pinValueIn; 	
 	FloatGuiPin pinMultiplyby;
	FloatGuiPin pinValueOut;	

public:
	FloatMultiplyGui()
	{
		initializePin(pinValueIn, static_cast<MpGuiBaseMemberPtr2>(&FloatMultiplyGui::onSetValueIn));
		initializePin( pinMultiplyby, static_cast<MpGuiBaseMemberPtr2>(&FloatMultiplyGui::onSetMultiplyby));
		initializePin(pinValueOut, static_cast<MpGuiBaseMemberPtr2>(&FloatMultiplyGui::onSetValueOut));
	}

};

namespace
{
	auto r = Register<FloatMultiplyGui>::withId(L"Float Multiply");
}

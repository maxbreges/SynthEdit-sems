#include "mp_sdk_gui2.h"

using namespace gmpi;

class FloatAddGui final : public SeGuiInvisibleBase
{
 	void onSetValueIn()
	{
		pinValueOut = pinValueIn + pinAdd;
	}

 	void onSetAdd()
	{
		pinValueOut = pinValueIn + pinAdd;
	}

 	void onSetValueOut()
	{
		// pinValueOut changed
	}

 	FloatGuiPin pinValueIn;
 	FloatGuiPin pinAdd;
 	FloatGuiPin pinValueOut;

public:
	FloatAddGui()
	{
		initializePin( pinValueIn, static_cast<MpGuiBaseMemberPtr2>(&FloatAddGui::onSetValueIn) );
		initializePin( pinAdd, static_cast<MpGuiBaseMemberPtr2>(&FloatAddGui::onSetAdd) );
		initializePin( pinValueOut, static_cast<MpGuiBaseMemberPtr2>(&FloatAddGui::onSetValueOut) );
	}

};

namespace
{
	auto r = Register<FloatAddGui>::withId(L"Float Add");
}

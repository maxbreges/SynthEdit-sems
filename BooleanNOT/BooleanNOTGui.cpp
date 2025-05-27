#include "mp_sdk_gui2.h"

using namespace gmpi;

class BooleanNOTGui final : public SeGuiInvisibleBase
{
 	void onSetOut()
	{
		pinIn =! pinOut;
	}

 	void onSetIn()
	{
		pinOut =! pinIn;
	}

	BoolGuiPin pinIn;
 	BoolGuiPin pinOut; 	

public:
	BooleanNOTGui()
	{
		initializePin(pinIn, static_cast<MpGuiBaseMemberPtr2>(&BooleanNOTGui::onSetIn));
		initializePin( pinOut, static_cast<MpGuiBaseMemberPtr2>(&BooleanNOTGui::onSetOut) );
	}

};

namespace
{
	auto r = Register<BooleanNOTGui>::withId(L"BooleanNOT");
}

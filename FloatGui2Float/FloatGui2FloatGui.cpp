#include "mp_sdk_gui2.h"

using namespace gmpi;

class FloatGui2FloatGui final : public SeGuiInvisibleBase
{
 	void onSetChannelM1()
	{
		// pinChannelM1 changed
	}

 	void onSetChannelM1Out()
	{
		pinChannelM1 = pinChannelM1Out;
	}
	void onSetChannelLOut()
	{
		pinChannelM1 = pinChannelLOut;
	}

 	FloatGuiPin pinChannelM1;

 	FloatGuiPin pinChannelM1Out;
	FloatGuiPin pinChannelLOut;

public:
	FloatGui2FloatGui()
	{
		initializePin( pinChannelM1, static_cast<MpGuiBaseMemberPtr2>(&FloatGui2FloatGui::onSetChannelM1) );	

		initializePin( pinChannelM1Out, static_cast<MpGuiBaseMemberPtr2>(&FloatGui2FloatGui::onSetChannelM1Out) );	
		initializePin(pinChannelLOut, static_cast<MpGuiBaseMemberPtr2>(&FloatGui2FloatGui::onSetChannelLOut));
	}
};

namespace
{
	auto r = Register<FloatGui2FloatGui>::withId(L"FloatGui2Float");
}

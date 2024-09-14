#include "mp_sdk_gui2.h"

using namespace gmpi;

class Float2FloatGuiGui final : public SeGuiInvisibleBase
{
 	void onSetChannelM1()
	{
		pinFloatOut = pinChannelM1;
		pinFloatOutL = pinChannelM1;
	}

 	void onSetFloatOut()
	{
		// pinFloatIn changed
	}

 	void onSetFloatOutL()
	{
		// pinFloatInIn changed
	}

 	FloatGuiPin pinChannelM1;
 	FloatGuiPin pinFloatOut;
 	FloatGuiPin pinFloatOutL;

public:
	Float2FloatGuiGui()
	{
		initializePin( pinChannelM1, static_cast<MpGuiBaseMemberPtr2>(&Float2FloatGuiGui::onSetChannelM1) );
		initializePin( pinFloatOut, static_cast<MpGuiBaseMemberPtr2>(&Float2FloatGuiGui::onSetFloatOut) );
		initializePin( pinFloatOutL, static_cast<MpGuiBaseMemberPtr2>(&Float2FloatGuiGui::onSetFloatOutL) );
	}
};

namespace
{
	auto r = Register<Float2FloatGuiGui>::withId(L"Float2FloatGui");
}

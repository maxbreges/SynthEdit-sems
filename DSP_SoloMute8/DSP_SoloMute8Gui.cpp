#include "mp_sdk_gui2.h"

using namespace gmpi;

class FloatGui2FloatGui final : public SeGuiInvisibleBase
{
	void onSetChannelM1()
	{
		// pinChannelM1 changed
	}
	void onSetChannelM2()
	{
		// pinChannelM2 changed
	}
	void onSetChannelM3()
	{
		// pinChannelM3 changed
	}
	void onSetChannelM4()
	{
		// pinChannelM4 changed
	}
	void onSetChannelM5()
	{
		// pinChannelM1 changed
	}
	void onSetChannelM6()
	{
		// pinChannelM2 changed
	}
	void onSetChannelM7()
	{
		// pinChannelM3 changed
	}
	void onSetChannelM8()
	{
		// pinChannelM4 changed
	}

	void onSetChannelM1Out()
	{
		pinChannelM1 = pinChannelM1Out;
	}
	void onSetChannelM2Out()
	{
		pinChannelM2 = pinChannelM2Out;
	}
	void onSetChannelM3Out()
	{
		pinChannelM3 = pinChannelM3Out;
	}
	void onSetChannelM4Out()
	{
		pinChannelM4 = pinChannelM4Out;
	}
	void onSetChannelM5Out()
	{
		pinChannelM5 = pinChannelM5Out;
	}
	void onSetChannelM6Out()
	{
		pinChannelM6 = pinChannelM6Out;
	}
	void onSetChannelM7Out()
	{
		pinChannelM7 = pinChannelM7Out;
	}
	void onSetChannelM8Out()
	{
		pinChannelM8 = pinChannelM8Out;
	}


	FloatGuiPin pinChannelM1;
	FloatGuiPin pinChannelM2;
	FloatGuiPin pinChannelM3;
	FloatGuiPin pinChannelM4;
	FloatGuiPin pinChannelM5;
	FloatGuiPin pinChannelM6;
	FloatGuiPin pinChannelM7;
	FloatGuiPin pinChannelM8;


	FloatGuiPin pinChannelM1Out;
	FloatGuiPin pinChannelM2Out;
	FloatGuiPin pinChannelM3Out;
	FloatGuiPin pinChannelM4Out;
	FloatGuiPin pinChannelM5Out;
	FloatGuiPin pinChannelM6Out;
	FloatGuiPin pinChannelM7Out;
	FloatGuiPin pinChannelM8Out;


public:
	FloatGui2FloatGui()
	{
		initializePin(pinChannelM1, static_cast<MpGuiBaseMemberPtr2>(&FloatGui2FloatGui::onSetChannelM1));
		initializePin(pinChannelM2, static_cast<MpGuiBaseMemberPtr2>(&FloatGui2FloatGui::onSetChannelM2));
		initializePin(pinChannelM3, static_cast<MpGuiBaseMemberPtr2>(&FloatGui2FloatGui::onSetChannelM3));
		initializePin(pinChannelM4, static_cast<MpGuiBaseMemberPtr2>(&FloatGui2FloatGui::onSetChannelM4));
		initializePin(pinChannelM5, static_cast<MpGuiBaseMemberPtr2>(&FloatGui2FloatGui::onSetChannelM5));
		initializePin(pinChannelM6, static_cast<MpGuiBaseMemberPtr2>(&FloatGui2FloatGui::onSetChannelM6));
		initializePin(pinChannelM7, static_cast<MpGuiBaseMemberPtr2>(&FloatGui2FloatGui::onSetChannelM7));
		initializePin(pinChannelM8, static_cast<MpGuiBaseMemberPtr2>(&FloatGui2FloatGui::onSetChannelM8));

		initializePin(pinChannelM1Out, static_cast<MpGuiBaseMemberPtr2>(&FloatGui2FloatGui::onSetChannelM1Out));
		initializePin(pinChannelM2Out, static_cast<MpGuiBaseMemberPtr2>(&FloatGui2FloatGui::onSetChannelM2Out));
		initializePin(pinChannelM3Out, static_cast<MpGuiBaseMemberPtr2>(&FloatGui2FloatGui::onSetChannelM3Out));
		initializePin(pinChannelM4Out, static_cast<MpGuiBaseMemberPtr2>(&FloatGui2FloatGui::onSetChannelM4Out));
		initializePin(pinChannelM5Out, static_cast<MpGuiBaseMemberPtr2>(&FloatGui2FloatGui::onSetChannelM5Out));
		initializePin(pinChannelM6Out, static_cast<MpGuiBaseMemberPtr2>(&FloatGui2FloatGui::onSetChannelM6Out));
		initializePin(pinChannelM7Out, static_cast<MpGuiBaseMemberPtr2>(&FloatGui2FloatGui::onSetChannelM7Out));
		initializePin(pinChannelM8Out, static_cast<MpGuiBaseMemberPtr2>(&FloatGui2FloatGui::onSetChannelM8Out));

	}
};

namespace
{
	auto r = Register<FloatGui2FloatGui>::withId(L"mxSoloMute4");
}

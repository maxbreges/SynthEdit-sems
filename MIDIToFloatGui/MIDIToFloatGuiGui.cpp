#include "mp_sdk_gui2.h"

using namespace gmpi;

class MIDIToIntGui final : public::SeGuiInvisibleBase
{
	void onSetChan()
	{
		pinChan = InpinChan;
	}
	void onSetNote()
	{
		pinNote = InpinNote;
	}
	void onSetVelocity()
	{
		pinVelocity = InpinVelocity;
	}
	void onSetStatus()
	{
		pinStatus = InpinStatus;
	}

	FloatGuiPin InpinChan;
	FloatGuiPin InpinNote;
	FloatGuiPin InpinVelocity;
	FloatGuiPin InpinStatus;

	FloatGuiPin pinChan;
	FloatGuiPin pinNote;
	FloatGuiPin pinVelocity;
	FloatGuiPin pinStatus;

public:

	MIDIToIntGui()
	{
		initializePin(InpinChan, static_cast<MpGuiBaseMemberPtr2>(&MIDIToIntGui::onSetChan));
		initializePin(InpinNote, static_cast<MpGuiBaseMemberPtr2>(&MIDIToIntGui::onSetNote));
		initializePin(InpinVelocity, static_cast<MpGuiBaseMemberPtr2>(&MIDIToIntGui::onSetVelocity));
		initializePin(InpinStatus, static_cast<MpGuiBaseMemberPtr2>(&MIDIToIntGui::onSetStatus));

		initializePin(pinChan, static_cast<MpGuiBaseMemberPtr2>(&MIDIToIntGui::onSetChan));
		initializePin(pinNote, static_cast<MpGuiBaseMemberPtr2>(&MIDIToIntGui::onSetNote));
		initializePin(pinVelocity, static_cast<MpGuiBaseMemberPtr2>(&MIDIToIntGui::onSetVelocity));
		initializePin(pinStatus, static_cast<MpGuiBaseMemberPtr2>(&MIDIToIntGui::onSetStatus));
	}

};

namespace
{
	auto r = Register<MIDIToIntGui>::withId(L"MIDIToFloatGui");
}
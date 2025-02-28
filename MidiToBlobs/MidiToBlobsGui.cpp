#include "mp_sdk_gui2.h"

using namespace gmpi;

class MidiToBlobsGui final : public SeGuiInvisibleBase
{
	void onSetNoteIn()
	{
		pinX = pinXIn;
		pinY = pinYIn;
	//	pinDebugOut = pinDebug;
	}

	BlobGuiPin pinXIn;
	BlobGuiPin pinYIn;
	BlobGuiPin pinX;
	BlobGuiPin pinY;
	//StringGuiPin pinDebug;
	//StringGuiPin pinDebugOut;

public:
	MidiToBlobsGui()
	{
		initializePin(pinXIn, static_cast<MpGuiBaseMemberPtr2>(&MidiToBlobsGui::onSetNoteIn));
		initializePin(pinYIn, static_cast<MpGuiBaseMemberPtr2>(&MidiToBlobsGui::onSetNoteIn));
		initializePin(pinX, static_cast<MpGuiBaseMemberPtr2>(&MidiToBlobsGui::onSetNoteIn));
		initializePin(pinY, static_cast<MpGuiBaseMemberPtr2>(&MidiToBlobsGui::onSetNoteIn));
	//	initializePin(pinDebug, static_cast<MpGuiBaseMemberPtr2>(&MidiToBlobsGui::onSetNoteIn));
	//	initializePin(pinDebugOut, static_cast<MpGuiBaseMemberPtr2>(&MidiToBlobsGui::onSetNoteIn));
	}
};

namespace
{
	auto r = Register<MidiToBlobsGui>::withId(L"MidiToBlobs");
}
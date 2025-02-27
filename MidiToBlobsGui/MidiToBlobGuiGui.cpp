#include "mp_sdk_gui2.h"

using namespace gmpi;

class MidiToBlobsGui final : public SeGuiInvisibleBase
{
	void onSetNoteIn()
	{
		pinX = pinXIn;
		pinY = pinYIn;
	}

	BlobGuiPin pinXIn;
	BlobGuiPin pinYIn;
	BlobGuiPin pinX;
	BlobGuiPin pinY;

public:
	MidiToBlobsGui()
	{
		initializePin(pinXIn, static_cast<MpGuiBaseMemberPtr2>(&MidiToBlobsGui::onSetNoteIn));
		initializePin(pinYIn, static_cast<MpGuiBaseMemberPtr2>(&MidiToBlobsGui::onSetNoteIn));
		initializePin(pinX, static_cast<MpGuiBaseMemberPtr2>(&MidiToBlobsGui::onSetNoteIn));
		initializePin(pinY, static_cast<MpGuiBaseMemberPtr2>(&MidiToBlobsGui::onSetNoteIn));
	}
};

namespace
{
	auto r = Register<MidiToBlobsGui>::withId(L"MidiToBlobs");
}
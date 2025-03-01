#include "mp_sdk_gui2.h"

using namespace gmpi;

class SetPinGui final : public SeGuiInvisibleBase
{
	//std::vector<int> activeNotes; // Store the currently active notes
	//std::vector<int> activeChannels; // Store the currently active notes

 	void onSetPin()
	{
/*		int n_items = pinX.rawSize() / sizeof(int);
		if (n_items <= 0) return;

		// Resize the activeNotes vector to hold the incoming chord notes
		activeNotes.resize(n_items);
		int* noteData = (int*)pinX.rawData();

		// Store all received note numbers in activeNotes
		for (int i = 0; i < n_items; ++i) {
			activeNotes[i] = noteData[i];
		}*/
		pinXOut = pinX;
		pinYOut = pinY;
	}


 	BlobGuiPin pinX;
	BlobGuiPin pinY;
	BlobGuiPin pinXOut;
	BlobGuiPin pinYOut;

public:
	SetPinGui()
	{
		initializePin( pinX, static_cast<MpGuiBaseMemberPtr2>(&SetPinGui::onSetPin) );
		initializePin( pinY, static_cast<MpGuiBaseMemberPtr2>(&SetPinGui::onSetPin) );
		initializePin( pinXOut, static_cast<MpGuiBaseMemberPtr2>(&SetPinGui::onSetPin) );
		initializePin( pinYOut, static_cast<MpGuiBaseMemberPtr2>(&SetPinGui::onSetPin) );
	}
};

namespace
{
	auto r = Register<SetPinGui>::withId(L"setPin");
}

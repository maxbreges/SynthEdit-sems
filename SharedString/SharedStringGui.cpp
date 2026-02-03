#include "mp_sdk_gui2.h"

using namespace gmpi;

class SharedStringGui final : public SeGuiInvisibleBase
{
 	void onSetFilenameIn()
	{
		// pinFilenameIn changed
	}

 	void onSetTextOut()
	{
		// pinTextOut changed
	}

 	void onSetBoolIn()
	{
		// pinBoolIn changed
	}

 	StringGuiPin pinFilenameIn;
 	StringGuiPin pinTextOut;
 	BoolGuiPin pinBoolIn;

public:
	SharedStringGui()
	{
		initializePin( pinFilenameIn, static_cast<MpGuiBaseMemberPtr2>(&SharedStringGui::onSetFilenameIn) );
		initializePin( pinTextOut, static_cast<MpGuiBaseMemberPtr2>(&SharedStringGui::onSetTextOut) );
		initializePin( pinBoolIn, static_cast<MpGuiBaseMemberPtr2>(&SharedStringGui::onSetBoolIn) );
	}

};

namespace
{
	auto r = Register<SharedStringGui>::withId(L"sharedString");
}

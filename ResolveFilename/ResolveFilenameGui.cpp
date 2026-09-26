#include "mp_sdk_gui2.h"

using namespace gmpi;

class ResolveFilenameGui final : public SeGuiInvisibleBase
{
	int Init = false;

	void onSetFromDSP()
	{
		pinInit = Init;
		pinCommonPath = pinFromDSP;
		Init = true;
		pinInit = Init;
		onSetReset();
	}

	StringGuiPin pinFromDSP;
	StringGuiPin pinCommonPath;
	BoolGuiPin pinInit;
	BoolGuiPin pinReset;

public:
	ResolveFilenameGui()
	{	initializePin(pinFromDSP, static_cast<MpGuiBaseMemberPtr2>(&ResolveFilenameGui::onSetFromDSP));
		initializePin(pinCommonPath);
		initializePin(pinInit, static_cast<MpGuiBaseMemberPtr2>(&ResolveFilenameGui::onSetFromDSP));
		initializePin(pinReset, static_cast<MpGuiBaseMemberPtr2>(&ResolveFilenameGui::onSetReset));
	}
	void onSetReset()
	{
		pinInit = pinReset;
	}
};

namespace
{
	auto r = Register<ResolveFilenameGui>::withId(L"resolveFilename");
}

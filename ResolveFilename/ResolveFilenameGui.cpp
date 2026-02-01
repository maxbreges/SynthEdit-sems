#include "mp_sdk_gui2.h"

using namespace gmpi;

class ResolveFilenameGui final : public SeGuiInvisibleBase
{
	void onSetFromDSP()
	{
		pinCommonPath = pinFromDSP;
	}

	StringGuiPin pinFromDSP;
	StringGuiPin pinCommonPath;

public:
	ResolveFilenameGui()
	{	initializePin(pinFromDSP, static_cast<MpGuiBaseMemberPtr2>(&ResolveFilenameGui::onSetFromDSP));
		initializePin(pinCommonPath);
	}
};

namespace
{
	auto r = Register<ResolveFilenameGui>::withId(L"resolveFilename");
}

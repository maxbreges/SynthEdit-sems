#include "mp_sdk_gui2.h"

using namespace gmpi;

class ExtractExtGui final : public SeGuiInvisibleBase
{
	// Helper: Get extension 
	std::wstring getExtension(const std::wstring& ext)
	{
		size_t dotPos = ext.find_last_of('.');
		if (dotPos != std::wstring::npos)
			return ext.substr(dotPos + 1);
		return ext;
	}

	void onSetFile()
	{
		pinExtension = getExtension(pinFile);
	}

 	StringGuiPin pinFile;
	StringGuiPin pinExtension;


public:
	ExtractExtGui()
	{
		initializePin( pinFile, static_cast<MpGuiBaseMemberPtr2>(&ExtractExtGui::onSetFile) );
		initializePin( pinExtension );
	}

};

namespace
{
	auto r = Register<ExtractExtGui>::withId(L"My ExtractExt");
}

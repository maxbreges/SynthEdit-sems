#include "mp_sdk_audio.h"
#include "../shared/string_utilities.h"//for StripExtension

using namespace gmpi;

class SharedString final : public MpBase2
{	
	StringInPin pinFileName;
	StringOutPin pinAppDirPathOut;
	StringOutPin pinAppDirPathOut2Gui;

public:
	SharedString()
	{
		initializePin(pinFileName);
		initializePin(pinAppDirPathOut);
		initializePin(pinAppDirPathOut2Gui);
	}
	
	void onSetPins() override
	{		
		std::wstring filename = StripExtension(pinFileName) + L".txt";
		// Step 1: Resolve filename to full path
		wchar_t fullFilename[500];
		getHost()->resolveFilename(filename.c_str(), sizeof(fullFilename) / sizeof(fullFilename[0]), fullFilename);

		std::wstring fullPath(fullFilename);

		pinAppDirPathOut = fullPath;
	}
};

namespace
{
	auto r = Register<SharedString>::withId(L"sharedString");
}

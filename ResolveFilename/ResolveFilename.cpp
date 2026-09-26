#include "mp_sdk_audio.h"

using namespace gmpi;

class ResolveFilename final : public MpBase2
{
	StringInPin pinFileName;
	StringOutPin pinAppDirPathOut;
	StringOutPin pinAppDirPathOut2Gui;
	StringOutPin pinDebug;

public:
	ResolveFilename()
	{
		initializePin(pinFileName);
		initializePin( pinAppDirPathOut );
		initializePin( pinAppDirPathOut2Gui );
		initializePin(pinDebug);
	}
	
	void onSetPins() override
	{
		std::wstring filename = pinFileName.getValue() + L".txt";
		// Step 1: Resolve filename to full path
		wchar_t fullFilename[MAX_PATH];
		getHost()->resolveFilename(filename.c_str(), sizeof(fullFilename) / sizeof(fullFilename[0]), fullFilename);
		
		std::wstring fullPath(fullFilename);
		pinDebug = fullPath;
		// Step 2: Extract directory path
		size_t lastSlashPos = fullPath.find_last_of(L"\\/");

		std::wstring folderPath;

		if (lastSlashPos != std::wstring::npos) {
			// Get only the directory part
			folderPath = fullPath.substr(0, lastSlashPos);
		}
		else {
			// If no slash found, fallback or set to empty
			folderPath = L"";
		}

		// Now, 'folderPath' contains the directory path only
		// You can use it to create files later, or just to get the folder path
#if defined(_WIN32)
		pinAppDirPathOut = folderPath + (L"\\");
		pinAppDirPathOut2Gui = pinAppDirPathOut;
#elif defined(__APPLE__)
		pinAppDirPathOut = folderPath + (L"/");
		pinAppDirPathOut2Gui = pinAppDirPathOut;
#endif
		
	}
};

namespace
{
	auto r = Register<ResolveFilename>::withId(L"resolveFilename");
}

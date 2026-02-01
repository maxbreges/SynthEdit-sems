#include "mp_sdk_audio.h"
#include "../shared/string_utilities.h"
#include <string>

using namespace gmpi;

class ResolveFilename final : public MpBase2
{
	StringInPin pinFileName;
	StringOutPin pinCommonPathOut;
	StringOutPin pinCommonPathOut2Gui;

public:
	ResolveFilename()
	{
		initializePin(pinFileName);
		initializePin( pinCommonPathOut );
		initializePin( pinCommonPathOut2Gui );
	}

	void onSetPins() override
	{
		std::wstring filename = StripExtension(pinFileName) + L".txt";
		// Step 1: Resolve filename to full path
		wchar_t fullFilename[500];
		getHost()->resolveFilename(filename.c_str(), sizeof(fullFilename) / sizeof(fullFilename[0]), fullFilename);

		std::wstring fullPath(fullFilename);

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
		pinCommonPathOut2Gui = pinCommonPathOut = folderPath + (L"\\");
	}
};

namespace
{
	auto r = Register<ResolveFilename>::withId(L"resolveFilename");
}

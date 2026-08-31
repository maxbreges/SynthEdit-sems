#include "mp_sdk_gui2.h"
#include "mp_gui.h"
#include <fstream>
#include <sstream>

using namespace gmpi;

class SetContentFolderGui final : public SeGuiInvisibleBase
{
	bool m_prev_trigger = false;
	std::string backslash;
	std::wstring wbackslash;

	bool onSelectFolder = false;

	StringGuiPin pinAppDirPath;
	BoolGuiPin pinTrigger;
	StringGuiPin pinDirectoryPath;
	BoolGuiPin pinBackslash;

public:
	SetContentFolderGui()
	{
		initializePin(pinAppDirPath, static_cast<MpGuiBaseMemberPtr2>(&SetContentFolderGui::onSetAppDirPath));
		initializePin(pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&SetContentFolderGui::onSetTrigger));
		initializePin(pinDirectoryPath);
		initializePin(pinBackslash, static_cast<MpGuiBaseMemberPtr2>(&SetContentFolderGui::onSetBackslash));
	}

	void onSetBackslash()
	{
		if (pinBackslash)
		{
#if defined(_WIN32)
			wbackslash = L'\\';
#elif defined(__APPLE__)
			backslash = "/";
#endif
		}
	}	

	void onSetTrigger()
	{
		if (!pinTrigger && m_prev_trigger == true)
		{
			selectFolder();
		}
		m_prev_trigger = pinTrigger;
	}

#if defined(_WIN32)
	std::wstring wfileContent; //user custom path
#elif defined(__APPLE__)
	std::string fileContent; //user custom path
#endif
	

	void onSetAppDirPath()
	{
		if (!onSelectFolder)
		{
			pinDirectoryPath = pinAppDirPath;
		}
		else
		{
#if defined(_WIN32)
			pinDirectoryPath = wfileContent;
#elif defined(__APPLE__)
			pinDirectoryPath = fileContent;
#endif			
		}
	}


private:
	void selectFolder()
	{
#if defined(_WIN32)
		selectFolderWindows();
#elif defined(__APPLE__)
		selectFolderMac();
#endif
	}

	void selectFolderWindows(); // Declaration
	void selectFolderMac();     // Declaration
};

// Platform-specific implementations

#ifdef _WIN32
#include <windows.h>
#include <shobjidl.h>
#include <objbase.h>

#include <shlobj.h> // For SHCreateItemFromParsingName

void SetContentFolderGui::selectFolderWindows()
{
	// Initialize COM
	HRESULT hr = CoInitialize(nullptr);
	if (FAILED(hr))
		return; // COM init failed

	IFileOpenDialog* pFileOpen = nullptr;
	hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileOpen));
	if (SUCCEEDED(hr))
	{
		DWORD dwFlags;
		pFileOpen->GetOptions(&dwFlags);
		pFileOpen->SetOptions(dwFlags | FOS_PICKFOLDERS);

		// Set initial folder if pinAppDirPath is valid
		if (!pinAppDirPath.getValue().empty())
		{
			IShellItem* pInitialFolder = nullptr;
			hr = SHCreateItemFromParsingName(
				std::wstring(pinAppDirPath).c_str(),
				nullptr,
				IID_PPV_ARGS(&pInitialFolder));
			if (SUCCEEDED(hr))
			{
				pFileOpen->SetFolder(pInitialFolder);
				pInitialFolder->Release();
			}
		}

		hr = pFileOpen->Show(nullptr);
		if (SUCCEEDED(hr))
		{
			IShellItem* pItem = nullptr;
			hr = pFileOpen->GetResult(&pItem);
			if (SUCCEEDED(hr))
			{
				PWSTR pszFilePath = nullptr;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
				if (SUCCEEDED(hr))
				{
					// Append backslash if needed
					wfileContent = std::wstring(pszFilePath) + wbackslash;
					onSelectFolder = true;
					onSetAppDirPath();
					CoTaskMemFree(pszFilePath);
				}
				pItem->Release();
			}
		}
		pFileOpen->Release();
	}
	CoUninitialize();
}
#endif

#ifdef __APPLE__
// macOS implementation using system call to 'osascript'
#include <cstdio>

void SetContentFolderGui::selectFolderMac()
{
	if (pinAppDirPath.getValue().empty())
	{
		// No initial path specified; fallback to default
		const char* command = "osascript -e 'POSIX path of (choose folder)'";
		FILE* pipe = popen(command, "r");
		if (!pipe) return;

		char buffer[1024];
		std::string result;
		if (fgets(buffer, sizeof(buffer), pipe))
		{
			result = buffer;
			if (!result.empty() && result.back() == '\n')
				result.pop_back();

			fileContent = result;
			onSelectFolder = true;
			onSetAppDirPath();
		}
		pclose(pipe);
	}
	else
	{
		// Use AppleScript with default location
		std::string initialPath = pinAppDirPath; // Assuming pinAppDirPath contains POSIX path
		// Escape double quotes in path
		size_t pos = 0;
		while ((pos = initialPath.find("\"", pos)) != std::string::npos)
		{
			initialPath.replace(pos, 1, "\\\"");
			pos += 2;
		}

		std::string script = "osascript -e 'tell application \"Finder\"' -e 'set folderPath to POSIX file \"" + initialPath + "\"' -e 'set folderRef to folder folderPath' -e 'choose folder default location:folderRef' -e 'end tell'";
		// Run the script
		FILE* pipe = popen(script.c_str(), "r");
		if (!pipe) return;

		char buffer[1024];
		std::string result;
		if (fgets(buffer, sizeof(buffer), pipe))
		{
			result = buffer;
			if (!result.empty() && result.back() == '\n')
				result.pop_back();

			fileContent = result;
			onSelectFolder = true;
			onSetAppDirPath();
		}
		pclose(pipe);
	}
}
#endif

namespace
{
	auto r = Register<SetContentFolderGui>::withId(L"SetContentFolder");
}

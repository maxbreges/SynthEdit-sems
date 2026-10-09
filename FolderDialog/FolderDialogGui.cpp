#include "mp_sdk_gui2.h"

using namespace gmpi;

class FolderDialogGui final : public SeGuiInvisibleBase
{
    bool m_prev_trigger = false;
    std::string backslash;
    std::wstring wbackslash;

    std::string previousString;

    std::string defaultFolder;

    void onSetTrigger()
    {
        // When trigger pin is set, open folder dialog
        if (!pinTrigger && m_prev_trigger == true)
        {
            selectFolder();
        }
        m_prev_trigger = pinTrigger;
    }

    StringGuiPin pinFolderName;
    BoolGuiPin pinTrigger;
    BoolGuiPin pinBackslash;
    BoolGuiPin pinState;
    StringGuiPin pinFolderToOpen;
    BoolGuiPin pinFolderChangedTrig;

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

public:
    FolderDialogGui()
    {
        initializePin(pinFolderName, static_cast<MpGuiBaseMemberPtr2>(&FolderDialogGui::onSetFolderName));
        initializePin(pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&FolderDialogGui::onSetTrigger));
        initializePin(pinBackslash, static_cast<MpGuiBaseMemberPtr2>(&FolderDialogGui::onSetBackslash));
        initializePin(pinState);
        initializePin(pinFolderToOpen, static_cast<MpGuiBaseMemberPtr2>(&FolderDialogGui::onSetDefaultFolder));
        initializePin(pinFolderChangedTrig);
    }

    void onSetDefaultFolder()
    {
        defaultFolder = pinFolderToOpen;
    }

    void onSetFolderName()
    {
        if ((!pinTrigger && m_prev_trigger == true)&&(previousString.compare(0, pinFolderName.getValue().size(), pinFolderName) != 0))
        {
            pinFolderChangedTrig = true;
        }
        pinFolderChangedTrig = false;
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

void FolderDialogGui::selectFolderWindows()
{
    HRESULT hr = CoInitialize(nullptr);
    if (FAILED(hr))
        return;

    IFileOpenDialog* pFileOpen = nullptr;
    hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileOpen));
    if (SUCCEEDED(hr))
    {
        // Check if pinFolderToOpen has a value
        std::wstring folderToOpen = pinFolderToOpen.getValue();
        if (!folderToOpen.empty())
        {
            IShellItem* pFolderItem = nullptr;
            hr = SHCreateItemFromParsingName(folderToOpen.c_str(), nullptr, IID_PPV_ARGS(&pFolderItem));
            if (SUCCEEDED(hr))
            {
                pFileOpen->SetFolder(pFolderItem);
                pFolderItem->Release();
            }
        }

        pinState = true;
        DWORD dwFlags;
        pFileOpen->GetOptions(&dwFlags);
        pFileOpen->SetOptions(dwFlags | FOS_PICKFOLDERS);

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
                    pinFolderName = std::wstring(pszFilePath) + wbackslash;
                    previousString = pinFolderName;
                    CoTaskMemFree(pszFilePath);
                }
                pItem->Release();
            }
        }
        pFileOpen->Release();
        pinState = false;
    }
    CoUninitialize();
}
#endif

#ifdef __APPLE__
// macOS implementation using system call to 'osascript'
#include <cstdio>

void FolderDialogGui::selectFolderMac()
{
    const char* default_folder = defaultFolder.c_str();
    char command[512];
    snprintf(command, sizeof(command),
        "osascript -e 'set folder to (choose folder with prompt \"Select a folder\" default location (POSIX file \"%s\"))' -e 'display dialog folder'", default_folder);

    FILE* pipe = popen(command, "r");

    if (!pipe) return;
    pinState = true;
    char buffer[1024]; // larger buffer for longer paths
    std::string result;
    if (fgets(buffer, sizeof(buffer), pipe))
    {
        // Remove trailing newline
        if (!result.empty() && result.back() == '\n')
        {
            result.pop_back();
        }

        // Remove any quotes around the path
        if (result.size() > 1 && result[0] == '"' && result[result.size() - 1] == '"')
        {
            result = result.substr(1, result.size() - 2);
        }

        // Store the result
        // Here, you can do something with the selected folder path, like set it to a variable
        // For example:
        selectedFolder = result;
    }
    pclose(pipe);
}

#endif

namespace
{
    auto r = Register<FolderDialogGui>::withId(L"mxFolderDialog");
}

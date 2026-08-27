#include "mp_sdk_gui2.h"

using namespace gmpi;

class FolderDialogGui final : public SeGuiInvisibleBase
{
    bool m_prev_trigger = false;
    std::string backslash;
    std::wstring wbackslash;

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
        initializePin(pinFolderName);
        initializePin(pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&FolderDialogGui::onSetTrigger));
        initializePin(pinBackslash, static_cast<MpGuiBaseMemberPtr2>(&FolderDialogGui::onSetBackslash));
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
                    // Update pinFolderName with selected path
                    pinFolderName = std::wstring(pszFilePath) + wbackslash;
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

void FolderDialogGui::selectFolderMac()
{
    const char* command = "osascript -e 'POSIX path of (choose folder)'";
    FILE* pipe = popen(command, "r");
    if (!pipe) return;

    char buffer[1024]; // larger buffer for longer paths
    std::string result;
    if (fgets(buffer, sizeof(buffer), pipe))
    {
        result = buffer;
        // Remove trailing newline
        if (!result.empty() && result.back() == '\n')
            result.pop_back();

        // Update pinFolderName
        pinFolderName = result;
    }
    pclose(pipe);
}
#endif

namespace
{
    auto r = Register<FolderDialogGui>::withId(L"mxFolderDialog");
}

#include "mp_gui.h"

using namespace gmpi;

#ifdef _WIN32
#include <windows.h>
#include <shobjidl.h> // For IFileOpenDialog, FOS_PICKFOLDERS
#include <objbase.h>  // For CoInitialize, CoUninitialize
#else
#include <string>
#import <Cocoa/Cocoa.h>
#endif

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

public:
    FolderDialogGui()
    {
        initializePin(pinFolderName);
        initializePin(pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&FolderDialogGui::onSetTrigger));
        initializePin(pinBackslash, static_cast<MpGuiBaseMemberPtr2>(&FolderDialogGui::onSetBackslash));
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
        else
        {
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

    void selectFolderWindows(); // Declaration only
    void selectFolderMac();     // Declaration only
};

// Now define the platform-specific functions outside the class

#ifdef _WIN32
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
                    // Update pinFolderName
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
// Your macOS implementation remains the same
void FolderDialogGui::selectFolderMac()
{
    @autoreleasepool{
        NSOpenPanel * panel = [NSOpenPanel openPanel];
        [panel setCanChooseDirectories : YES] ;
        [panel setCanChooseFiles : NO] ;
        [panel setAllowsMultipleSelection : NO] ;

        if ([panel runModal] == NSFileHandlingPanelOKButton) {
            NSURL* url = [[panel URLs]firstObject];
            if (url) {
                NSString* path = [url path];
                // Convert NSString to std::string
                std::string folderPath([path UTF8String]);
                pinFolderName = folderPath + backslash;
            }
        }
    }
}
#endif

namespace
{
	auto r = Register<FolderDialogGui>::withId(L"mxFolderDialog");
}

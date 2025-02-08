#include "mp_sdk_gui2.h"
#include <Windows.h>
#include <shlobj.h>
#include <string>

using namespace gmpi;

class SetFolderGui final : public SeGuiInvisibleBase
{
    bool m_prev_trigger = false;

    void onSetTrigger()
    {      

        if (pinTrigger == false && m_prev_trigger == true) // dialog triggered on mouse-up (else dialog grabs focus, button never resets)
        {

            // Open folder dialog
            BROWSEINFO bi = { 0 };
            bi.lpszTitle = L"Select a Folder";
            bi.ulFlags = BIF_NEWDIALOGSTYLE; // Allows new style if you want

            // To set a default folder path, you can specify bi.pszDisplayName or use one of the existing folder IDs.
            // bi.pszDisplayName = <your default folder here, if needed>;

            // Show the dialog
            LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

            if (pidl != nullptr)
            {
                // Get the path of the selected folder
                TCHAR path[MAX_PATH];
                if (SHGetPathFromIDList(pidl, path))
                {
                    std::wstring wPath(path); // Create a wstring from TCHAR path
                    if (pinBackslash)
                    {
                        wPath += L"\\"; // Append backslash
                        pinDirectory = (wPath.c_str()); // Set the pin value
                    }
                    else
                    {
                        pinDirectory = (wPath.c_str()); // Set the pin value
                    }
                }

                // Free the PIDL allocated by SHBrowseForFolder
                CoTaskMemFree(pidl);
            }
        }
        // Update the previous trigger state for the next event
        m_prev_trigger = pinTrigger;
    }

    void onSetDirectory()
    {
        // Handle any logic related to setting the directory, if needed.
    }

    void onSetBackslash()
    {

    }

    BoolGuiPin pinTrigger;
    StringGuiPin pinDirectory;
    BoolGuiPin pinBackslash;

public:
    SetFolderGui()
    {
        initializePin(pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&SetFolderGui::onSetTrigger));
        initializePin(pinDirectory, static_cast<MpGuiBaseMemberPtr2>(&SetFolderGui::onSetDirectory));
        initializePin(pinBackslash, static_cast<MpGuiBaseMemberPtr2>(&SetFolderGui::onSetBackslash));
    }
};

namespace
{
    auto r = Register<SetFolderGui>::withId(L"SetFolder");
}
#include "mp_gui.h"
#include <string>
#include <vector>
#include <algorithm> // for transform
#include <sstream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <filesystem>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

// Helper function to extract directory path from a full file path
std::string getDirectoryFromPath(const std::string& filepath)
{
    size_t p = filepath.find_last_of("/\\");
    if (p != std::string::npos)
        return filepath.substr(0, p);
    else
        return ""; // No directory part found
}

using namespace gmpi;
using namespace gmpi_gui;
//namespace fs = std::filesystem;

class JustPathXXGui final : public SeGuiInvisibleBase
{
    bool m_prev_trigger = false;
    GmpiGui::FileDialog nativeFileDialog;
    std::string fileNameString;

    void onSetChoice()
    {
        // pinDirectory changed; handle as needed
    }

    void onSetTrigger()
    {
        // Check if the trigger has just been set (mouse button released)
        if (pinTrigger == false && m_prev_trigger == true)
        {
            IMpGraphicsHost* dialogHost = 0;
            getHost()->queryInterface(SE_IID_GRAPHICS_HOST, reinterpret_cast<void**>(&dialogHost));

            if (dialogHost != 0)
            {
                dialogHost->createFileDialog(0, nativeFileDialog.GetAddressOf());

                if (!nativeFileDialog.isNull())
                {
                    nativeFileDialog.ShowAsync([this](int32_t result) -> void { this->OnFileDialogComplete(result); });
                }
            }
        }

        m_prev_trigger = pinTrigger;
    }

    BoolGuiPin pinTrigger;
    StringGuiPin pinAllowedExtensions;
    BoolGuiPin pinHideExtensions;
    IntGuiPin pinChoice;//index
    StringGuiPin pinItemList;
    StringGuiPin pinFilename;

public:
    JustPathXXGui()
    {
        initializePin(pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&JustPathXXGui::onSetTrigger));
        initializePin(pinAllowedExtensions);
        initializePin(pinHideExtensions);
        initializePin(pinChoice, static_cast<MpGuiBaseMemberPtr2>(&JustPathXXGui::onSetChoice));
        initializePin(pinItemList);
        initializePin(pinFilename);
    }

    int32_t OnFileDialogComplete(int32_t result)
    {
        if (result == gmpi::MP_OK)
        {
            fileNameString = nativeFileDialog.GetSelectedFilename();

            // Replace filesystem path extraction with manual string manipulation
            std::string directoryPath = getDirectoryFromPath(fileNameString);
            
            pinFilename = fileNameString;
        }

        nativeFileDialog.setNull(); // Release it.

        return 0;
    }
};

namespace
{
    auto r = Register<JustPathXXGui>::withId(L"JustPathXX");
}
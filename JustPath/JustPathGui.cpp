#include "mp_sdk_gui2.h"
#include "mp_gui.h"
//#include <filesystem> // Removed for cross-platform compatibility

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

class JustPathGui final : public SeGuiInvisibleBase
{
    bool m_prev_trigger = false;
    GmpiGui::FileDialog nativeFileDialog;
    std::string fileNameString;

    void onSetDirectory()
    {
        // pinDirectory changed; handle as needed
    }

    void onSetTrigger()
    {
        // Check if the trigger has just been set (mouse button released)
        if (pinTrigger == false && m_prev_trigger == true)
        {
            IMpGraphicsHostBase* dialogHost = 0;
            getHost()->queryInterface(SE_IID_GRAPHICS_HOST_BASE, reinterpret_cast<void**>(&dialogHost));

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
    StringGuiPin pinDirectory;
    StringGuiPin pinFilename;

public:
    JustPathGui()
    {
        initializePin(pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&JustPathGui::onSetTrigger));
        initializePin(pinDirectory, static_cast<MpGuiBaseMemberPtr2>(&JustPathGui::onSetDirectory));
        initializePin(pinFilename);
    }

    int32_t OnFileDialogComplete(int32_t result)
    {
        if (result == gmpi::MP_OK)
        {
            fileNameString = nativeFileDialog.GetSelectedFilename();

            // Replace filesystem path extraction with manual string manipulation
            std::string directoryPath = getDirectoryFromPath(fileNameString);

            pinDirectory = directoryPath; // Assign the extracted directory path
            pinFilename = fileNameString;
        }

        nativeFileDialog.setNull(); // Release it.

        return 0;
    }
};

namespace
{
    auto r = Register<JustPathGui>::withId(L"JustPath");
}
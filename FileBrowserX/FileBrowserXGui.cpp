#include "mp_sdk_gui2.h"
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

using namespace gmpi;
using namespace gmpi_gui;
namespace fs = std::filesystem;

class FileBrowserXGui final : public SeGuiInvisibleBase
{
    bool m_prev_trigger = false;
    GmpiGui::FileDialog nativeFileDialog;
    std::string fileNameString;
    std::string directoryPath;

    void onSetTrigger()
    {
        // Check if the trigger has just been set (mouse button released)
        if (pinTrigger == false && m_prev_trigger == true)
        {
            IMpGraphicsHost* dialogHost = 0;
            getHost()->queryInterface(SE_IID_GRAPHICS_HOST, reinterpret_cast<void**>(&dialogHost));

            dialogHost->createFileDialog(0, nativeFileDialog.GetAddressOf());

            if (!nativeFileDialog.isNull())
            {
                nativeFileDialog.ShowAsync([this](int32_t result) -> void { this->OnFileDialogComplete(result); });
            }
        }

        m_prev_trigger = pinTrigger;
    }

    // Helper function to extract directory path from a full file path
    std::string getDirectoryFromPath(const std::string& filepath)
    {
        size_t p = filepath.find_last_of("/\\");
        if (p != std::string::npos)
            return filepath.substr(0, p);
        else
            return ""; // No directory part found
    }

    int32_t OnFileDialogComplete(int32_t result)
    {
        if (result == gmpi::MP_OK)
        {
            fileNameString = nativeFileDialog.GetSelectedFilename();

            // Replace filesystem path extraction with manual string manipulation
            std::string directoryPath = getDirectoryFromPath(fileNameString);

            pinFileNameOut = fileNameString;
        }

        nativeFileDialog.setNull(); // Release it.
        listFilesInDirectory();

        return 0;
    }


 	void onSetPath()
	{
		// pinPath changed
	}

 	void onSetChoice()
	{
		// pinChoice changed
	}

	BoolGuiPin pinTrigger;
	StringGuiPin pinAllowedExtensions;
	BoolGuiPin pinHideExtensions;
	IntGuiPin pinChoice;//index
	StringGuiPin pinItemList;
	StringGuiPin pinFileNameOut;

public:
	FileBrowserXGui()
	{
		initializePin(pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&FileBrowserXGui::onSetTrigger));
		initializePin(pinAllowedExtensions);
		initializePin(pinHideExtensions);
		initializePin(pinChoice, static_cast<MpGuiBaseMemberPtr2>(&FileBrowserXGui::onSetChoice));
		initializePin(pinItemList);
		initializePin(pinFileNameOut);
	}

private:
    // Helper: List files in directory filtered by extensions

    std::vector<std::string> listFilesInDirectory()
    {
        std::vector<std::string> files;

#if defined(_WIN32) || defined(_WIN64)

        fs::path filePath(fileNameString);
        fs::path dirPath = filePath.parent_path();
        // Get extension of the selected file (lowercase)
        std::string targetExt = filePath.extension().string();

        // Convert targetExt to lowercase
        std::transform(targetExt.begin(), targetExt.end(), targetExt.begin(),
            [](unsigned char c) { return std::tolower(c); });

        for (const auto& entry : fs::directory_iterator(dirPath))
        {
            if (entry.is_regular_file())
            {
                std::string fname = entry.path().filename().string();

                // Filter by extension (case-insensitive)
                std::string ext = entry.path().extension().string();

                // Convert extensions to lowercase for comparison
                std::transform(ext.begin(), ext.end(), ext.begin(),
                    [](unsigned char c) { return std::tolower(c); });

                // Also convert targetExt to lowercase (already done above)

                if (ext == targetExt)
                {
                    // Exclude hidden files (optional)
                    if (!fname.empty() && fname.front() != '.')
                    {
                        std::string filenameWithoutExt = fname.substr(0, fname.size() - ext.size());
                        files.push_back(filenameWithoutExt);
                    }
                }
            }
        }    
#else
        DIR* dirp = directoryPath;
        if (!dirp)
            return files;

        struct dirent* dp;
        while ((dp = readdir(dirp)) != nullptr)
        {
            // Skip "." and ".."
            if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
                continue;

            std::string filenameStr(dp->d_name);
            files.push_back(filenameStr);
        }
        closedir(dirp);

        // Sort alphabetically, case-insensitive
        std::sort(files.begin(), files.end(),
            [](const std::string& a, const std::string& b)
            {
                return std::lexicographical_compare(
                    a.begin(), a.end(),
                    b.begin(), b.end(),
                    [](unsigned char ac, unsigned char bc)
                    {
                        return std::tolower(ac) < std::tolower(bc);
                    });
            });
#endif
        // Join into comma-separated string
        std::stringstream ss;
        for (size_t i = 0; i < files.size(); ++i)
        {
            ss << files[i];
            if (i != files.size() - 1)
                ss << ", ";
        }
        pinItemList = ss.str();

        return files;
    }
};

namespace
{
	auto r = Register<FileBrowserXGui>::withId(L"My FileBrowserX");
}

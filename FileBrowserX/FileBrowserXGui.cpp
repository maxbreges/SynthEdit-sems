#include "mp_gui.h"
#include <string>
#include <vector>
#include <algorithm> // for transform
#include <sstream>
#include <locale>
#include <cwctype>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <filesystem>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

using namespace gmpi;
using namespace gmpi_gui;

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
            directoryPath = getDirectoryFromPath(fileNameString);

            pinFileNameOut = fileNameString;
        }

        nativeFileDialog.setNull(); // Release it.
        listFilesInDirectory(utf8_to_wstring(directoryPath));

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

    // Determine platform-specific path separator
#ifdef _WIN32
    static constexpr wchar_t PathSeparator = L'\\';
#else
    static constexpr wchar_t PathSeparator = L'/';
#endif
    // Conversion functions for UTF-8 and wstring
    std::string wstring_to_utf8(const std::wstring& wstr)
    {
        static std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        return conv.to_bytes(wstr);
    }

    std::wstring utf8_to_wstring(const std::string& str)
    {
        static std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        return conv.from_bytes(str);
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
        std::vector<std::wstring> files;
        // Helper: List files in directory filtered by extensions
        std::vector<std::wstring> listFilesInDirectory(const std::wstring& directory)
        {    

#if defined(_WIN32) || defined(_WIN64)
            std::filesystem::path filePath(fileNameString);
            std::filesystem::path dirPath = filePath.parent_path();
            // Get extension of the selected file (lowercase)
            std::string targetExt = filePath.extension().string();

            // Convert targetExt to lowercase
            std::transform(targetExt.begin(), targetExt.end(), targetExt.begin(),
                [](unsigned char c) { return std::tolower(c); });

            for (const auto& entry : std::filesystem::directory_iterator(dirPath))
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
                            std::wstring filenameWithoutExt = utf8_to_wstring(fname.substr(0, fname.size() - ext.size()));
                            files.push_back(filenameWithoutExt);
                        }
                    }
                }
            }
#else
            // Convert directory to UTF-8 string
            std::string dirUtf8 = wstring_to_utf8(directory);

            DIR* dirp = opendir(dirUtf8.c_str());
            if (!dirp)
                return files;

            struct dirent* dp;
            while ((dp = readdir(dirp)) != nullptr)
            {
                // Skip "." and ".."
                if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
                    continue;

                // Convert filename to wstring
                std::string filenameStr(dp->d_name);
                std::wstring filenameW = utf8_to_wstring(filenameStr);
                files.push_back(filenameW);
        }
            closedir(dirp);
           // Sort alphabetically, case-insensitive
std::sort(files.begin(), files.end(),
    [](const std::wstring& a, const std::wstring& b)
    {
        return std::lexicographical_compare(
            a.begin(), a.end(),
            b.begin(), b.end(),
            [](wchar_t ac, wchar_t bc)
            {
                return std::towlower(ac) < std::towlower(bc);
            });
    });
#endif
            // Join into comma-separated string
            std::wstringstream ss;
            for (size_t i = 0; i < files.size(); ++i)
            {
                ss << files[i];
                if (i != files.size() - 1)
                    ss << ", ";
            }
            pinItemList = ss.str();
            setPinChoiceFromPath();
            return files;
        }

        // Helper: Get filename without extension from full path
        std::wstring getFileNameWithoutExtension(const std::wstring& path)
        {
            // Find the position of the last directory separator
            size_t sepPos = path.find_last_of(L"/\\");
            size_t startPos = (sepPos != std::wstring::npos) ? sepPos + 1 : 0;

            // Extract the filename with extension
            std::wstring filenameWithExt = path.substr(startPos);

            // Find the last dot in the filename to remove extension
            size_t dotPos = filenameWithExt.find_last_of(L'.');

            // If there's a dot, remove the extension
            if (dotPos != std::wstring::npos)
            {
                return filenameWithExt.substr(0, dotPos);
            }
            else
            {
                // No extension found, return the filename as is
                return filenameWithExt;
            }
        }

        // Helper: Set pinChoice based on filename
        void setPinChoiceFromPath()
        {
            // Find filename in currentFileList
            for (size_t i = 0; i < files.size(); ++i)
            {
                if (files[i] == getFileNameWithoutExtension(utf8_to_wstring(fileNameString)))
                {
                    pinChoice = static_cast<int32_t>(i);
                    break;
                }
            }
        }

};

namespace
{
	auto r = Register<FileBrowserXGui>::withId(L"My FileBrowserX");
}

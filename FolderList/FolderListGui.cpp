#include "mp_sdk_gui2.h"
#include <string>
#include <sstream>
#include <vector>
#include <algorithm> // for transform
#include <codecvt>
#include <locale>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <filesystem>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

using namespace gmpi;
using namespace gmpi_gui;

class FolderListGui final : public SeGuiInvisibleBase
{
    std::string fileNameString;
    std::string directory;
    std::string targetExt;

    // Helper function to extract directory path from a full file path
    std::string getDirectoryFromPath(const std::string& filepath)
    {
        size_t p = filepath.find_last_of("/\\");
        if (p != std::string::npos)
            return filepath.substr(0, p);
        else
            return ""; // No directory part found
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

    StringGuiPin pinFilePath;
    IntGuiPin pinChoice;//index
    StringGuiPin pinItemList;  
    StringGuiPin pinExtension;

public:
	FolderListGui()
	{
        initializePin(pinFilePath, static_cast<MpGuiBaseMemberPtr2>(&FolderListGui::onSetPath));
        initializePin(pinChoice, static_cast<MpGuiBaseMemberPtr2>(&FolderListGui::onSetChoice));
        initializePin(pinItemList);
        initializePin(pinExtension);        
    }

    void onSetPath()
    {
        std::wstring fullPathString = pinFilePath.getValue();
        fileNameString = wstring_to_utf8(fullPathString);
        directory = getDirectoryFromPath(fileNameString);         
        Rescan();
    }

    void onSetChoice()
    {
        int choiceIndex = pinChoice;
        std::string separator = wstring_to_utf8(std::wstring(1, PathSeparator));

        if (choiceIndex >= 0 && choiceIndex < static_cast<int>(files.size()))
        {
            std::wstring filename = files[choiceIndex];
            std::string fullPath = directory + separator + wstring_to_utf8(filename) + targetExt;
            pinFilePath = fullPath;            
        }
    }

    void Rescan()
    {
        if (!files.empty())
        {
            files.clear();
        }
        if (directory.empty())
            return; // avoid invalid directory access
        listFilesInDirectory(utf8_to_wstring(directory));
        pinExtension = targetExt;
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
        targetExt = filePath.extension().string();

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
public:
    // Helper: Set pinChoice based on filename
    void setPinChoiceFromPath()
    {
        // Find filename in currentFileList
        for (size_t i = 0; i < files.size(); ++i)
        {
            if (files[i] == getFileNameWithoutExtension(utf8_to_wstring(pinFilePath)))
            {
                pinChoice = static_cast<int32_t>(i);
                
                break;
            }
        }
    }
};

namespace
{
	auto r = Register<FolderListGui>::withId(L"FolderList");
}

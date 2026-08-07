#include "mp_sdk_gui2.h"
#include <string>
#include <vector>
#include <algorithm>
#include <codecvt>
#include <locale>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <dirent.h>
#include <sys/stat.h>

#endif

using namespace gmpi;

class FileBrowserGui final : public SeGuiInvisibleBase
{
    void onSetAllowedExtensions()
    {
        // pinAllowedExtensions changed
    }

    void onSetHideExtensions()
    {
        // pinHideExtensions changed
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

    // Pins
    StringGuiPin pinPath;
    StringGuiPin pinAllowedExtensions;
    BoolGuiPin pinHideExtensions;
    IntGuiPin pinChoice;
    StringGuiPin pinItemList;


private:
    // Internal file list
    std::vector<std::wstring> currentFileList;
    std::wstring currentDirectory;

public:
    FileBrowserGui()
    {
        initializePin(pinPath, static_cast<MpGuiBaseMemberPtr2>(&FileBrowserGui::onSetPath));
        initializePin(pinAllowedExtensions, static_cast<MpGuiBaseMemberPtr2>(&FileBrowserGui::onSetAllowedExtensions));
        initializePin(pinHideExtensions, static_cast<MpGuiBaseMemberPtr2>(&FileBrowserGui::onSetHideExtensions));
        initializePin(pinChoice, static_cast<MpGuiBaseMemberPtr2>(&FileBrowserGui::onSetChoice));
        initializePin(pinItemList);
    }

private:
    // Helper: List files in directory filtered by extensions
    std::vector<std::wstring> listFilesInDirectory(const std::wstring& directory)
    {
        std::vector<std::wstring> files;

#if defined(_WIN32) || defined(_WIN64)
        WIN32_FIND_DATAW findFileData;
        HANDLE hFind;
        std::wstring searchPath = std::wstring(directory.begin(), directory.end()) + L"\\*";

        hFind = FindFirstFileW(searchPath.c_str(), &findFileData);
        if (hFind == INVALID_HANDLE_VALUE)
            return files;

        do
        {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                files.push_back(std::wstring(findFileData.cFileName));
            }
        } while (FindNextFileW(hFind, &findFileData) != 0);
        FindClose(hFind);
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
#endif
        return files;
    }

    // Helper: Filter files by extension(s)
    std::vector<std::wstring> filterFiles(const std::vector<std::wstring>& files)
    {
        std::vector<std::wstring> filtered;

        // Get allowed extensions
        std::wstring extStr = pinAllowedExtensions; // e.g., "wav,mp3"
        std::vector<std::wstring> allowedExts;
        size_t pos = 0;
        while ((pos = extStr.find(L',')) != std::wstring::npos)
        {
            allowedExts.push_back(extStr.substr(0, pos));
            extStr.erase(0, pos + 1);
        }
        if (!extStr.empty())
            allowedExts.push_back(extStr);

        // Convert allowed extensions to lowercase for comparison
        for (auto& e : allowedExts)
            std::transform(e.begin(), e.end(), e.begin(), ::tolower);

        for (const auto& filename : files)
        {
            if (allowedExts.empty() || pinAllowedExtensions.getValue().empty())
            {
                // No extension filter
                filtered.push_back(filename);
            }
            else
            {
                // Check extension
                size_t dotPos = filename.find_last_of('.');
                if (dotPos != std::wstring::npos)
                {
                    std::wstring fileExt = filename.substr(dotPos + 1);
                    std::transform(fileExt.begin(), fileExt.end(), fileExt.begin(), ::tolower);
                    if (std::find(allowedExts.begin(), allowedExts.end(), fileExt) != allowedExts.end())
                    {
                        filtered.push_back(filename);
                    }
                }
            }
        }
        return filtered;
    }

    // Helper: Remove extension if hideExtensions is true
    std::wstring stripExtension(const std::wstring& filename)
    {
        if (!pinHideExtensions.getValue())
            return filename;
        size_t dotPos = filename.find_last_of('.');
        if (dotPos != std::wstring::npos)
            return filename.substr(0, dotPos);
        return filename;
    }

    // Helper: Update pinItemList based on currentFileList
    void updatePinItemList()
    {
        std::wstring itemListStr;
        bool hideExt = pinHideExtensions.getValue();

        for (size_t i = 0; i < currentFileList.size(); ++i)
        {
            std::wstring name = stripExtension(currentFileList[i]);
            itemListStr += name;
            if (i != currentFileList.size() - 1)
                itemListStr += L",";
        }
        pinItemList = itemListStr;
    }

    // Helper: Set pinChoice based on filename
    void setPinChoiceFromPath()
    {
        std::wstring path = pinPath;
        // Find filename in currentFileList
        for (size_t i = 0; i < currentFileList.size(); ++i)
        {
            if (currentFileList[i] == getFileNameFromPath(path))
            {
                pinChoice = static_cast<int32_t>(i);
                break;
            }
        }
    }

    // Helper: Get filename from full path
    std::wstring getFileNameFromPath(const std::wstring& path)
    {
        size_t sepPos = path.find_last_of(L"/\\");
        if (sepPos != std::wstring::npos)
            return path.substr(sepPos + 1);
        return path;
    }

public:


    // Called when path pin changes
    void onSetPath()
    {
        onExternalPathChange();

        std::wstring newPath = pinPath;
        std::wstring dirPath = getDirectoryFromPath(newPath);
        currentDirectory = dirPath;

        refreshFileList();

        // Set pinChoice based on filename
        setPinChoiceFromPath();

    }

    // Called when choice pin changes
    void onSetChoice()
    {
        int choiceIndex = pinChoice.getValue();

        if (choiceIndex >= 0 && choiceIndex < static_cast<int>(currentFileList.size()))
        {
            std::wstring filename = currentFileList[choiceIndex];
            std::wstring fullPath = currentDirectory + L"/" + filename;
            pinPath = fullPath;
        }

    }

    // Additional: When pinPath is set externally, update selection
    void onExternalPathChange()
    {
        // Extract directory from the new pinPath
        std::wstring newPath = pinPath;
        std::wstring newDir = getDirectoryFromPath(newPath);
        currentDirectory = newDir;

        // Save filename from the full path
        std::wstring filenameToFind = getFileNameFromPath(newPath);

        // Refresh file list in the new directory
        refreshFileList();

        // Try to find the filename in the new file list
        int index = -1;
        for (size_t i = 0; i < currentFileList.size(); ++i)
        {
            if (currentFileList[i] == filenameToFind)
            {
                index = static_cast<int>(i);
                break;
            }
        }

        // Set pinChoice accordingly
        if (index >= 0)
        {
            pinChoice = index;
        }
        else
        {
            // If not found, default to 0 or keep current pinChoice
            if (!currentFileList.empty())
            {
                //pinChoice = 0;
                // Optionally, update pinPath to the first file
                std::wstring fullPath = currentDirectory + L"/" + currentFileList[0];
                pinPath = fullPath;
            }
        }

    }

private:
    // Refresh file list based on current directory and filters
    void refreshFileList()
    {
        if (currentDirectory.empty())
            return;

        currentFileList = listFilesInDirectory(currentDirectory);
        currentFileList = filterFiles(currentFileList);

        std::locale loc; // default locale
        // ...
        std::sort(currentFileList.begin(), currentFileList.end(), [&](const std::wstring& a, const std::wstring& b) {
            return std::use_facet<std::collate<wchar_t>>(loc).compare(a.data(), a.data() + a.size(), b.data(), b.data() + b.size()) < 0;
            });

        updatePinItemList();

        // Reset pinChoice if out of range
        int currentChoice = pinChoice.getValue();
        if (currentChoice >= (int)currentFileList.size())
        {
            pinChoice=0;
        }
        else
        {
            // Update pinPath to selected filename
            if (!currentFileList.empty())
            {
                std::wstring filename = currentFileList[pinChoice.getValue()];
                pinPath=currentDirectory + L"/" + filename;
            }
        }
    }

    // Helper: Extract directory from path
    std::wstring getDirectoryFromPath(const std::wstring& path)
    {
        size_t sepPos = path.find_last_of(L"/\\");
        if (sepPos != std::wstring::npos)
            return path.substr(0, sepPos);
        return L"";
    }

};

namespace
{
	auto r = Register<FileBrowserGui>::withId(L"My FileBrowser");
}

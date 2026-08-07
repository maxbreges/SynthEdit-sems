#include "mp_sdk_gui2.h"
#include <string>
#include <vector>
#include <algorithm>

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

    void onSetList()
    {
        // pinList changed
    }

    void onSetParent()
    {
        // pinParent changed
    }

    void onSetItemList()
    {
        // pinItemList changed
    }

    // Pins
    StringGuiPin pinPath;
    StringGuiPin pinAllowedExtensions;
    BoolGuiPin pinHideExtensions;
    IntGuiPin pinList;
    BoolGuiPin pinRescan;
    BoolGuiPin pinParent;
    IntGuiPin pinChoice;
    StringGuiPin pinItemList;

    // Internal file list
    std::vector<std::string> currentFileList;
    std::string currentDirectory;

public:
    FileBrowserGui()
    {
        initializePin(pinPath, static_cast<MpGuiBaseMemberPtr2>(&FileBrowserGui::onSetPath));
        initializePin(pinAllowedExtensions, static_cast<MpGuiBaseMemberPtr2>(&FileBrowserGui::onSetAllowedExtensions));
        initializePin(pinHideExtensions, static_cast<MpGuiBaseMemberPtr2>(&FileBrowserGui::onSetHideExtensions));
        initializePin(pinList, static_cast<MpGuiBaseMemberPtr2>(&FileBrowserGui::onSetList));
        initializePin(pinRescan, static_cast<MpGuiBaseMemberPtr2>(&FileBrowserGui::onSetRescan));
        initializePin(pinParent, static_cast<MpGuiBaseMemberPtr2>(&FileBrowserGui::onSetParent));
        initializePin(pinChoice, static_cast<MpGuiBaseMemberPtr2>(&FileBrowserGui::onSetChoice));
        initializePin(pinItemList, static_cast<MpGuiBaseMemberPtr2>(&FileBrowserGui::onSetItemList));
    }

private:
    // Helper: List files in directory filtered by extensions
    std::vector<std::string> listFilesInDirectory(const std::string& directory)
    {
        std::vector<std::string> files;

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
                // Convert wchar_t* to std::string
                std::wstring filenameW(findFileData.cFileName);
                std::string filename(filenameW.begin(), filenameW.end());
                files.push_back(filename);
            }
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
#else
        DIR* dirp = opendir(directory.c_str());
        if (!dirp)
            return files;
        struct dirent* dp;
        while ((dp = readdir(dirp)) != nullptr)
        {
            if (dp->d_type != DT_DIR)
            {
                files.push_back(dp->d_name);
            }
        }
        closedir(dirp);
#endif
        return files;
    }

    // Helper: Filter files by extension(s)
    std::vector<std::string> filterFiles(const std::vector<std::string>& files)
    {
        std::vector<std::string> filtered;

        // Get allowed extensions
        std::string extStr = pinAllowedExtensions; // e.g., "wav,mp3"
        std::vector<std::string> allowedExts;
        size_t pos = 0;
        while ((pos = extStr.find(',')) != std::string::npos)
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
                if (dotPos != std::string::npos)
                {
                    std::string fileExt = filename.substr(dotPos + 1);
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
    std::string stripExtension(const std::string& filename)
    {
        if (!pinHideExtensions.getValue())
            return filename;
        size_t dotPos = filename.find_last_of('.');
        if (dotPos != std::string::npos)
            return filename.substr(0, dotPos);
        return filename;
    }

    // Helper: Update pinItemList based on currentFileList
    void updatePinItemList()
    {
        std::string itemListStr;
        bool hideExt = pinHideExtensions.getValue();

        for (size_t i = 0; i < currentFileList.size(); ++i)
        {
            std::string name = stripExtension(currentFileList[i]);
            itemListStr += name;
            if (i != currentFileList.size() - 1)
                itemListStr += ",";
        }
        pinItemList=itemListStr;
    }

    // Helper: Set pinChoice based on filename
    void setPinChoiceFromPath()
    {
        std::string path = pinPath;
        // Find filename in currentFileList
        for (size_t i = 0; i < currentFileList.size(); ++i)
        {
            if (currentFileList[i] == getFileNameFromPath(path))
            {
                pinChoice=i;
                break;
            }
        }
    }

    // Helper: Get filename from full path
    std::string getFileNameFromPath(const std::string& path)
    {
        size_t sepPos = path.find_last_of("/\\");
        if (sepPos != std::string::npos)
            return path.substr(sepPos + 1);
        return path;
    }

public:
    // Called when rescan pin changes
    void onSetRescan()
    {
        if (pinRescan.getValue())
        {
            refreshFileList();
        }
    }

    // Called when path pin changes
    void onSetPath()
    {
        std::string newPath = pinPath;
        std::string dirPath = getDirectoryFromPath(newPath);
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
            std::string filename = currentFileList[choiceIndex];
            std::string fullPath = currentDirectory + "/" + filename;
            pinPath=fullPath;
        }
    }

    // Additional: When pinPath is set externally, update selection
    void onExternalPathChange()
    {
        // Similar to onSetPath
        onSetPath();
    }

private:
    // Refresh file list based on current directory and filters
    void refreshFileList()
    {
        if (currentDirectory.empty())
            return;

        currentFileList = listFilesInDirectory(currentDirectory);
        currentFileList = filterFiles(currentFileList);
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
                std::string filename = currentFileList[pinChoice.getValue()];
                pinPath=currentDirectory + "/" + filename;
            }
        }
    }

    // Helper: Extract directory from path
    std::string getDirectoryFromPath(const std::string& path)
    {
        size_t sepPos = path.find_last_of("/\\");
        if (sepPos != std::string::npos)
            return path.substr(0, sepPos);
        return "";
    }
};

namespace
{
	auto r = Register<FileBrowserGui>::withId(L"My FileBrowser");
}

#include "mp_sdk_gui2.h"
#include <string>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <dirent.h>
#include <algorithm> // for transform
#endif

using namespace gmpi;
std::vector<std::wstring> filenames;
class ListFilesGui final : public SeGuiInvisibleBase
{
    void onSetDirectory()
    {
        filenames.clear();
        // Get the directory path from the pin
        std::wstring directory = pinFilePath.getValue();

        // Remove trailing slash/backslash if present
        if (!directory.empty() && (directory.back() == L'\\' || directory.back() == L'/'))
            directory.pop_back();

        // Get extension filter
        std::string extensionFilter = pinExtension; // assuming it's a string
        // Convert extension to lowercase for comparison
        std::transform(extensionFilter.begin(), extensionFilter.end(), extensionFilter.begin(), ::tolower);



        // Platform-specific directory listing
#if defined(_WIN32) || defined(_WIN64)
        std::wstring searchPath = directory + L"\\*";
        WIN32_FIND_DATA findFileData;
        HANDLE hFind = FindFirstFile(searchPath.c_str(), &findFileData);
        if (hFind != INVALID_HANDLE_VALUE)
        {
            do
            {
                std::wstring filename = findFileData.cFileName;
                if (filename != L"." && filename != L"..")
                {
                    // Check extension if filter is set
                    if (extensionFilter.empty() || hasExtension(filename, extensionFilter))
                    {
                        filenames.push_back(filename);
                    }
                }
            } while (FindNextFile(hFind, &findFileData) != 0);
            FindClose(hFind);
        }
#else
        DIR* dir = opendir(std::string(directory.begin(), directory.end()).c_str());
        if (dir)
        {
            struct dirent* entry;
            while ((entry = readdir(dir)) != nullptr)
            {
                std::string name = entry->d_name;
                if (name != "." && name != "..")
                {
                    std::wstring wname(name.begin(), name.end());
                    if (extensionFilter.empty() || hasExtension(wname, extensionFilter))
                    {
                        filenames.push_back(wname);
                    }
                }
            }
            closedir(dir);
        }
#endif

        // Build comma-separated string
        std::wstring result;
        for (size_t i = 0; i < filenames.size(); ++i)
        {
            result += filenames[i];
            if (i != filenames.size() - 1)
                result += L", ";
        }

        // Set the result to the pin
        pinFilesList = result;
    }

    // Helper function to check extension
    bool hasExtension(const std::wstring& filename, const std::string& extensionFilter)
    {
        size_t dotPos = filename.find_last_of(L'.');
        if (dotPos == std::wstring::npos)
            return false; // no extension

        std::wstring ext = filename.substr(dotPos + 1);
        // Convert ext to lowercase
        std::string extStr(ext.begin(), ext.end());
        std::transform(extStr.begin(), extStr.end(), extStr.begin(), ::tolower);

        return extStr == extensionFilter;
    }

    void onSetFilesList() {}
    void onSetExtension() {}

    StringGuiPin pinFilePath;
    StringGuiPin pinExtension;
    StringGuiPin pinFilesList;  
    IntGuiPin pinIndex;

public:
    ListFilesGui()
    {
        initializePin(pinFilePath, static_cast<MpGuiBaseMemberPtr2>(&ListFilesGui::onSetDirectory));
        initializePin(pinExtension, static_cast<MpGuiBaseMemberPtr2>(&ListFilesGui::onSetExtension));
        initializePin(pinFilesList, static_cast<MpGuiBaseMemberPtr2>(&ListFilesGui::onSetFilesList));
        initializePin(pinIndex);
    }

    // Helper: Set pinChoice based on filename
    void setPinChoiceFromPath()
    {
        std::wstring path = pinFilePath;
        // Find filename in currentFileList
        for (size_t i = 0; i < filenames.size(); ++i)
        {
            if (filenames[i] == getFileNameFromPath(path))
            {
                pinIndex = i;
                break;
            }
        }
    }

    // Helper: Get filename from full path
    std::wstring getFileNameFromPath(const std::wstring& path)
    {
        size_t sepPos = path.find_last_of(L"/\\");
        if (sepPos != std::string::npos)
            return path.substr(sepPos + 1);
        return path;
    }
};

namespace
{
    auto r = Register<ListFilesGui>::withId(L"My ListFiles");
}
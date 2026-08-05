#include "mp_sdk_gui2.h"
#include <string>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <dirent.h>
#endif

using namespace gmpi;

class ListFilesGui final : public SeGuiInvisibleBase
{
    void onSetDirectory()
    {
        // Get the directory path from the pin
        std::wstring directory = pinDirectory.getValue();

        // Remove trailing slash/backslash if present
        if (!directory.empty() && (directory.back() == L'\\' || directory.back() == L'/'))
            directory.pop_back();

        std::vector<std::wstring> filenames;

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
                    filenames.push_back(filename);
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
                    // Convert to wstring
                    std::wstring wname(name.begin(), name.end());
                    filenames.push_back(wname);
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

    void onSetFilesList()
    {
        // Could be used if needed when files list pin changes
    }

    StringGuiPin pinDirectory;
    StringGuiPin pinFilesList;

public:
    ListFilesGui()
    {
        initializePin(pinDirectory, static_cast<MpGuiBaseMemberPtr2>(&ListFilesGui::onSetDirectory));
        initializePin(pinFilesList, static_cast<MpGuiBaseMemberPtr2>(&ListFilesGui::onSetFilesList));
    }
};

namespace
{
    auto r = Register<ListFilesGui>::withId(L"My ListFiles");
}
#include "mp_sdk_gui2.h"
#include <sstream>
#include <vector>
#include <string>

using namespace gmpi;

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <locale>
#include <codecvt>
#include <wchar.h>
#include <strings.h> // For wcscasecmp
#endif

// Helper to list files with extension in a directory
std::vector<std::wstring> listFiles(const std::wstring& directory, const std::wstring& extension)
{
    std::vector<std::wstring> files;

#ifdef _WIN32
    WIN32_FIND_DATA findFileData;
    std::wstring searchPath = directory + L"\\*";

    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findFileData);
    if (hFind == INVALID_HANDLE_VALUE)
        return files;

    do
    {
        const std::wstring filename = findFileData.cFileName;
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            if (extension.empty() ||
                (filename.length() >= extension.length() &&
                    _wcsicmp(filename.substr(filename.length() - extension.length()).c_str(), extension.c_str()) == 0))
            {
                files.push_back(filename);
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);
    FindClose(hFind);
#else
    // POSIX implementation
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string dir_utf8 = converter.to_bytes(directory);

    DIR* dir = opendir(dir_utf8.c_str());
    if (!dir)
        return files;

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        std::string filename_utf8(entry->d_name);
        std::wstring filename = converter.from_bytes(filename_utf8);

        std::wstring full_path = directory + L"/" + filename;
        struct stat path_stat;
        if (stat(converter.to_bytes(full_path).c_str(), &path_stat) == 0)
        {
            if (S_ISREG(path_stat.st_mode))
            {
                if (extension.empty() ||
                    (filename.length() >= extension.length() &&
                        wcscasecmp(filename.c_str() + filename.length() - extension.length(), extension.c_str()) == 0))
                {
                    files.push_back(filename);
                }
            }
        }
    }
    closedir(dir);
#endif

    return files;
}

class ItemListGui final : public SeGuiInvisibleBase
{
    void onSetFileName()
    {
        auto fullFilePath = pinFileName.getValue(); // e.g., "C:\\Folder\\file.txt"
        auto extension = pinFileExtension.getValue(); // e.g., L".txt"

        // Extract directory part from fullFilePath
        size_t lastSlashPos = fullFilePath.find_last_of(L"\\/"); // handles both '/' and '\\'
        std::wstring directory;
        if (lastSlashPos != std::wstring::npos)
        {
            directory = fullFilePath.substr(0, lastSlashPos);
        }
        else
        {
            // If no slash found, assume current directory or handle as needed
            directory = L".";
        }

        auto files = listFiles(directory, extension);

        // Prepare list of filenames (excluding path)
        std::wstringstream ss;
        for (size_t i = 0; i < files.size(); ++i)
        {
            ss << files[i];
            if (i < files.size() - 1)
                ss << L",";
        }
        pinItemList = ss.str();
    }

    void onSetFileExtension()
    {
        // Optional: trigger listing again if extension changes
        onSetFileName();
    }

    StringGuiPin pinFileName;       // Full path filename pin
    StringGuiPin pinFileExtension;  // Extension filter
    StringGuiPin pinItemList;

public:
    ItemListGui()
    {
        initializePin(pinFileName, static_cast<MpGuiBaseMemberPtr2>(&ItemListGui::onSetFileName));
        initializePin(pinFileExtension, static_cast<MpGuiBaseMemberPtr2>(&ItemListGui::onSetFileExtension));
        initializePin(pinItemList);
    }
};

namespace
{
    auto r = Register<ItemListGui>::withId(L"ItemList");
}
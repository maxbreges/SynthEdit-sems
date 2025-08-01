#include "mp_sdk_gui2.h"
#include <sstream>
#include <vector>
#include <string>

using namespace gmpi;

#ifdef _WIN32
#include <windows.h>
#else
// Platform-specific includes if needed
#endif

// Helper to list files with extension
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
    // Implement for other platforms if needed
#endif

    return files;
}

class ItemListGui final : public SeGuiInvisibleBase
{
    void onSetDirectory()
    {
        auto dir = pinDirectory.getValue(); // std::wstring
        auto extension = pinFileExtension.getValue(); // e.g., L".txt"

        auto files = listFiles(dir, extension);

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
        onSetDirectory();
    }

    StringGuiPin pinDirectory;
    StringGuiPin pinFileExtension;
    StringGuiPin pinItemList;

public:
    ItemListGui()
    {
        initializePin(pinDirectory, static_cast<MpGuiBaseMemberPtr2>(&ItemListGui::onSetDirectory));
        initializePin(pinFileExtension, static_cast<MpGuiBaseMemberPtr2>(&ItemListGui::onSetFileExtension));
        initializePin(pinItemList);
    }
};

namespace
{
    auto r = Register<ItemListGui>::withId(L"ItemList");
}
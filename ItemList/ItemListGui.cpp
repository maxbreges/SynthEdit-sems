#include "mp_sdk_gui2.h"
#include <string>
#include <vector>
#include <sstream>

using namespace gmpi;

class ItemListGui final : public SeGuiInvisibleBase
{
    void onSetFileName()
    {
        // When pinItemList changes, update pinFileName based on selected filename(s)

        std::wstring currentFullPath = pinFileName.getValue();

        // Find the last occurrence of '/' or '\\' to identify the directory part
        size_t lastSlashPos = currentFullPath.find_last_of(L"/\\");
        std::wstring directory;
        std::wstring filenamePart;

        if (lastSlashPos != std::wstring::npos)
        {
            directory = currentFullPath.substr(0, lastSlashPos);
            filenamePart = currentFullPath.substr(lastSlashPos + 1);
        }
        else
        {
            // No directory info in current path, default to current working directory or empty
            directory = L""; // Or set to your default directory
            filenamePart = currentFullPath;
        }

        // Parse the comma-separated list
        std::wstringstream ss(pinItemList.getValue());
        std::wstring item;
        std::vector<std::wstring> filenames;
        while (std::getline(ss, item, L','))
        {
            // Remove leading/trailing spaces
            size_t start = item.find_first_not_of(L" \t");
            size_t end = item.find_last_not_of(L" \t");
            if (start != std::wstring::npos && end != std::wstring::npos)
            {
                item = item.substr(start, end - start + 1);
            }
            filenames.push_back(item);
        }

        // For demo, pick the first filename or handle selection differently
        if (!filenames.empty())
        {
            std::wstring selectedFile = filenames[0]; // Or select based on some criteria

            // Construct full path
            // Use the extracted directory if available, fallback to current directory if not
            std::wstring dirToUse = directory.empty() ? L"" : directory;
            std::wstring pathSeparator = L"/";

            std::wstring fullPath = dirToUse + pathSeparator + selectedFile;
            pinFileName = fullPath;
        }
        else
        {
            // No items, clear filename
            pinFileName = L"";
        }
    }

    StringGuiPin pinFileName;       // Full path filename pin
    StringGuiPin pinItemList;       // Comma-separated list of filenames

public:
    ItemListGui()
    {
        initializePin(pinFileName, static_cast<MpGuiBaseMemberPtr2>(&ItemListGui::onSetFileName));
        initializePin(pinItemList);
    }
};

namespace
{
    auto r = Register<ItemListGui>::withId(L"ItemList");
}
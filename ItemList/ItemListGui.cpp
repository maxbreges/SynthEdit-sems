#include "mp_sdk_gui2.h"
#include <string>
#include <vector>
#include <sstream>

using namespace gmpi;

class ItemListGui final : public SeGuiInvisibleBase
{
    // Callbacks for pin changes
    void onSetFileName()
    {
        // Your existing code to build the filename from list
        updateFileNameFromList();
    }

    void onSetItemList()
    {
        // When the list changes, update the filename accordingly
        updateFileNameFromList();
    }

    void updateFileNameFromList()
    {
        std::wstring currentFullPath = pinFileName.getValue();

        // Extract directory from currentFullPath
        size_t lastSlashPos = currentFullPath.find_last_of(L"/\\");
        std::wstring directory;
        if (lastSlashPos != std::wstring::npos)
        {
            directory = currentFullPath.substr(0, lastSlashPos);
        }
        else
        {
            directory = L""; // Default or leave empty
        }

        // Parse the list of filenames
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

        // For demonstration, pick the first filename
        if (!filenames.empty())
        {
            std::wstring selectedFile = filenames[0];

            // Construct full path
            std::wstring fullPath = directory + L"/" + selectedFile;
            pinFileName = fullPath;
        }
        else
        {
            pinFileName = L"";
        }
    }

public:
    StringGuiPin pinFileName;  // Full path filename pin
    StringGuiPin pinItemList;  // Comma-separated list of filenames

    ItemListGui()
    {
        // Initialize with callbacks
        initializePin(pinFileName, static_cast<MpGuiBaseMemberPtr2>(&ItemListGui::onSetFileName));
        initializePin(pinItemList, static_cast<MpGuiBaseMemberPtr2>(&ItemListGui::onSetItemList));
    }
};

namespace
{
    auto r = Register<ItemListGui>::withId(L"ItemList");
}
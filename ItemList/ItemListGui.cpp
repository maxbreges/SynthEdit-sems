#include "mp_sdk_gui2.h"
#include <filesystem>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <cctype>

using namespace gmpi;
namespace fs = std::filesystem;

class ItemListGui final : public SeGuiInvisibleBase
{  
    void onSetFileName()
    {
        pinDebug = "onSetFileName()";

        auto filename = pinFileName.getValue();        

        fs::path filePath(filename);
        fs::path dirPath = filePath.parent_path();

        if (fs::exists(dirPath) && fs::is_directory(dirPath))
        {
            std::vector<std::wstring> fileNames;

            // Get extension of the selected file (lowercase)
            std::wstring targetExt = filePath.extension().native();

            for (const auto& entry : fs::directory_iterator(dirPath))
            {
                pinDebug = "Iterator";

                if (entry.is_regular_file())
                {
                    std::wstring fname = entry.path().filename().native();

                    // Filter by extension (case-insensitive)
                    std::wstring ext = entry.path().extension().native();

                    // Convert extensions to lowercase for comparison
                    auto toLower = [](wchar_t c) { return std::tolower(c); };

                    std::wstring extLower(ext.size(), L'\0');
                    std::transform(ext.begin(), ext.end(), extLower.begin(), toLower);

                    std::wstring targetExtLower(targetExt.size(), L'\0');
                    std::transform(targetExt.begin(), targetExt.end(), targetExtLower.begin(), toLower);

                    if (extLower == targetExtLower)
                    {
                        // Exclude hidden files (optional)
                        if (fname.front() != L'.')
                        {
                            fileNames.push_back(fname);
                        }
                    }
                }
            }

            // Sort alphabetically, case-insensitive
            std::sort(fileNames.begin(), fileNames.end(),
                [](const std::wstring& a, const std::wstring& b)
                {
                    return std::lexicographical_compare(
                        a.begin(), a.end(),
                        b.begin(), b.end(),
                        [](wchar_t ac, wchar_t bc)
                        {
                            return std::tolower(ac) < std::tolower(bc);
                        });
                });

            // Join into comma-separated string
            std::wstringstream ss;
            for (size_t i = 0; i < fileNames.size(); ++i)
            {
                ss << fileNames[i];
                if (i != fileNames.size() - 1)
                    ss << L", ";
            }

            pinItemList = ss.str();
            pinDebug = "updated";
        }
        else
        {
            pinItemList = L"";
        }
    }

    StringGuiPin pinFileName;  // Full path filename pin
    StringGuiPin pinItemList;  // List of filenames
    StringGuiPin pinDebug;

public:
    ItemListGui()
    {
        initializePin(pinFileName, static_cast<MpGuiBaseMemberPtr2>(&ItemListGui::onSetFileName));
        initializePin(pinItemList);
        initializePin(pinDebug);
    }
};

namespace
{
    auto r = Register<ItemListGui>::withId(L"ItemList");
}
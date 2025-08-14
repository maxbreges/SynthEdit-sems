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

        std::string filename = pinFileName;

        fs::path filePath(filename);
        fs::path dirPath = filePath.parent_path();

        if (fs::exists(dirPath) && fs::is_directory(dirPath))
        {
            std::vector<std::string> fileNames;

            // Get extension of the selected file (lowercase)
            std::string targetExt = filePath.extension().string();

            // Convert targetExt to lowercase
            std::transform(targetExt.begin(), targetExt.end(), targetExt.begin(),
                [](unsigned char c) { return std::tolower(c); });

            for (const auto& entry : fs::directory_iterator(dirPath))
            {
                pinDebug = "Iterator";

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
                            std::string filenameWithoutExt = fname.substr(0, fname.size() - ext.size());
                            fileNames.push_back(filenameWithoutExt);
                        }
                    }
                }
            }

            // Sort alphabetically, case-insensitive
            std::sort(fileNames.begin(), fileNames.end(),
                [](const std::string& a, const std::string& b)
                {
                    return std::lexicographical_compare(
                        a.begin(), a.end(),
                        b.begin(), b.end(),
                        [](unsigned char ac, unsigned char bc)
                        {
                            return std::tolower(ac) < std::tolower(bc);
                        });
                });

            // Join into comma-separated string
            std::stringstream ss;
            for (size_t i = 0; i < fileNames.size(); ++i)
            {
                ss << fileNames[i];
                if (i != fileNames.size() - 1)
                    ss << ", ";
            }

            pinItemList = ss.str();
            pinDebug = "updated";
        }
        else
        {
            pinItemList = "";
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
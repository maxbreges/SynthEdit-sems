#include "mp_sdk_gui2.h"
#include <filesystem>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <cctype>

using namespace gmpi;
namespace fs = std::filesystem;
std::vector<std::string> fileNames;

class CheckingStringOnMacGui final : public SeGuiInvisibleBase
{
 	void onSetFileName()
	{
        fileNames.clear();

        std::string filename = pinFileName;

        fs::path filePath(filename);
        fs::path dirPath = filePath.parent_path();     
        pinDirectory = dirPath;

        if (fs::exists(dirPath) && fs::is_directory(dirPath))
        {
            // Get extension of the selected file (lowercase)
            std::string targetExt = filePath.extension().string();

            // Convert targetExt to lowercase
            std::transform(targetExt.begin(), targetExt.end(), targetExt.begin(),
                [](unsigned char c) { return std::tolower(c); });

            for (const auto& entry : fs::directory_iterator(dirPath))
            {
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
            pinFileExtension = targetExt;
        }
        else
        {
            pinItemList = "";
        }
        onSetSelectedFile();
    }

    void onSetSelectedFile()
    {
        if (fileNames.empty())
        {
            pinChoice = -1;
            return;
        }

        std::string selectedFileStem = fs::path(pinFileName).stem().string();

        auto it = std::find(fileNames.begin(), fileNames.end(), selectedFileStem);
        if (it != fileNames.end())
        {
            auto index = std::distance(fileNames.begin(), it);
            pinChoice = static_cast<int>(index);
        }
        else
        {
            pinChoice = -1;
        }
    }

 	void onSetItemList()
	{
		// pinItemList changed
	}

 	void onSetFileExtension()
	{
		// pinDebug changed
	}

    void onSetChoice()
    {
        if (pinChoice >= 0 && pinChoice < fileNames.size())
        {
            onSetFileNameOut();
        }
        
    }

    void onSetFileNameOut()
    {
        std::string dir = pinDirectory;
        std::string ext = pinFileExtension;

        std::string filenameOnly = dir + "/" + fileNames[pinChoice] + ext; // Append the extension

        pinFileNameOut = filenameOnly;
    }


 	StringGuiPin pinFileName;
 	StringGuiPin pinItemList;
 	StringGuiPin pinFileExtension;
    StringGuiPin pinDirectory;
    IntGuiPin pinChoice;
    StringGuiPin pinFileNameOut;

public:
	CheckingStringOnMacGui()
	{
		initializePin( pinFileName, static_cast<MpGuiBaseMemberPtr2>(&CheckingStringOnMacGui::onSetFileName) );
		initializePin( pinItemList, static_cast<MpGuiBaseMemberPtr2>(&CheckingStringOnMacGui::onSetItemList) );
		initializePin(pinFileExtension, static_cast<MpGuiBaseMemberPtr2>(&CheckingStringOnMacGui::onSetFileExtension) );
        initializePin(pinDirectory);
        initializePin(pinChoice, static_cast<MpGuiBaseMemberPtr2>(&CheckingStringOnMacGui::onSetChoice));
        initializePin(pinFileNameOut);

	}

};

namespace
{
	auto r = Register<CheckingStringOnMacGui>::withId(L"My CheckingStringOnMac");
}

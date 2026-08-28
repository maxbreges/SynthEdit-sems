#include "mp_sdk_gui2.h"
#include <string>
#include <sstream>
#include <vector>
#include <algorithm> // for transform

#ifdef __APPLE__
#include <dirent.h> // POSIX directory functions
#else
#include <filesystem>
#endif

using namespace gmpi;

// Determine platform-specific path separator
#ifdef __APPLE__
static const std::string PathSeparator = "/";
#else
static const std::string PathSeparator = "\\";
#endif

class FolderListDirGui final : public SeGuiInvisibleBase
{
    std::string directoryPath;
    std::string targetExt;
    std::string Name;

 	void onSetFolderPath()
	{
        directoryPath = pinFolderPath;
        if (!files.empty())
        {
            files.clear();
        }
        if (directoryPath.empty())
            return; // avoid invalid directory access
        pinChoice = -1;
        listFilesInDirectory();
	}

    void onSetChoice()
    {
        int choiceIndex = pinChoice;

        if (choiceIndex >= 0 && choiceIndex < static_cast<int>(files.size()))
        {
            pinName = files[choiceIndex];
        }
        Name = pinName;
    }

 	void onSetItemList()
	{
		// pinItemList changed
	}
    void onSetExtension()
    {
        targetExt = pinExtension;
    }

    void onSetFileName()
    {
    }

 	StringGuiPin pinFolderPath;
 	IntGuiPin pinChoice;
 	StringGuiPin pinItemList;
    StringGuiPin pinExtension;
    StringGuiPin pinName;//local var


public:
	FolderListDirGui()
	{
		initializePin( pinFolderPath, static_cast<MpGuiBaseMemberPtr2>(&FolderListDirGui::onSetFolderPath) );
		initializePin( pinChoice, static_cast<MpGuiBaseMemberPtr2>(&FolderListDirGui::onSetChoice) );
		initializePin( pinItemList, static_cast<MpGuiBaseMemberPtr2>(&FolderListDirGui::onSetItemList) );
        initializePin(pinExtension, static_cast<MpGuiBaseMemberPtr2>(&FolderListDirGui::onSetExtension));
        initializePin(pinName, static_cast<MpGuiBaseMemberPtr2>(&FolderListDirGui::onSetFileName));

	}

    std::vector<std::string> files;
    void listFilesInDirectory()
    {
        if (directoryPath.empty())
            return;

#ifdef __APPLE__
        // macOS / POSIX implementation
        DIR* dir = opendir(directoryPath.c_str());
        if (!dir)
            return;

        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr)
        {
            std::string fname = entry->d_name;

            // Skip "." and ".."
            if (fname == "." || fname == "..")
                continue;

            // Check extension
            std::string ext;
            size_t dotPos = fname.rfind('.');
            if (dotPos != std::string::npos)
                ext = fname.substr(dotPos); // includes dot

            // Convert extension to lowercase
            std::transform(ext.begin(), ext.end(), ext.begin(),
                [](unsigned char c) { return std::tolower(c); });

            if (ext == targetExt)
            {
                // Exclude hidden files (optional)
                if (!fname.empty() && fname.front() != '.')
                {
                    std::string filenameWithoutExt = fname.substr(0, fname.size() - ext.size());
                    files.push_back(filenameWithoutExt);
                }
            }
        }
        closedir(dir);

        // Sort alphabetically, case-insensitive
        std::sort(files.begin(), files.end(),
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
#else
        // Use std::filesystem
        try {
            for (const auto& entry : std::filesystem::directory_iterator(directoryPath))
            {

                if (entry.is_regular_file())
                {
                    std::string fname = entry.path().filename().string();

                    // Filter by extension (case-insensitive)
                    std::string ext = entry.path().extension().string();

                    // Convert extensions to lowercase for comparison
                    std::transform(ext.begin(), ext.end(), ext.begin(),
                        [](unsigned char c) { return std::tolower(c); });

                    if (ext == targetExt)
                    {
                        // Exclude hidden files (optional)
                        if (!fname.empty() && fname.front() != '.')
                        {
                            std::string filenameWithoutExt = fname.substr(0, fname.size() - ext.size());
                            files.push_back(filenameWithoutExt);
                        }
                    }
                }
            }
        }
        catch (const std::filesystem::filesystem_error& e) {
            // Handle errors if needed
        }
#endif

        // Join into comma-separated string
        std::stringstream ss;
        for (size_t i = 0; i < files.size(); ++i)
        {
            ss << files[i];
            if (i != files.size() - 1)
                ss << ", ";
        }
        pinItemList = ss.str();
        pinChoice = 0;
        onSetChoice();
    }

};

namespace
{
	auto r = Register<FolderListDirGui>::withId(L"My FolderListDir");
}

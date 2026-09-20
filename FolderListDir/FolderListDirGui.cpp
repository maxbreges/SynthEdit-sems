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

class FolderListDirGui final : public SeGuiInvisibleBase
{
    std::string directoryPath;
    std::string targetExt;
    std::string prevItemList;

 	StringGuiPin pinFolderPath;
 	StringGuiPin pinItemList;
    StringGuiPin pinExtension;
    BoolGuiPin pinClearVectorMemory;
    BoolGuiPin pinUpdate;

public:
	FolderListDirGui()
	{
		initializePin( pinFolderPath, static_cast<MpGuiBaseMemberPtr2>(&FolderListDirGui::onSetFolderPath) );
		initializePin( pinItemList );
        initializePin(pinExtension, static_cast<MpGuiBaseMemberPtr2>(&FolderListDirGui::onSetExtension));
        initializePin(pinClearVectorMemory, static_cast<MpGuiBaseMemberPtr2>(&FolderListDirGui::onSetClear));
        initializePin(pinUpdate);
    }

    void onSetClear()
    {
        if (pinClearVectorMemory)
        {
            files.clear();
        }
    }

    void onSetUpdate()
    {
        pinUpdate = false;
    }

    void onSetFolderPath()
    {
        if (!files.empty())
        {
            files.clear();
        }
        directoryPath = pinFolderPath;       

        if (directoryPath.empty())
        {
            directoryPath = pinFolderPath;
            if (directoryPath.empty())
            {
                return;
            }
        }

        listFilesInDirectory();
    }

    void onSetExtension()
    {
        targetExt = pinExtension;
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
        prevItemList = pinItemList;
        pinUpdate = true;
        onSetUpdate();
    }
};

namespace
{
	auto r = Register<FolderListDirGui>::withId(L"My FolderListDir");
}

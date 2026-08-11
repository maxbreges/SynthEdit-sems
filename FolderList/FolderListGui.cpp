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

class FolderListGui final : public SeGuiInvisibleBase
{
    std::string fileNameString;
    std::string directoryPath;
    std::string targetExt;

    // Helper function to extract directory path from a full file path
    std::string getDirectoryFromPath(const std::string& filepath)
    {
        size_t p = filepath.find_last_of("/\\");
        if (p != std::string::npos)
            return filepath.substr(0, p);
        else
            return ""; // No directory part found
    }

    // Helper: Get extension 
    std::string getExtension(const std::string& ext)
    {
        size_t dotPos = ext.find_last_of('.');
        if (dotPos != std::string::npos)
            return ext.substr(dotPos);
        return ext;
    }

    // Helper: Get filename without extension from full path
    std::string getFileNameWithoutExtension(const std::string& path)
    {
        // Find the position of the last directory separator
        size_t sepPos = path.find_last_of("/\\");
        size_t startPos = (sepPos != std::string::npos) ? sepPos + 1 : 0;

        // Extract the filename with extension
        std::string filenameWithExt = path.substr(startPos);

        // Find the last dot in the filename to remove extension
        size_t dotPos = filenameWithExt.find_last_of('.');

        // If there's a dot, remove the extension
        if (dotPos != std::string::npos)
        {
            return filenameWithExt.substr(0, dotPos);
        }
        else
        {
            // No extension found, return the filename as is
            return filenameWithExt;
        }
    }

    StringGuiPin pinFilePath;
    IntGuiPin pinChoice;//index
    StringGuiPin pinItemList;  
    StringGuiPin pinExtension;

public:
	FolderListGui()
	{
        initializePin(pinFilePath, static_cast<MpGuiBaseMemberPtr2>(&FolderListGui::onSetPath));
        initializePin(pinChoice, static_cast<MpGuiBaseMemberPtr2>(&FolderListGui::onSetChoice));
        initializePin(pinItemList);
        initializePin(pinExtension);        
    }

    void onSetPath()
    { 
        directoryPath = getDirectoryFromPath(pinFilePath);

        targetExt = getExtension(pinFilePath);

        pinExtension = targetExt;

        Rescan();
    }

    void onSetChoice()
    {
        int choiceIndex = pinChoice;

        std::string separator = PathSeparator;

        if (choiceIndex >= 0 && choiceIndex < static_cast<int>(files.size()))
        {
            std::string filename = files[choiceIndex];
            std::string fullPath = directoryPath + separator + filename + targetExt;
            pinFilePath = fullPath;            
        }
    }

    void Rescan()
    {
        if (!files.empty())
        {
            files.clear();
        }
        if (directoryPath.empty())
            return; // avoid invalid directory access
        listFilesInDirectory();
        setPinChoiceFromPath();
    }

    // Helper: Set pinChoice based on filename
    void setPinChoiceFromPath()
    {
        // Find filename in currentFileList
        for (size_t i = 0; i < files.size(); ++i)
        {            
            if (files[i] == getFileNameWithoutExtension(pinFilePath))
            {
                pinChoice = static_cast<int32_t>(i);
                break;
            }
        }
    }

    std::vector<std::string> files;
    void listFilesInDirectory()
    {
        if (directoryPath.empty())
            return;       
        
#ifdef __APPLE__
        // macOS / POSIX implementation
        DIR* dir = opendir(dirPath.c_str());
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
    }
};

namespace
{
	auto r = Register<FolderListGui>::withId(L"FolderList");
}

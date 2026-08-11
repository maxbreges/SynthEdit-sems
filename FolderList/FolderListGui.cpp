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
    std::string directoryPath;
    std::string Name;
    std::wstring fullFileName;
    std::string targetExt;

    // Helper: Extract directory from path
    std::wstring getDirectory(const std::wstring& path)
    {
        size_t sepPos = path.find_last_of(L"/\\");
        if (sepPos != std::wstring::npos)
            return path.substr(0, sepPos);
        return L"";
    }

    // Helper: Get filename from full path
    std::wstring getFileName(const std::wstring& name)
    {
        size_t sepPos = name.find_last_of(L"/\\");
        if (sepPos != std::wstring::npos)
            return name.substr(sepPos + 1);
        return name;
    }
    // Helper: Get extension 
    std::wstring getExtension(const std::wstring& ext)
    {
        size_t dotPos = ext.find_last_of('.');
        if (dotPos != std::wstring::npos)
            return ext.substr(dotPos);
        return ext;
    }
    // Helper: Remove extension
    std::wstring stripExtension(const std::wstring& filename)
    {
        size_t dotPos = filename.find_last_of('.');
        if (dotPos != std::wstring::npos)
            return filename.substr(0, dotPos);
        return filename;
    }

    StringGuiPin pinFilePath;
    IntGuiPin pinChoice;//index
    StringGuiPin pinItemList;  
    StringGuiPin pinExtension;
    StringGuiPin pinName;//local var
    StringGuiPin pinDirectory;//local var

public:
	FolderListGui()
	{
        initializePin(pinFilePath, static_cast<MpGuiBaseMemberPtr2>(&FolderListGui::onSetPath));
        initializePin(pinChoice, static_cast<MpGuiBaseMemberPtr2>(&FolderListGui::onSetChoice));
        initializePin(pinItemList);
        initializePin(pinExtension); 
        initializePin(pinName, static_cast<MpGuiBaseMemberPtr2>(&FolderListGui::onSetFileName));
        initializePin(pinDirectory);
    }

    void onSetPath()
    { 
        pinDirectory = getDirectory(pinFilePath);

        fullFileName = getFileName(pinFilePath);
        pinName = stripExtension(fullFileName);

        pinExtension = getExtension(pinFilePath);

        directoryPath = pinDirectory;
        Name = pinName;
        targetExt = pinExtension;

        if (!files.empty())
        {
            files.clear();
        }
        if (directoryPath.empty())
            return; // avoid invalid directory access
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
        std::string fullPath = directoryPath + PathSeparator + Name + targetExt;
        pinFilePath = fullPath;
    }

    void onSetFileName()
    {       
    }

    std::vector<std::string> files;
    void listFilesInDirectory()
    {
        if (directoryPath.empty())
            return;       
        
#ifdef __APPLE__
        // macOS / POSIX implementation
        DIR* dir = opendir(directoryPath);
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
        setPinChoiceFromPath();
    }

    // Helper: Set pinChoice based on filename
    void setPinChoiceFromPath()
    {
        // Find filename in currentFileList
        for (size_t i = 0; i < files.size(); ++i)
        {
            if (files[i] == Name)
            {
                pinChoice = static_cast<int32_t>(i);
                break;
            }
        }
    }
};

namespace
{
	auto r = Register<FolderListGui>::withId(L"FolderList");
}

#include "mp_sdk_gui2.h"
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <codecvt>
#include <locale>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <filesystem>
#else
#include <dirent.h>
#include <sys/stat.h>

#endif

using namespace gmpi;

class FileBrowserGui final : public SeGuiInvisibleBase
{
    void onSetAllowedExtensions()
    {
        // pinAllowedExtensions changed
    }

    // Determine platform-specific path separator
#ifdef _WIN32
    static constexpr wchar_t PathSeparator = L'\\';
#else
    static constexpr wchar_t PathSeparator = L'/';
#endif
    // Conversion functions for UTF-8 and wstring
    std::string wstring_to_utf8(const std::wstring& wstr)
    {
        static std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        return conv.to_bytes(wstr);
    }

    std::wstring utf8_to_wstring(const std::string& str)
    {
        static std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        return conv.from_bytes(str);
    }

    // Pins
    StringGuiPin pinDirectory;
    StringGuiPin pinAllowedExtensions;
    StringGuiPin pinItemList;

public:
    FileBrowserGui()
    {
        initializePin(pinDirectory, static_cast<MpGuiBaseMemberPtr2>(&FileBrowserGui::onSetPath));
        initializePin(pinAllowedExtensions, static_cast<MpGuiBaseMemberPtr2>(&FileBrowserGui::onSetAllowedExtensions));
        initializePin(pinItemList);
    }
    // Called when path pin changes
    void onSetPath()
    {
        std::wstring dirPath = pinDirectory.getValue();
        if (dirPath.empty())
            return;

        if (!currentFileList.empty())
        {
            currentFileList.clear();
        }
        if (currentFileList.empty())
        {
            currentDirectory = dirPath;
            listFilesInDirectory();
        }
        //refreshFileList();
    }

private:
    // Internal file list
    std::vector<std::wstring> currentFileList;
    std::wstring currentDirectory;
    std::string targetExt;

    // Helper: List files in directory filtered by extensions
    void listFilesInDirectory()
    {

        std::vector<std::wstring> files;

#if defined(_WIN32) || defined(_WIN64)

        std::filesystem::path dirPath = pinDirectory;
        // Get extension of the selected file (lowercase)
        targetExt = pinAllowedExtensions;

        // Convert targetExt to lowercase
        std::transform(targetExt.begin(), targetExt.end(), targetExt.begin(),
            [](unsigned char c) { return std::tolower(c); });

        for (const auto& entry : std::filesystem::directory_iterator(dirPath))
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
                        std::wstring filenameWithoutExt = utf8_to_wstring(fname.substr(0, fname.size() - ext.size()));
                        files.push_back(filenameWithoutExt);
                    }
                }
            }
        }
#else
        // Convert directory to UTF-8 string
        std::string dirUtf8 = wstring_to_utf8(directory);

        DIR* dirp = opendir(dirUtf8.c_str());
        if (!dirp)
            return files;

        struct dirent* dp;
        while ((dp = readdir(dirp)) != nullptr)
        {
            // Skip "." and ".."
            if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
                continue;

            // Convert filename to wstring
            std::string filenameStr(dp->d_name);
            std::wstring filenameW = utf8_to_wstring(filenameStr);
            files.push_back(filenameW);
    }
        closedir(dirp);
#endif
        
        // Join into comma-separated string
        std::wstringstream ss;
        for (size_t i = 0; i < files.size(); ++i)
        {
            ss << files[i];
            if (i != files.size() - 1)
                ss << ", ";
        }
        pinItemList = ss.str();
    }

    // Helper: Filter files by extension(s)
    std::vector<std::wstring> filterFiles(const std::vector<std::wstring>& files)
    {
        std::vector<std::wstring> filtered;

        // Get allowed extensions
        std::wstring extStr = pinAllowedExtensions; // e.g., "wav,mp3"
        std::vector<std::wstring> allowedExts;
        size_t pos = 0;
        while ((pos = extStr.find(L',')) != std::wstring::npos)
        {
            allowedExts.push_back(extStr.substr(0, pos));
            extStr.erase(0, pos + 1);
        }
        if (!extStr.empty())
            allowedExts.push_back(extStr);

        // Convert allowed extensions to lowercase for comparison
        for (auto& e : allowedExts)
            std::transform(e.begin(), e.end(), e.begin(), ::tolower);

        for (const auto& filename : files)
        {
            if (allowedExts.empty() || pinAllowedExtensions.getValue().empty())
            {
                // No extension filter
                filtered.push_back(filename);
            }
            else
            {
                // Check extension
                size_t dotPos = filename.find_last_of('.');
                if (dotPos != std::wstring::npos)
                {
                    std::wstring fileExt = filename.substr(dotPos + 1);
                    std::transform(fileExt.begin(), fileExt.end(), fileExt.begin(), ::tolower);
                    if (std::find(allowedExts.begin(), allowedExts.end(), fileExt) != allowedExts.end())
                    {
                        filtered.push_back(filename);
                    }
                }
            }
        }
        return filtered;
    }

    // Helper: Remove extension 
    std::wstring stripExtension(const std::wstring& filename)
    {

        size_t dotPos = filename.find_last_of('.');
        if (dotPos != std::wstring::npos)
            return filename.substr(0, dotPos);
        return filename;
    }

    // Helper: Update pinItemList based on currentFileList
    void updatePinItemList()
    {
        std::wstring itemListStr;

        for (size_t i = 0; i < currentFileList.size(); ++i)
        {
            std::wstring name = stripExtension(currentFileList[i]);
            
            itemListStr += name;
            if (i != currentFileList.size() - 1)
                itemListStr += L",";            
        }
        pinItemList = itemListStr;         
    }

    // Helper: Get filename from full path
    std::wstring getFileNameFromPath(const std::wstring& path)
    {
        size_t sepPos = path.find_last_of(L"/\\");
        if (sepPos != std::wstring::npos)
            return path.substr(sepPos + 1);
        return path;
    }

private:
    // Refresh file list based on current directory and filters
/*    void refreshFileList()
    {
        
        if (currentDirectory.empty())
            return;
        
        currentFileList = listFilesInDirectory(currentDirectory);
        currentFileList = filterFiles(currentFileList);
        
        std::locale loc; // default locale
        // ...
        
        std::sort(currentFileList.begin(), currentFileList.end(), [&](const std::wstring& a, const std::wstring& b) {
            return std::use_facet<std::collate<wchar_t>>(loc).compare(a.data(), a.data() + a.size(), b.data(), b.data() + b.size()) < 0;
            });

        updatePinItemList();
        
    }*/

};

namespace
{
	auto r = Register<FileBrowserGui>::withId(L"My FileBrowser");
}

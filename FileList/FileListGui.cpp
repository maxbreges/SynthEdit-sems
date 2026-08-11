#include "mp_sdk_gui2.h"
#include <sstream>
#include <filesystem>

using namespace gmpi;

class FileListGui final : public SeGuiInvisibleBase
{
    std::string targetExt;
    std::string dirPath;

 	void onSetDirectory()
	{  
        dirPath = pinDirectory;
        targetExt = pinExtension;
        // Convert targetExt to lowercase
        std::transform(targetExt.begin(), targetExt.end(), targetExt.begin(),
            [](unsigned char c) { return std::tolower(c); });
        
        listFilesInDirectory();
	}

 	StringGuiPin pinDirectory;
	StringGuiPin pinExtension;
 	StringGuiPin pinItemList; 	

public:
	FileListGui()
	{
		initializePin( pinDirectory, static_cast<MpGuiBaseMemberPtr2>(&FileListGui::onSetDirectory) );
		initializePin( pinExtension, static_cast<MpGuiBaseMemberPtr2>(&FileListGui::onSetDirectory));
		initializePin(pinItemList);
	}

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

    void listFilesInDirectory()
    {
        if (dirPath.empty())
            return;        

       std::vector<std::string> files;
       
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
                        std::string filenameWithoutExt = fname.substr(0, fname.size() - ext.size());
                        files.push_back(filenameWithoutExt);
                    }
                }                
            }
        }

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
	auto r = Register<FileListGui>::withId(L"My FileList");
}

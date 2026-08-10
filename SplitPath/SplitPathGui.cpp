#include "mp_sdk_gui2.h"

using namespace gmpi;

// Determine platform-specific path separator
#ifdef _WIN32
static const std::string PathSeparator = "\\";
#else
static const std::string PathSeparator = "/";
#endif

class SplitPathGui final : public SeGuiInvisibleBase
{
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

 	void onSetFilePath()
	{
        pinDirectory = getDirectory(pinFilePath);       

        fullFileName = getFileName(pinFilePath);
        pinName = stripExtension(fullFileName);        

        pinExtension = getExtension(pinFilePath); 

        Directory = pinDirectory;
        Name = pinName;
        Extension = pinExtension;
	}

    void onSetFileName()
    { 
        Name = pinName;
        std::string fullPath = Directory + PathSeparator + Name + Extension;
        pinFilePath = fullPath;
    }

    std::string Directory;
    std::string Name;
    std::wstring fullFileName;
    std::string Extension;


 	StringGuiPin pinFilePath;
 	StringGuiPin pinDirectory;
 	StringGuiPin pinName;
 	StringGuiPin pinExtension;

public:
	SplitPathGui()
	{
		initializePin( pinFilePath, static_cast<MpGuiBaseMemberPtr2>(&SplitPathGui::onSetFilePath) );
		initializePin( pinDirectory );
		initializePin( pinName, static_cast<MpGuiBaseMemberPtr2>(&SplitPathGui::onSetFileName));
		initializePin( pinExtension );
	}
};

namespace
{
	auto r = Register<SplitPathGui>::withId(L"My SplitPath");
}

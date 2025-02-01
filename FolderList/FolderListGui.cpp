#include "mp_sdk_gui2.h"
#include <string>
#include <sstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

using namespace gmpi;

class FolderListGui final : public SeGuiInvisibleBase
{
 	void onSetPath()
	{		
		FolderList();
	}

    void onSetShowExt()
    {
    }

    int32_t FolderList()
    {
        std::stringstream buffer;

        std::string path = pinPath; // Ensure pinPath is defined somewhere.

        int totalFiles = 0;
        int fileCount = 0;

        // Check if the path exists and is a directory
        if (!fs::exists(path) || !fs::is_directory(path)) {
            std::cerr << "Invalid directory: " << path << std::endl;
            return -1; // Error code for invalid directory
        }

        try 
        {
                if (!pinShowExtension)
                {
                    for (const auto& entry : fs::directory_iterator(path))
                    {
                        ++totalFiles;
                    }
                    for (const auto& entry : fs::directory_iterator(path))
                    {
                        buffer << entry.path().filename().stem().string() << std::endl; // Add path to buffer
                        ++fileCount;
                        if (fileCount < totalFiles)
                        {
                            buffer << "," << std::endl;
                        }
                    }
                    
                }
                if (pinShowExtension)
                {
                    for (const auto& entry : fs::directory_iterator(path))
                    {
                        ++totalFiles;
                    }
                    for (const auto& entry : fs::directory_iterator(path))
                    {
                        buffer << entry.path().filename().string() << std::endl; // Add path to buffer
                        ++fileCount;
                        if (fileCount < totalFiles)
                        {
                            buffer << "," << std::endl;
                        }
                    }
                   
                }                   

        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
            return -2; // Error code for filesystem error
        }
        catch (const std::exception& e) {
            std::cerr << "General error: " << e.what() << std::endl;
            return -3; // General error code
        }

        // Use a std::stringstream to create the file list
        std::string fileList = buffer.str();
        pinFolderList = fileList; // Assuming pinFolderList is defined as std::string or similar
        pinListSize = fileCount-1;
        return 0; // Success
    }

 	StringGuiPin pinPath;
    BoolGuiPin pinShowExtension;
    IntGuiPin pinListSize;
 	StringGuiPin pinFolderList;

public:
	FolderListGui()
	{
		initializePin( pinPath, static_cast<MpGuiBaseMemberPtr2>(&FolderListGui::onSetPath) );
        initializePin(pinShowExtension, static_cast<MpGuiBaseMemberPtr2>(&FolderListGui::onSetShowExt));
        initializePin(pinListSize, static_cast<MpGuiBaseMemberPtr2>(&FolderListGui::onSetPath));
		initializePin( pinFolderList, static_cast<MpGuiBaseMemberPtr2>(&FolderListGui::onSetPath) );
	}

};

namespace
{
	auto r = Register<FolderListGui>::withId(L"FolderList");
}

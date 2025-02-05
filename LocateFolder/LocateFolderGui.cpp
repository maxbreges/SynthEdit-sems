#include "mp_sdk_gui2.h"
#include <filesystem>
#include <sstream>
#include <vector>
#include <Cocoa/Cocoa.h>  // Include Cocoa framework


using namespace gmpi;
namespace fs = std::filesystem;

class LocateFolderGui final : public SeGuiInvisibleBase
{
	bool m_prev_trigger = false;
	std::vector<std::wstring> m_fileNames;// Store the list of files for index tracking
	std::vector<std::wstring> m_fileNamesEx;// Store the list of files for index tracking
	std::wstring pinSelectedFile;
	
	void onSetTrigger()
	{
		if (pinTrigger == false && m_prev_trigger == true) // dialog triggered on mouse-up (else dialog grabs focus, button never resets)
		{
			std::wstring selectedFile = openFileDialog();
			std::stringstream Directory;

			if (!selectedFile.empty())
			{				
				Directory << fs::path(selectedFile).parent_path().string() + "\\";		

				std::string directory(Directory.str());
		
				pinDirectory = directory;

				pinSelectedFile = selectedFile.c_str();

				// Populate items list and choice
				updateItemsList(fs::path(selectedFile).parent_path());				
			}
		}
			m_prev_trigger = pinTrigger;
	}

 	void onSetDirectory()
	{
		// pinDirectory changed
	}

	void onSetItemsList()
	{
		// pinListItems changed
	}

	

	void onSetChoice()
	{
	}	

 	BoolGuiPin pinTrigger;
	StringGuiPin pinDirectory;
 	StringGuiPin pinItemsList;
	StringGuiPin pinItemsListExt;
	IntGuiPin pinChoice;
	StringGuiPin pinExtension;

public:
	LocateFolderGui()
	{
		initializePin( pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&LocateFolderGui::onSetTrigger) );
		initializePin(pinDirectory, static_cast<MpGuiBaseMemberPtr2>(&LocateFolderGui::onSetDirectory));
		initializePin( pinItemsList, static_cast<MpGuiBaseMemberPtr2>(&LocateFolderGui::onSetItemsList) );
	    initializePin(pinItemsListExt, static_cast<MpGuiBaseMemberPtr2>(&LocateFolderGui::onSetItemsList));
		initializePin(pinChoice, static_cast<MpGuiBaseMemberPtr2>(&LocateFolderGui::onSetChoice));
		initializePin(pinExtension, static_cast<MpGuiBaseMemberPtr2>(&LocateFolderGui::onSetItemsList));
	}

	std::wstring openFileDialog()
{
    // Create an NSOpenPanel
    NSOpenPanel* openPanel = [NSOpenPanel openPanel];
    [openPanel setAllowedFileTypes:@[(NSString *)kUTTypeAll]];
    [openPanel setCanChooseFiles:YES];
    [openPanel setCanChooseDirectories:NO];
    [openPanel setAllowsMultipleSelection:NO];

    // Run the open panel modally and retrieve the selected file
    if ([openPanel runModal] == NSModalResponseOK)
    {
        NSURL *selectedFileURL = [[openPanel URLs] firstObject];
        NSString *filePath = [selectedFileURL path]; // Get the string path
        return std::wstring(filePath.UTF8String); // Convert to std::wstring
    }

    return L""; // return an empty string if cancelled
}

	void updateItemsList(const fs::path& directory)
	{
	
		m_fileNames.clear(); // Clear previous file names
		m_fileNamesEx.clear(); // Clear previous file names
		if (fs::exists(directory) && fs::is_directory(directory))
		{
			for (const auto& entry : fs::directory_iterator(directory))
			{
				if (entry.is_regular_file())
				{
					// Store the full filename (with extension) in m_fileNames
					m_fileNames.push_back(entry.path().filename().wstring()); // <-- Changed this line
				}
			}

			for (const auto& entry : fs::directory_iterator(directory))
			{
				if (entry.is_regular_file())
				{
					// Store the full filename (with extension) in m_fileNames
					m_fileNamesEx.push_back(entry.path().filename().stem().wstring()); // 
					std::stringstream Extension;
					Extension << fs::path(pinSelectedFile).extension().string();
					pinExtension = Extension.str();
				}
			}
		}

		// Join file names into a comma-separated list for display
		std::wstringstream ss;
		std::wstringstream ssEx;

		for (size_t i = 0; i < m_fileNames.size(); ++i)
		{
			ss << m_fileNames[i];
			if (i < m_fileNames.size() - 1)
				ss << L", "; // Add comma between items
		}
		for (size_t i = 0; i < m_fileNamesEx.size(); ++i)
		{
			ssEx << m_fileNamesEx[i];
			if (i < m_fileNames.size() - 1)
			ssEx << L", "; // Add comma between items
		}
		pinItemsList = ss.str();

		pinItemsListExt = ssEx.str();		

		onSetSelectedFile();
	}

	void onSetSelectedFile()
	{
		if (m_fileNames.empty())
		{
			pinChoice = -5; // Or another default value to indicate no selection
			return;
		}

		std::wstring selectedFile = fs::path(pinSelectedFile).filename().wstring();

		auto it = std::find(m_fileNames.begin(), m_fileNames.end(), selectedFile);

		if (it != m_fileNames.end())
		{
			pinChoice = static_cast<int>(std::distance(m_fileNames.begin(), it));
		}
		else
		{
			pinChoice = -7; // File not found, set to -1 (or appropriate value)
		}
	}

};

namespace
{
	auto r = Register<LocateFolderGui>::withId(L"LocateFolder");
}

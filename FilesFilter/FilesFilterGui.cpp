#include "mp_sdk_gui2.h"
#include "mp_gui.h"
#include <filesystem>
#include <vector>
#include <sstream>
#include <algorithm> // For std::transform
#include <locale> // For std::locale and std::tolower

namespace fs = std::filesystem;

using namespace gmpi;
using namespace gmpi_gui;

class FilesFilterGui final : public SeGuiInvisibleBase
{
	std::string fullfilename;
	bool m_prev_trigger = false;
	GmpiGui::FileDialog nativeFileDialog;
	std::vector<std::wstring> m_fileNames;// Store the list of files for index tracking

	std::wstring toLower(const std::wstring& str) {
		std::wstring lowerStr = str;
		std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
			[](wchar_t c) { return std::tolower(c); });
		return lowerStr;
	}

	void onSetTrigger()
	{
		// Check if the trigger has just been set (mouse button released)
		// 'pinTrigger' represents the current state of the trigger (pressed or not)
		// 'm_prev_trigger' holds the previous state of the trigger
		if (pinTrigger == false && m_prev_trigger == true) // dialog triggered on mouse-up (else dialog grabs focus, button never resets)
		{
			IMpGraphicsHostBase* dialogHost = 0;  // Initialize a pointer for the dialog host interface
			getHost()->queryInterface(SE_IID_GRAPHICS_HOST_BASE, reinterpret_cast<void**>(&dialogHost)); // Query the host for the graphics interface

			// Check if we successfully obtained the dialog host interface
			if (dialogHost != 0)
			{
				dialogHost->createFileDialog(0, nativeFileDialog.GetAddressOf()); // Create the file dialog

				// Check if the dialog was created successfully
				if (!nativeFileDialog.isNull())
				{
					// Show the dialog asynchronously and pass the callback function to handle the result
					nativeFileDialog.ShowAsync([this](int32_t result) -> void { this->OnFileDialogComplete(result); });
				}
			}
		}

		// Update the previous trigger state for the next event
		m_prev_trigger = pinTrigger;
	}

	int32_t OnFileDialogComplete(int32_t result)
	{
		if (result == gmpi::MP_OK)
		{
			fullfilename = nativeFileDialog.GetSelectedFilename();
		}
		onSetText();
		//onSetItemsList();
		onSetDirectory();
		nativeFileDialog.setNull(); // release it.

		return 0;
	}

	void onSetDirectory()
	{	
		pinDirectory = (fs::path(fullfilename).parent_path());
	}

	void onSetText()
	{
		// Get the current text from the pinText pin
		std::wstring searchText = pinText;

		// Filter the file list based on the search text
		m_fileNames.clear(); // Clear previous file names

		auto updateItemsListPath(fs::path(fullfilename).parent_path());		

		if (fs::exists(updateItemsListPath) && fs::is_directory(updateItemsListPath))
		{
			for (const auto& entry : fs::directory_iterator(updateItemsListPath))
			{
				if (pinCaseSensitive)
				{
					if (entry.is_regular_file() && entry.path().filename().wstring().find(searchText) != std::wstring::npos)
					{
						// Store the full filename (with extension) in m_fileNames
						m_fileNames.push_back(entry.path().filename().wstring()); // the list of filenames
					}
				}

				else

				if (entry.is_regular_file())
				{
					// Convert the filename to lowercase for comparison
					std::wstring filename = toLower(entry.path().filename().wstring());

					// Perform case-insensitive search
					if (filename.find(searchText) != std::wstring::npos)
					{
						// Store the full filename (with extension) in m_fileNames
						m_fileNames.push_back(entry.path().filename().wstring()); // the list of filenames
					}
				}				
			}
		}

		// Join file names into a comma-separated list for display
		std::wstringstream ss;

		for (size_t i = 0; i < m_fileNames.size(); ++i)
		{
			ss << m_fileNames[i];
			if (i < m_fileNames.size() - 1)
				ss << L", "; // Add comma between items
		}

		pinItemsList = ss.str(); //output list to the pin
	}

 	void onSetItemsList()
	{
		updateItemsList(fs::path(fullfilename).parent_path());
	}

	void onSetCaseSensitive()
	{
	}

 	BoolGuiPin pinTrigger;
	StringGuiPin pinDirectory;
 	StringGuiPin pinText;
 	StringGuiPin pinItemsList;
	BoolGuiPin pinCaseSensitive;
 	
public:
	FilesFilterGui()
	{
		initializePin( pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&FilesFilterGui::onSetTrigger) );
		initializePin(pinDirectory, static_cast<MpGuiBaseMemberPtr2>(&FilesFilterGui::onSetDirectory));
		initializePin(pinText, static_cast<MpGuiBaseMemberPtr2>(&FilesFilterGui::onSetText));
		initializePin( pinItemsList, static_cast<MpGuiBaseMemberPtr2>(&FilesFilterGui::onSetItemsList) );
		initializePin(pinCaseSensitive, static_cast<MpGuiBaseMemberPtr2>(&FilesFilterGui::onSetCaseSensitive));
	}

	void updateItemsList(const fs::path& directory)
	{
		//m_fileNames.clear(); // Clear previous file names

		if (fs::exists(directory) && fs::is_directory(directory))
		{
			for (const auto& entry : fs::directory_iterator(directory))
			{
				if (entry.is_regular_file())
				{
					// Store the full filename (with extension) in m_fileNames
					m_fileNames.push_back(entry.path().filename().wstring()); // the list of filenames
				}
			}
		}

		// Join file names into a comma-separated list for display
		std::wstringstream ss;

		for (size_t i = 0; i < m_fileNames.size(); ++i)
		{
			ss << m_fileNames[i];
			if (i < m_fileNames.size() - 1)
				ss << L", "; // Add comma between items
		}

		pinItemsList = ss.str(); //output list to the pin
	}
};

namespace
{
	auto r = Register<FilesFilterGui>::withId(L"FilesFilter");
}

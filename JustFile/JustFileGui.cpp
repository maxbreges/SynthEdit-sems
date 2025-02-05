#include "mp_sdk_gui2.h"
#include "mp_gui.h"

#include "C:\modules\shared\unicode_conversion.h"
#include "C:\modules\shared\it_enum_list.h"
#include "C:\modules\shared\string_utilities.h"
#include "MpString.h"
#include <sstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <iterator>

namespace fs = std::filesystem;

using namespace gmpi;
using namespace gmpi_gui;
using namespace gmpi_sdk;
using namespace JmUnicodeConversions;

class JustFileGui final : public SeGuiInvisibleBase
{
	bool m_prev_trigger = false;
	GmpiGui::FileDialog nativeFileDialog;
	std::vector<std::string> files;

 	void onSetFileName()
	{
	}

 	void onSetFileExtension()
	{
		// pinFileExtension changed
	}

	void onSetTrigger()
	{
		// Check if the trigger has just been set (mouse button released)
		// 'pinTrigger' represents the current state of the trigger (pressed or not)
		// 'm_prev_trigger' holds the previous state of the trigger
		if (pinTrigger == false && m_prev_trigger == true) // dialog triggered on mouse-up (else dialog grabs focus, button never resets)
		{
			std::wstring filename = pinFileName;  // Store the current filename from the input pin
			std::wstring file_extension = pinFileExtension;  // Store the current file extension from the input pin

			IMpGraphicsHostBase* dialogHost = 0;  // Initialize a pointer for the dialog host interface
			getHost()->queryInterface(SE_IID_GRAPHICS_HOST_BASE, reinterpret_cast<void**>(&dialogHost)); // Query the host for the graphics interface

			// Check if we successfully obtained the dialog host interface
			if (dialogHost != 0)
			{
				int dialogMode = (int)pinSaveMode; // Get the dialog mode (whether to save or open)
				dialogHost->createFileDialog(dialogMode, nativeFileDialog.GetAddressOf()); // Create the file dialog

				// Check if the dialog was created successfully
				if (!nativeFileDialog.isNull())
				{
					nativeFileDialog.AddExtensionList(pinFileExtension); // Add the file extension filter to the dialog

					auto filename = pinFileName.getValue(); // Retrieve the filename value from the 'pinFileName' pin
					if (!filename.empty())  // Check if the filename is not empty
					{
						filename = uiHost.resolveFilename(filename); // Resolve the file name to an absolute path
						nativeFileDialog.SetInitialFullPath(JmUnicodeConversions::WStringToUtf8(filename)); // Set the resolved file name as the initial path in the dialog
					}
					else
					{
						// If the filename is empty, set the initial directory of the dialog to a default folder based on the file extension
						nativeFileDialog.setInitialDirectory(getDefaultFolder(pinFileExtension));
					}

					// Show the dialog asynchronously and pass the callback function to handle the result
					nativeFileDialog.ShowAsync([this](int32_t result) -> void { this->OnFileDialogComplete(result); });
				}
			}
		}

		// Update the previous trigger state for the next event
		m_prev_trigger = pinTrigger;
	}

 	void onSetSaveMode()
	{
		// pinSaveMode changed
	}

	std::string lastKnownDirectory; // A class-level static variable

	void onSetChoice()
	{
		if (pinChoice >= 0 && pinChoice < files.size())
		{
			std::string dir;
			std::string nam;
			std::string con;
			dir = pinDirectory;

			pinSelection = files[pinChoice];
			nam = pinSelection;
			con = dir + nam;

			// Update pinFileName with the filename corresponding to pinChoice
			pinFileName = con; // Use the full filename with extension

			// Update last known directory if the directory is valid
			if (!dir.empty()) {
				lastKnownDirectory = pinDirectory;
			}
			// Check if pinDirectory was cleared and, if so, set it to the last known directory
			if (dir.empty()) {
				pinDirectory = lastKnownDirectory;
			}
			
		}
	}
	void onSetSelection()
	{
	}
	void onSetListItems()
	{
	}
	void onSetDirectory()
	{
	}

 	StringGuiPin pinFileName;
 	StringGuiPin pinFileExtension;
 	BoolGuiPin pinTrigger;
 	BoolGuiPin pinSaveMode;
	StringGuiPin pinSelection;
	IntGuiPin pinChoice;
	StringGuiPin pinListItems;
	StringGuiPin pinDirectory;

public:
	JustFileGui()
	{
		initializePin( pinFileName, static_cast<MpGuiBaseMemberPtr2>(&JustFileGui::onSetFileName) );
		initializePin( pinFileExtension, static_cast<MpGuiBaseMemberPtr2>(&JustFileGui::onSetFileExtension) );
		initializePin( pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&JustFileGui::onSetTrigger) );
		initializePin( pinSaveMode, static_cast<MpGuiBaseMemberPtr2>(&JustFileGui::onSetSaveMode) );
		initializePin(pinSelection, static_cast<MpGuiBaseMemberPtr2>(&JustFileGui::onSetSelection));
		initializePin(pinChoice, static_cast<MpGuiBaseMemberPtr2>(&JustFileGui::onSetChoice));
		initializePin(pinListItems, static_cast<MpGuiBaseMemberPtr2>(&JustFileGui::onSetListItems));
		initializePin(pinDirectory, static_cast<MpGuiBaseMemberPtr2>(&JustFileGui::onSetDirectory));
	}

	std::string getDefaultFolder(std::wstring extension)
	{
		const std::wstring searchFilename = L"dummy." + extension;
		const auto fullFileName = uiHost.resolveFilename(searchFilename.c_str());
		return JmUnicodeConversions::WStringToUtf8(fullFileName.substr(0, fullFileName.find(L"dummy") - 1));
	}

	int32_t OnFileDialogComplete(int32_t result)
	{
		if (result == gmpi::MP_OK)
		{
			// Trim filename if in default folder.
			auto filepath = nativeFileDialog.GetSelectedFilename();
			auto fileext = GetExtension(filepath);
			const char* fileclass = nullptr;

			// Retrieve the directory from the file path
			fs::path filePath(filepath);
			fs::path directory = filePath.parent_path(); // Get the directory
			std::string filename = filePath.filename().string(); // Get the file name

			// Convert directory to string
			std::string dirStr = directory.string();

			// Ensure directory ends with a backslash
			if (!dirStr.empty() && dirStr.back() != '\\')
			{
				dirStr += '\\'; // Append backslash if it's not already there
			}			

			pinDirectory = dirStr; // Set pinDirectory with ensured trailing backslash
			pinFileName = dirStr + filename; // Update pinFileName with full path
			pinSelection = filename;

			// Check if the directory exists
			if (!fs::exists(directory) || !fs::is_directory(directory))
			{
				return -1; // Indicate that the directory is invalid
			}	

			files.clear(); // Add this line to clear previous files

			// List all files in the directory
			for (const auto& entry : fs::directory_iterator(directory))
			{
				if (fs::is_regular_file(entry))
				{
					files.push_back(entry.path().filename().string()); // Store full filename
				}
			}

			// Initialize or clear pinChoice
			pinChoice = -1; // Default value indicating not found

			// Search for the file and set pinChoice
			auto it = std::find(files.begin(), files.end(), filename);
			if (it != files.end())
			{
				pinChoice = std::distance(files.begin(), it); // Update the index
			}
			

			// Prepare the pinListItems for external visibility (if needed)
			std::ostringstream oss;
			for (const auto& file : files)
			{
				oss << file << ",";
			}
			pinListItems = oss.str();

			// File classification based on extension
			if (fileext == "sf2" || fileext == "sfz") {
				fileclass = "Instrument";
			}
			else if (fileext == "png" || fileext == "bmp" || fileext == "jpg") {
				fileclass = "Image";
			}
			else if (fileext == "wav") {
				fileclass = "Audio";
			}
			else if (fileext == "mid") {
				fileclass = "MIDI";
			}

			if (fileclass) {
				auto shortName = StripPath(filepath);
				const auto r = uiHost.FindResourceU(shortName.c_str(), fileclass);

				// If resource is found, use the short name
				if (filepath == r) {
					filepath = shortName;
				}
			}

			//pinFileName = filepath;
		}

		nativeFileDialog.setNull(); // release it.

		return 0;
	}
};

namespace
{
	auto r = Register<JustFileGui>::withId(L"JustFile");
}

#include "FileDialogGui.h"
#include "mp_sdk_gui2.h"

#include "C:\modules\shared\unicode_conversion.h"
#include "C:\modules\shared\it_enum_list.h"
#include "C:\modules\shared\string_utilities.h"
#include "MpString.h"
#include <sstream>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

using namespace gmpi;
using namespace gmpi_gui;
using namespace gmpi_sdk;
using namespace JmUnicodeConversions;

GMPI_REGISTER_GUI(MP_SUB_TYPE_GUI2, FileDialogGui, L"FileDialog");

FileDialogGui::FileDialogGui() :
	m_prev_trigger(false)

{
	// initialise pins.
	initializePin(pinFileName, static_cast<MpGuiBaseMemberPtr2>(&FileDialogGui::onSetPath));
	initializePin(pinFileExtension);
	initializePin(pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&FileDialogGui::onSetTrigger));
	initializePin(pinSaveMode);
	initializePin(pinDirectory, static_cast<MpGuiBaseMemberPtr2>(&FileDialogGui::onSetPath));
	initializePin(pinListItems, static_cast<MpGuiBaseMemberPtr2>(&FileDialogGui::onSetPath));
	initializePin(pinChoice, static_cast<MpGuiBaseMemberPtr2>(&FileDialogGui::onSetPath));
	initializePin(pinSelection, static_cast<MpGuiBaseMemberPtr2>(&FileDialogGui::onSetPath));
}

void FileDialogGui::onSetPath()
{
	std::string filename = pinFileName;
	std::stringstream Directory;
	Directory << fs::path(filename).parent_path().string() + "\\";
	std::string directory(Directory.str());
	pinDirectory = directory;
	//onSetSelectedFile();
	updateItemsList(fs::path(pinFileName).parent_path());	
}

std::string FileDialogGui::getDefaultFolder(std::wstring extension)
{
	const std::wstring searchFilename = L"dummy." + extension;
	const auto fullFileName = uiHost.resolveFilename(searchFilename.c_str());
	return JmUnicodeConversions::WStringToUtf8(fullFileName.substr(0, fullFileName.find(L"dummy") - 1));
}

void FileDialogGui::onSetTrigger()
{
	// trigger on mouse-up
	if (pinTrigger == false && m_prev_trigger == true) // dialog triggered on mouse-up (else dialog grabs focus, button never resets)
	{
		std::wstring filename = pinFileName;
		std::wstring file_extension = pinFileExtension;

		IMpGraphicsHostBase* dialogHost = 0;
		getHost()->queryInterface(SE_IID_GRAPHICS_HOST_BASE, reinterpret_cast<void**>(&dialogHost));

		if (dialogHost != 0)
		{
			int dialogMode = (int)pinSaveMode;
			dialogHost->createFileDialog(dialogMode, nativeFileDialog.GetAddressOf());

			if (!nativeFileDialog.isNull())
			{
				nativeFileDialog.AddExtensionList(pinFileExtension);

				auto filename = pinFileName.getValue();
				if (!filename.empty())
				{
					filename = uiHost.resolveFilename(filename);
					nativeFileDialog.SetInitialFullPath(JmUnicodeConversions::WStringToUtf8(filename));
					
				}
				else
				{
					nativeFileDialog.setInitialDirectory(getDefaultFolder(pinFileExtension));
				}

				nativeFileDialog.ShowAsync([this](int32_t result) -> void { this->OnFileDialogComplete(result); });
				
			}
		}
	}

	m_prev_trigger = pinTrigger;
	
}

void FileDialogGui::updateItemsList(const fs::path& directory)
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
				Extension << fs::path(pinFileName).extension().string();
				//pinExtension = Extension.str();
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
	pinListItems = ss.str();

	//pinListItemsExt = ssEx.str();

	onSetSelectedFile();
}

void FileDialogGui::onSetSelectedFile()
{
	if (m_fileNames.empty())
	{
		pinChoice = -1; // Or another default value to indicate no selection
		return;
	}

	std::wstring selectedFile = fs::path(pinFileName).filename().wstring();

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

void FileDialogGui::OnFileDialogComplete(int32_t result)
{
	if (result == gmpi::MP_OK)
	{
		// Trim filename if in default folder.
		auto filepath = nativeFileDialog.GetSelectedFilename();
		auto fileext = GetExtension(filepath);
		const char* fileclass = nullptr;
		
		std::stringstream ss;
		ss << fs::path(filepath).stem().string();
		std::string filename(ss.str());
		pinSelection = filename;

		if (fileext == "sf2" || fileext == "sfz")
		{
			fileclass = "Instrument";
		}
		else
		{
			if (fileext == "png" || fileext == "bmp" || fileext == "jpg")
			{
				fileclass = "Image";
			}
			else
			{
				if (fileext == "wav")
				{
					fileclass = "Audio";
				}
				else
				{
					if (fileext == "mid")
					{
						fileclass = "MIDI";
					}
				}
			}
		}

		if (fileclass)
		{
			auto shortName = StripPath(filepath);

			const auto r = uiHost.FindResourceU(shortName.c_str(), fileclass);

			if (filepath == r)
			{
				filepath = shortName;
			}
		}

		pinFileName = filepath;
		
	}

	nativeFileDialog.setNull(); // release it.
}

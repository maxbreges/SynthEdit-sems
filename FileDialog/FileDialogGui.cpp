#include "./FileDialogGui.h"

#include "../shared/unicode_conversion.h"
#include "../shared/it_enum_list.h"
#include "../shared/string_utilities.h"
#include "../se_sdk3/MpString.h"
#include <sstream>

using namespace gmpi;
using namespace gmpi_gui;
using namespace gmpi_sdk;
using namespace JmUnicodeConversions;

GMPI_REGISTER_GUI(MP_SUB_TYPE_GUI2, FileDialogGui, L"FileDialog");

FileDialogGui::FileDialogGui() :
	m_prev_trigger(false)
{
	// initialise pins.
	initializePin(pinFileName);
	initializePin(pinFileExtension);
	initializePin(pinChoice, static_cast<MpGuiBaseMemberPtr2>(&FileDialogGui::onSetChoice));
	initializePin(pinItemsList);
	initializePin(pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&FileDialogGui::onSetTrigger));
	initializePin(pinSaveMode);
	initializePin(pinDirectory);
	initializePin(pinDebug);
	initializePin(pinItemsListIn, static_cast<MpGuiBaseMemberPtr2>(&FileDialogGui::onSetItemsList));
	initializePin(pinOpened);
}


std::string FileDialogGui::getDefaultFolder(std::wstring extension)
{
	const std::wstring searchFilename = L"dummy." + extension;
	const auto fullFileName = uiHost.resolveFilename(searchFilename.c_str());
	return JmUnicodeConversions::WStringToUtf8(fullFileName.substr(0, fullFileName.find(L"dummy") - 1));
}

void FileDialogGui::onSetTrigger()
{
	pinOpened = pinTrigger;
	// trigger on mouse-up
	if (!pinTrigger  && m_prev_trigger == true) // dialog triggered on mouse-up (else dialog grabs focus, button never resets)
	{
		std::ostringstream oss;
		oss << "onSetTrigger called: pinTrigger=" << pinTrigger << ", m_prev_trigger=" << m_prev_trigger << "\n";
		pinDebug = oss.str();
		std::wstring filename = pinFileName;
		std::wstring file_extension = pinFileExtension;

		IMpGraphicsHostBase* dialogHost = 0;
		getHost()->queryInterface(SE_IID_GRAPHICS_HOST_BASE, reinterpret_cast<void**>(&dialogHost));

		if (dialogHost != 0)
		{
			//pinOpened = true;
			int dialogMode = (int)pinSaveMode;
			dialogHost->createFileDialog(dialogMode, nativeFileDialog.GetAddressOf());

			if ( ! nativeFileDialog.isNull())
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
	//pinOpened = m_prev_trigger;
}

std::string fileext;

void FileDialogGui::onSetItemsList()
{
	std::wstring itemsList = pinItemsListIn.getValue();
	m_fileNames.clear();

	std::wistringstream wss(itemsList);
	std::wstring item;

	while (std::getline(wss, item, L',')) {
		if (!item.empty()) // Avoid empty entries
			m_fileNames.push_back(item);
	}

	//pinDebug = L"onSetItemsList()";
	//onSetChoice();
}

void FileDialogGui::onSetChoice()
{
	if (pinChoice >= 0 && pinChoice < m_fileNames.size())
	{
		std::wstring filenameOnly = pinDirectory.getValue() + L"\\" + m_fileNames[pinChoice] + L"." + pinFileExtension.getValue(); // Append the extension

		pinFileName = filenameOnly;
	}
	else
	{
		// Possibly clear the filename if the choice is out of range
		pinFileName = L"";
	}

	// Combine file names into a single string for debug purposes
	std::wstringstream debugStream;
	for (const auto& name : m_fileNames) {
		debugStream << name << L","; // Adds a comma for separation
	}

	std::wstring debugOutput = debugStream.str();
	if (!debugOutput.empty()) {
		debugOutput.erase(debugOutput.end() - 2, debugOutput.end()); // Remove last comma and space
	}

	pinDebug = debugOutput;
}

void FileDialogGui::OnFileDialogComplete(int32_t result)
{
	//pinOpened = false;

	if (result != gmpi::MP_OK)
	{
		return;
	}

	if (result == gmpi::MP_OK)
	{
		// Trim filename if in default folder.
		auto filepath = nativeFileDialog.GetSelectedFilename();
		fileext = GetExtension(filepath);
		const char* fileclass = nullptr;

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
				//filepath = shortName;
			}
		}

		pinFileName = filepath;
		pinDebug = filepath;
	}

	auto parentPath = fs::path(pinFileName).parent_path();
	pinDirectory = parentPath;
	updateItemsList(parentPath.string());	
	nativeFileDialog.setNull(); // release it.
}

void FileDialogGui::updateItemsList(const fs::path& directory)
{
	m_fileNames.clear(); // Clear previous file names

	if (fs::exists(directory) && fs::is_directory(directory))
	{
		for (const auto& entry : fs::directory_iterator(directory))
		{
			if (entry.is_regular_file())
			{
				auto file_extension = entry.path().extension().string().substr(1);

				// Now check if the extension matches the requested one
				if (file_extension == fileext) // no operator matches these operands 
				{
					// Store the full filename (with extension) in m_fileNames
					m_fileNames.push_back(entry.path().stem().wstring()); // Store only the file name without extension
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
			ss << L","; // Add comma between items
	}

	pinItemsList = ss.str();
	pinItemsListIn = pinItemsList;
	onSetSelectedFile();
}

void FileDialogGui::onSetSelectedFile()
{
	if (m_fileNames.empty())
	{
		pinChoice = -1; // Or another default value to indicate no selection
		return;
	}

	std::wstring selectedFile = fs::path(pinFileName).stem().wstring();

	auto it = std::find(m_fileNames.begin(), m_fileNames.end(), selectedFile);

	if (it != m_fileNames.end())
	{
		pinChoice = static_cast<int>(std::distance(m_fileNames.begin(), it));
	}
	else
	{
		pinChoice = -1; // File not found, set to -1 (or appropriate value)
	}
}

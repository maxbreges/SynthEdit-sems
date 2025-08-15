#include "./FileDialogGui.h"

#include "../shared/unicode_conversion.h"
#include "../shared/it_enum_list.h"
#include "../shared/string_utilities.h"
#include "../se_sdk3/MpString.h"
#include <sstream>
#include <algorithm> // for std::sort
#include <cctype>    // for tolower

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
	initializePin(pinFileNameOut);
}

void FileDialogGui::onSetItemsList()
{
	std::wstring itemsList = pinItemsListIn.getValue();
	m_fileNamesSet.clear();

	std::wistringstream wss(itemsList);
	std::wstring item;

	while (std::getline(wss, item, L',')) {
		if (!item.empty()) // Avoid empty entries
			m_fileNamesSet.insert(item);
	}
}

void FileDialogGui::onSetChoice()
{
	if (pinChoice >= 0 && pinChoice < static_cast<int>(m_fileNamesSet.size()))
	{
#ifdef _WIN32
		const wchar_t* pathSeparator = L"\\";
#else
		const wchar_t* pathSeparator = L"/";
#endif
		auto it = std::next(m_fileNamesSet.begin(), pinChoice);
		auto element = *it; // element is a std::wstring		

		std::wstring filenameOnly = pinDirectory.getValue() + pathSeparator + element + L"." + pinFileExtension.getValue();
		
		pinFileName = filenameOnly;
	}
	else
	{
		// Possibly clear the filename if the choice is out of range
		pinFileName = L"";
	}

	std::wstring selectedFileStem = fs::path(pinFileName).stem().wstring();
	pinFileNameOut = selectedFileStem;

/*	// Combine file names into a single string for debug purposes
	std::wstringstream debugStream;
	for (const auto& name : m_fileNamesSet) {
		debugStream << name << L","; // Adds a comma for separation
	}

	std::wstring debugOutput = debugStream.str();
	if (!debugOutput.empty()) {
		debugOutput.erase(debugOutput.end() - 2, debugOutput.end()); // Remove last comma and space
	}*/
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
		//std::wstring file_extension = pinFileExtension;

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

std::string fileext;

bool iequals(const std::string& a, const std::string& b)
{
	if (a.size() != b.size())
		return false;
	return std::equal(a.begin(), a.end(), b.begin(), b.end(),
		[](char a_char, char b_char) {
			return tolower(a_char) == tolower(b_char);
		});
}

void FileDialogGui::OnFileDialogComplete(int32_t result)
{
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

		if (iequals(fileext, "sf2") || iequals(fileext, "sfz"))
		{
			fileclass = "Instrument";
		}
		else if (iequals(fileext, "png") || iequals(fileext, "bmp") || iequals(fileext, "jpg"))
		{
			fileclass = "Image";
		}
		else if (iequals(fileext, "wav"))
		{
			fileclass = "Audio";
		}
		else if (iequals(fileext, "mid"))
		{
			fileclass = "MIDI";
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
		

		if (!filepath.empty())
		{
			pinOpened = 1.f;
		}
		else
		{
			pinOpened = 0.f;			
		}
	}

	auto parentPath = fs::path(pinFileName).parent_path();
	pinDirectory = parentPath;
	updateItemsList(parentPath.string());
	nativeFileDialog.setNull(); // release it.
}


void FileDialogGui::updateItemsList(const fs::path& directory)
{
	m_fileNamesSet.clear();

	if (fs::exists(directory) && fs::is_directory(directory))
	{
		for (const auto& entry : fs::directory_iterator(directory))
		{
			if (entry.is_regular_file())
			{
				auto extension = entry.path().extension().string().substr(1);
				if (iequals(extension, fileext))
				{
					// Insert into the set
					m_fileNamesSet.insert(entry.path().stem().wstring());
				}
			}
		}
	}

	// Clear previous list
	pinItemsList = "";

	std::wstringstream ss;
	size_t count = 0;
	for (const auto& filename : m_fileNamesSet)
	{
		if (count > 0)
			ss << L",";
		ss << filename;
		++count;
	}

	pinItemsList = ss.str();
	pinItemsListIn = pinItemsList;
	onSetSelectedFile();
}

void FileDialogGui::onSetSelectedFile()
{
	if (m_fileNamesSet.empty())
	{
		pinChoice = -1;
		return;
	}

	std::wstring selectedFileStem = fs::path(pinFileName).stem().wstring();
	
	auto it = m_fileNamesSet.find(selectedFileStem);
	if (it != m_fileNamesSet.end())
	{
		// Get the index of the filename in the sorted set
		auto index = std::distance(m_fileNamesSet.begin(), it);
		pinChoice = static_cast<int>(index);
	}
	else
	{
		pinChoice = -1;
	}
}

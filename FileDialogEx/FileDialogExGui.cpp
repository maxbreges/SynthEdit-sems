#include "./FileDialogExGui.h"
#include "../shared/unicode_conversion.h"
#include "../shared/it_enum_list.h"
#include "../shared/string_utilities.h"
#include "../se_sdk3/MpString.h"
#include <sstream>
#include <algorithm>    // for std::equal
#include <cctype>       // for ::tolower
#include <system_error> // for std::error_code

using namespace gmpi;
using namespace gmpi_gui;
using namespace gmpi_sdk;
using namespace JmUnicodeConversions;

GMPI_REGISTER_GUI(MP_SUB_TYPE_GUI2, FileDialogExGui, L"FileDialogEx");

// Helper function for case-insensitive string comparison.
// You might already have a similar utility in string_utilities.h
bool iequals(const std::string& a, const std::string& b)
{
	return std::equal(a.begin(), a.end(),
		b.begin(), b.end(),
		[](char char_a, char char_b) {
			return ::tolower(static_cast<unsigned char>(char_a)) ==
				::tolower(static_cast<unsigned char>(char_b));
		});
}

FileDialogExGui::FileDialogExGui() :
	m_prev_trigger(false)
{
	// initialise pins.
	initializePin(pinFileName, static_cast<MpGuiBaseMemberPtr2>(&FileDialogExGui::onSetFileName));
	initializePin(pinFileExtension);
	initializePin(pinChoice, static_cast<MpGuiBaseMemberPtr2>(&FileDialogExGui::onSetChoice));
	initializePin(pinItemsList);
	initializePin(pinDirectory);
	initializePin(pinDebug);
	initializePin(pinParentPath, static_cast<MpGuiBaseMemberPtr2>(&FileDialogExGui::onSetItemsList));
	initializePin(pinOpened);
}

void FileDialogExGui::onSetItemsList()
{
	std::wstring itemsList = pinParentPath.getValue();
	m_fileNames.clear();
	std::wistringstream wss(itemsList);
	std::wstring item;
	while (std::getline(wss, item, L',')) {
		if (!item.empty()) // Avoid empty entries
			m_fileNames.push_back(item);
	}
}

void FileDialogExGui::onSetChoice()
{
	if (pinChoice >= 0 && pinChoice < m_fileNames.size())
	{
		// FIX: Use std::filesystem for cross-platform path construction.
		fs::path directoryPath(pinDirectory.getValue());
		std::wstring filenameWithExt = m_fileNames[pinChoice] + L"." + pinFileExtension.getValue();
		fs::path fullPath = directoryPath / filenameWithExt;
		pinFileName = fullPath.wstring();
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
		// FIX: Correctly remove the trailing comma.
		debugOutput.pop_back();
	}
	//pinDebug = debugOutput;
}

void FileDialogExGui::onSetFileName()
{
	// FIX: Use fs::path consistently and be explicit with string conversions.
	fs::path filePath(pinFileName.getValue());
	auto parentPath = filePath.parent_path();
	pinDirectory = parentPath.wstring();
	updateItemsList(parentPath);
}

void FileDialogExGui::updateItemsList(const fs::path& directory)
{
	// FIX: Get desired extension from pin and convert to a std::string for comparison.
	std::string desired_ext = WStringToUtf8(pinFileExtension.getValue());
	m_fileNames.clear();

	if (fs::exists(directory) && fs::is_directory(directory))
	{
		// FIX: Use the non-throwing version of directory_iterator for safety.
		std::error_code ec;
		for (const auto& entry : fs::directory_iterator(directory, ec))
		{
			if (entry.is_regular_file())
			{
				// FIX: Get extension and handle the leading dot.
				std::string file_extension = entry.path().extension().string();
				if (!file_extension.empty() && file_extension[0] == '.')
				{
					file_extension = file_extension.substr(1);
				}

				// FIX: Perform a case-insensitive comparison for file extensions.
				if (iequals(file_extension, desired_ext))
				{
					// Store the file name stem as a wstring.
					m_fileNames.push_back(entry.path().stem().wstring());
				}
			}
		}
		if (ec)
		{
			// Optional: Log the error if directory iteration fails.
			 pinDebug = L"Error iterating directory: " + Utf8ToWstring(ec.message());
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
	pinParentPath = pinItemsList;
	onSetSelectedFile();
}

void FileDialogExGui::onSetSelectedFile()
{
	if (m_fileNames.empty())
	{
		pinChoice = -1;
		return;
	}

	std::wstring selectedFile = fs::path(pinFileName.getValue()).stem().wstring();
	auto it = std::find(m_fileNames.begin(), m_fileNames.end(), selectedFile);
	if (it != m_fileNames.end())
	{
		pinChoice = static_cast<int>(std::distance(m_fileNames.begin(), it));
	}
	else
	{
		pinChoice = -1;
	}
}
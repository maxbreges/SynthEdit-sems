#include "./FileDialogExGui.h"

#include "../shared/unicode_conversion.h"
#include "../shared/it_enum_list.h"
#include "../shared/string_utilities.h"
#include "../se_sdk3/MpString.h"
#include <sstream>
#include <cctype>       // for ::tolower

using namespace gmpi;
using namespace gmpi_gui;
using namespace gmpi_sdk;
using namespace JmUnicodeConversions;

GMPI_REGISTER_GUI(MP_SUB_TYPE_GUI2, FileDialogExGui, L"FileDialogEx");

// Helper function for case-insensitive string comparison.
// You might already have a similar utility in string_utilities.h
bool iequals(const std::string & a, const std::string & b)
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

std::string fileext;

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

	//pinDebug = L"onSetItemsList()";
	//onSetChoice();
}

void FileDialogExGui::onSetChoice()
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

	//pinDebug = debugOutput;
}

void FileDialogExGui::onSetFileName()
{
	auto parentPath = fs::path(pinFileName.getValue()).parent_path();
	pinDirectory = parentPath;
	updateItemsList(parentPath.string());
}

void FileDialogExGui::updateItemsList(const fs::path& directory)
{
	fileext = pinFileExtension;
	m_fileNames.clear(); // Clear previous file names

	if (fs::exists(directory) && fs::is_directory(directory))
	{		
		for (const auto& entry : fs::directory_iterator(directory))
		{
			if (entry.is_regular_file())
			{
				auto file_extension = entry.path().extension().string().substr(1);
				pinDebug = fileext;
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
	pinParentPath = pinItemsList;
	onSetSelectedFile();
}

void FileDialogExGui::onSetSelectedFile()
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

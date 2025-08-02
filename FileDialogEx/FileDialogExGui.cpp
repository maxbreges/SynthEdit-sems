#include "./FileDialogExGui.h"
#include <sstream>
#include <algorithm>

using namespace gmpi;
using namespace gmpi_gui;
using namespace gmpi_sdk;

GMPI_REGISTER_GUI(MP_SUB_TYPE_GUI2, FileDialogExGui, L"FileDialogEx");

FileDialogExGui::FileDialogExGui()
{
	// Initialize pins with their handlers, same pattern as DAM_GUIFileBrowser_Gui
	initializePin(pinDirectory, static_cast<MpGuiBaseMemberPtr2>(&FileDialogExGui::onSetDirectory));
	initializePin(pinFileExtension, static_cast<MpGuiBaseMemberPtr2>(&FileDialogExGui::onSetFileExtension));
	initializePin(pinHideExt); // No handler, value is read directly
	initializePin(pinRescan, static_cast<MpGuiBaseMemberPtr2>(&FileDialogExGui::onSetRescan));
	initializePin(pinParent, static_cast<MpGuiBaseMemberPtr2>(&FileDialogExGui::onSetParent));
	initializePin(pinChoice, static_cast<MpGuiBaseMemberPtr2>(&FileDialogExGui::onSetChoice));
	initializePin(pinItemsList); // Output only
	initializePin(pinFileName, static_cast<MpGuiBaseMemberPtr2>(&FileDialogExGui::onSetFileName));
	initializePin(pinNameOut);   // Output only

	SetTimerIntervalMs(10);
}

bool FileDialogExGui::OnTimer()
{
	// next user selection (even at the same list index) is a new value and triggers onSetChoice.
	pinChoice = -1;
	return false; // Run timer only once
}

void FileDialogExGui::scanCurrentPath()
{
	std::wostringstream list;
	std::wstring tmp;

	bool add_comma = false;
	const bool hide_ext = pinHideExt;
	const std::wstring current_fname = pinFileName;

	entryList.clear();
	nItems = 0;

	if (fs::exists(currentPath) && fs::is_directory(currentPath))
	{
		for (auto& entry : fs::directory_iterator(currentPath, fs::directory_options::skip_permission_denied))
		{
			if (add_comma)
				list << L",";
			add_comma = true;

			const bool is_dir = entry.is_directory();
			const bool extension_match = fileExtension.empty() || (entry.path().extension().compare(fileExtension) == 0);

			if (is_dir || (entry.is_regular_file() && extension_match))
			{
				tmp = entry.path().filename().wstring();

				if (hide_ext && !is_dir)
				{
					const size_t lastdot = tmp.find_last_of(L'.');
					if (lastdot != std::wstring::npos)
						tmp = tmp.substr(0, lastdot);
				}

				// Replace commas to avoid breaking the list format.
				std::replace(tmp.begin(), tmp.end(), L',', L'_');

				list << tmp;
				entryList.push_back(entry);

				if (is_dir)
				{
					list << L"/";
				}
				nItems++;

				// Restore selection if the current item matches pinFileName.
				if (!current_fname.empty() && entry.path().wstring() == current_fname)
				{
					pinChoice = nItems - 1;
				}
			}
			else
			{
				add_comma = false;
			}
		}
	}
	pinItemsList = list.str();
}

void FileDialogExGui::onSetFileExtension()
{
	// Add leading dot for comparison with path().extension()
	std::wstring ext = pinFileExtension;
	if (ext.empty())
	{
		fileExtension = L"";
	}
	else
	{
		fileExtension = L".";
		fileExtension.append(ext);
	}

	if (!currentPath.empty())
	{
		scanCurrentPath();
	}
}

void FileDialogExGui::onSetDirectory()
{
	currentPath = pinDirectory.getValue();

	if (!fs::exists(currentPath))
	{
		pinItemsList = L"Path not found.";
		nItems = 0;
		entryList.clear();
		return;
	}

	// If a file path is passed, use its parent directory and set the file as output.
	if (!fs::is_directory(currentPath))
	{
		pinFileName = currentPath.wstring();
		pinNameOut = currentPath.filename().wstring();
		currentPath = currentPath.parent_path();
		pinDirectory = currentPath.wstring(); // Update pin to reflect just the path.
	}

	pinChoice = -1;
	scanCurrentPath();
}

void FileDialogExGui::onSetRescan()
{
	if (pinRescan)
	{
		scanCurrentPath();
		pinRescan = false; // Reset trigger
	}
}

void FileDialogExGui::onSetParent()
{
	if (pinParent)
	{
		if (currentPath.has_parent_path() && currentPath.parent_path() != currentPath)
		{
			currentPath = currentPath.parent_path();
			pinDirectory = currentPath.wstring();
			pinChoice = -1;
			scanCurrentPath();
		}
		pinParent = false; // Reset trigger
	}
}

void FileDialogExGui::onSetChoice()
{
	int c = pinChoice;

	if (c >= 0 && c < nItems)
	{
		fs::directory_entry entry = entryList[c];

		if (entry.is_directory()) // Navigate to sub-dir
		{
			currentPath = entry.path();
			pinDirectory = currentPath.wstring();
			scanCurrentPath();
			StartTimer(); // Use timer to reset choice pin to -1, allowing re-selection.
		}
		else // A file was chosen
		{
			pinFileName = entry.path().wstring();
			pinNameOut = entry.path().filename().wstring();
		}
	}
}

void FileDialogExGui::onSetFileName()
{
	// This handler is for restoring selection state from the host.
	const std::wstring fname = pinFileName;

	if (!fname.empty() && nItems > 0)
	{
		for (int c = 0; c < nItems; ++c)
		{
			if (entryList[c].path().wstring() == fname)
			{
				pinChoice = c;
				break; // Found it.
			}
		}
	}
}

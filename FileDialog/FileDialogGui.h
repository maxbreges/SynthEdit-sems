# pragma once
#ifndef FILEDIALOGGUI_H_INCLUDED
#define FILEDIALOGGUI_H_INCLUDED

#include "mp_sdk_gui2.h"
#include "mp_gui.h"
#include <filesystem>

namespace fs = std::filesystem;

class FileDialogGui : public SeGuiInvisibleBase
{
	bool m_prev_trigger;
	std::vector<std::wstring> m_fileNames;// Store the list of files for index tracking
	std::vector<std::wstring> m_fileNamesEx;// Store the list of files for index tracking
	GmpiGui::FileDialog nativeFileDialog;

public:
	FileDialogGui();

	// overrides.

private:
	void onSetTrigger();
	void OnFileDialogComplete(int32_t result);
	void onSetPath();
	void updateItemsList(const fs::path& directory);
	void onSetSelectedFile();
	
	std::string getDefaultFolder(std::wstring extension);		

	StringGuiPin pinFileName;
	StringGuiPin pinFileExtension;
	BoolGuiPin pinTrigger;
	BoolGuiPin pinSaveMode;
	StringGuiPin pinDirectory;
	StringGuiPin pinListItems;
	IntGuiPin pinChoice;
	StringGuiPin pinSelection;
};

#endif

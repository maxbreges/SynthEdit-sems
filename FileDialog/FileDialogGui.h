#ifndef FILEDIALOGGUI_H_INCLUDED
#define FILEDIALOGGUI_H_INCLUDED

#include "../se_sdk3/mp_sdk_gui2.h"
#include "../se_sdk3/mp_gui.h"
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class FileDialogGui : public SeGuiInvisibleBase
{
	bool m_prev_trigger;
	GmpiGui::FileDialog nativeFileDialog;
	std::vector<std::wstring> m_fileNames;

public:
	FileDialogGui();

	// overrides.
	void onSetTrigger();

private:	
	void OnFileDialogComplete(int32_t result);
	std::string getDefaultFolder(std::wstring extension);
	void onSetChoice();
	void updateItemsList(const fs::path& directory); // Correctly defined in the class
	void onSetSelectedFile(); 
	void onSetItemsList();

	StringGuiPin pinFileName;
	StringGuiPin pinFileExtension;
	IntGuiPin pinChoice;
	StringGuiPin pinItemsList;
	FloatGuiPin pinTrigger;
	BoolGuiPin pinSaveMode;
	StringGuiPin pinDirectory;
	StringGuiPin pinDebug;
	StringGuiPin pinItemsListIn;
	BoolGuiPin pinOpened;
};

#endif
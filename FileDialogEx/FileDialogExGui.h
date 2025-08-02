#ifndef FILEDIALOGEXGUI_H_INCLUDED
#define FILEDIALOGEXGUI_H_INCLUDED

#include "../se_sdk3/mp_sdk_gui2.h"
#include "../se_sdk3/TimerManager.h" // For TimerClient
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class FileDialogExGui : public SeGuiInvisibleBase, public TimerClient
{
public:
	FileDialogExGui();
	virtual bool OnTimer() override;

private:
	// Pin handler methods,  
	void onSetDirectory();
	void onSetFileExtension();
	void onSetRescan();
	void onSetParent();
	void onSetChoice();
	void onSetFileName();

	// Core logic
	void scanCurrentPath();
	
	// Member variables  
	fs::path currentPath;
	std::vector<fs::directory_entry> entryList;
	std::wstring fileExtension;
	int nItems = 0;

	// Pins mirroring DAM_GUIFileBrowser_Gui's functionality
	StringGuiPin pinDirectory;      // Input: path to browse (was pinPath)
	StringGuiPin pinFileExtension;  // Input: file extension to filter (was pinExt)
	BoolGuiPin pinHideExt;        // Input: hide extension in list
	BoolGuiPin pinRescan;         // Input: trigger to rescan
	BoolGuiPin pinParent;         // Input: trigger to go to parent dir
	IntGuiPin pinChoice;          // Input/Output: selected item index
	StringGuiPin pinItemsList;      // Output: comma-separated list of items (was pinList)
	StringGuiPin pinFileName;       // Output: full path of selected file (was pinFNameOut)
	StringGuiPin pinNameOut;        // Output: filename of selected file
};

#endif

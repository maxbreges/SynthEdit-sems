#ifndef FILEDIALOGEXGUI_H_INCLUDED
#define FILEDIALOGEXGUI_H_INCLUDED

#include "../se_sdk3/mp_sdk_gui2.h"
#include "../se_sdk3/mp_gui.h"
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class FileDialogExGui : public SeGuiInvisibleBase
{
	bool m_prev_trigger;
	GmpiGui::FileDialog nativeFileDialog;
    std::vector<std::wstring> m_fileNames;

public:
	FileDialogExGui();

	// overrides.

private:

    void updateItemsList(const fs::path& directory); // Correctly defined in the class
    void onSetSelectedFile();

    void onSetFileName();
   // void onSetFileExtension();
    void onSetChoice();
    void onSetItemsList();
  //  void onSetDirectory();
   // void onSetDebug();
   // void onSetParentPath();
   // void onSetOpened();

    StringGuiPin pinFileName;
    StringGuiPin pinFileExtension;
    IntGuiPin pinChoice;
    StringGuiPin pinItemsList;

    BoolGuiPin pinSaveMode;
    StringGuiPin pinDirectory;
    StringGuiPin pinDebug;
    StringGuiPin pinParentPath;
    BoolGuiPin pinOpened;
};

#endif



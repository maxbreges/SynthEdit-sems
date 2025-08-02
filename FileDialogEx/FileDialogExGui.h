#ifndef FILEDIALOGEXGUI_H_INCLUDED
#define FILEDIALOGEXGUI_H_INCLUDED

#include "../se_sdk3/mp_sdk_gui2.h"
#include "../se_sdk3/mp_gui.h"

class FileDialogExGui : public SeGuiInvisibleBase
{
	bool m_prev_trigger;
	GmpiGui::FileDialog nativeFileDialog;

public:
	FileDialogExGui();

	// overrides.

private:
	void onSetTrigger();
	void OnFileDialogComplete(int32_t result);
	std::string getDefaultFolder(std::wstring extension);

	StringGuiPin pinFileName;
	StringGuiPin pinFileExtension;
	BoolGuiPin pinTrigger;
	BoolGuiPin pinSaveMode;
	StringGuiPin pinDirectory;
};

#endif



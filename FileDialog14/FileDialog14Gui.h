#pragma once
#ifndef FILEDIALOG14GUI_H_INCLUDED
#define FILEDIALOG14GUI_H_INCLUDED

#include "../se_sdk3/mp_gui.h"

class FileDialogGui : public SeGuiInvisibleBase
{
	bool m_prev_trigger;
	GmpiGui::FileDialog nativeFileDialog;

public:
	FileDialogGui();

	// overrides.

private:
	void onSetTrigger();
	void OnFileDialogComplete(int32_t result);
	std::string getDefaultFolder(std::wstring extension);

	StringGuiPin pinFileName;
	StringGuiPin pinFileExtension;
	BoolGuiPin pinTrigger;
	BoolGuiPin pinSaveMode;
	BoolGuiPin pinLed;
};

#endif



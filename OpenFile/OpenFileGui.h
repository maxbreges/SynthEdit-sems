#pragma once
#ifndef OPENFILEGUI_H_INCLUDED
#define OPENFILEGUI_H_INCLUDED

#include "../se_sdk3/mp_sdk_gui2.h"
#include "../se_sdk3/mp_gui.h"

class OpenFileGui : public SeGuiInvisibleBase
{
	bool m_prev_trigger;
	GmpiGui::FileDialog nativeFileDialog;

public:
	OpenFileGui();

	// overrides.

private:
	void onSetTrigger();
	void OnFileDialogComplete(int32_t result);
	std::string getDefaultFolder(std::wstring extension);

	StringGuiPin pinFileName;
	StringGuiPin pinFileExtension;
	BoolGuiPin pinTrigger;
	BoolGuiPin pinSaveMode;
	BoolGuiPin pinOnDialogComplete;
};

#endif



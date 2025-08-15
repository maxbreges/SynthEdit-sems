#ifndef FILEDIALOGGUI_H_INCLUDED
#define FILEDIALOGGUI_H_INCLUDED

#include "../se_sdk3/mp_sdk_gui2.h"
#include "../se_sdk3/mp_gui.h"
#include <vector>
#include <string>
#include <filesystem>
#include <set>

namespace fs = std::filesystem;

struct ci_wstring_compare
{
	bool operator()(const std::wstring& a, const std::wstring& b) const
	{
		auto itA = a.begin();
		auto itB = b.begin();

		while (itA != a.end() && itB != b.end())
		{
			wchar_t chA = towlower(*itA);
			wchar_t chB = towlower(*itB);
			if (chA < chB)
				return true;
			else if (chA > chB)
				return false;
			++itA;
			++itB;
		}
		// If all characters so far are equal, the shorter string is "less"
		return a.size() < b.size();
	}
};

class FileDialogGui : public SeGuiInvisibleBase
{
	bool m_prev_trigger;
	GmpiGui::FileDialog nativeFileDialog;
	//std::vector<std::wstring> m_fileNames;
	std::set<std::wstring, ci_wstring_compare> m_fileNamesSet;

public:
	FileDialogGui();

	// overrides.

//private:
	void onSetTrigger();
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
	BoolGuiPin pinTrigger;
	BoolGuiPin pinSaveMode;
	StringGuiPin pinDirectory;
	StringGuiPin pinDebug;
	StringGuiPin pinItemsListIn;
	FloatGuiPin pinOpened;
	StringGuiPin pinFileNameOut;
};

#endif
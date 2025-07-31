#include "./FileDialogExGui.h"

#include "../shared/unicode_conversion.h"
#include "../shared/it_enum_list.h"
#include "../shared/string_utilities.h"
#include "../se_sdk3/MpString.h"
#include <sstream>

using namespace gmpi;
using namespace gmpi_gui;
using namespace gmpi_sdk;
using namespace JmUnicodeConversions;

GMPI_REGISTER_GUI(MP_SUB_TYPE_GUI2, FileDialogExGui, L"FileDialogEx");

FileDialogExGui::FileDialogExGui() :
	m_prev_trigger(false)
{
	// initialise pins.
	initializePin(pinFileName, static_cast<MpGuiBaseMemberPtr2>(&FileDialogExGui::onSetFileName));
	initializePin(pinFileExtension, static_cast<MpGuiBaseMemberPtr2>(&FileDialogExGui::onSetFileExtension));
	initializePin(pinChoice, static_cast<MpGuiBaseMemberPtr2>(&FileDialogExGui::onSetChoice));
	initializePin(pinItemsList, static_cast<MpGuiBaseMemberPtr2>(&FileDialogExGui::onSetItemsList));
	initializePin(pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&FileDialogExGui::onSetTrigger));
	initializePin(pinSaveMode, static_cast<MpGuiBaseMemberPtr2>(&FileDialogExGui::onSetSaveMode));
	initializePin(pinDirectory, static_cast<MpGuiBaseMemberPtr2>(&FileDialogExGui::onSetDirectory));
	initializePin(pinDebug, static_cast<MpGuiBaseMemberPtr2>(&FileDialogExGui::onSetDebug));
	initializePin(pinParentPath, static_cast<MpGuiBaseMemberPtr2>(&FileDialogExGui::onSetParentPath));
	initializePin(pinOpened, static_cast<MpGuiBaseMemberPtr2>(&FileDialogExGui::onSetOpened));
}

std::string FileDialogExGui::getDefaultFolder(std::wstring extension)
{
	const std::wstring searchFilename = L"dummy." + extension;
	const auto fullFileName = uiHost.resolveFilename(searchFilename.c_str());
	return JmUnicodeConversions::WStringToUtf8(fullFileName.substr(0, fullFileName.find(L"dummy") - 1));
}

void FileDialogExGui::onSetTrigger()
{
	// trigger on mouse-up
	if (pinTrigger == false && m_prev_trigger == true) // dialog triggered on mouse-up (else dialog grabs focus, button never resets)
	{
		std::ostringstream oss;
		oss << "onSetTrigger called: pinTrigger=" << pinTrigger << ", m_prev_trigger=" << m_prev_trigger << "\n";
		pinDebug = oss.str();

		std::wstring filename = pinFileName;
		std::wstring file_extension = pinFileExtension;

		IMpGraphicsHostBase* dialogHost = 0;
		getHost()->queryInterface(SE_IID_GRAPHICS_HOST_BASE, reinterpret_cast<void**>(&dialogHost));

		if (dialogHost != 0)
		{
			pinOpened = true;
			int dialogMode = (int)pinSaveMode;
			dialogHost->createFileDialog(dialogMode, nativeFileDialog.GetAddressOf());

			if (!nativeFileDialog.isNull())
			{
				nativeFileDialog.AddExtensionList(pinFileExtension);

				auto filename = pinFileName.getValue();
				if (!filename.empty())
				{
					filename = uiHost.resolveFilename(filename);
					nativeFileDialog.SetInitialFullPath(JmUnicodeConversions::WStringToUtf8(filename));
				}
				else
				{
					nativeFileDialog.setInitialDirectory(getDefaultFolder(pinFileExtension));
				}

				nativeFileDialog.ShowAsync([this](int32_t result) -> void { this->OnFileDialogComplete(result); });
			}
		}
	}

	m_prev_trigger = pinTrigger;
}

std::string uniformPath(std::string path)
{
	std::string ret;

	auto folderPath = path;

	while (!folderPath.empty())
	{
		auto p = folderPath.find_last_of("\\/");

		if (!ret.empty())
			ret = '/' + ret;

		if (p == std::string::npos)
		{
			ret = folderPath + ret;
			folderPath.clear();
		}
		else
		{
			ret = std::string(folderPath.c_str() + p + 1) + ret;
			folderPath = Left(folderPath, p);
		}
	}

	return ret;
}

void FileDialogExGui::OnFileDialogComplete(int32_t result)
{
	pinOpened = false;

	if (result != gmpi::MP_OK)
	{
		return;
	}

	if (result == gmpi::MP_OK)
	{
		// Trim filename if in default folder.
		auto filepath = nativeFileDialog.GetSelectedFilename();
		auto fileext = GetExtension(filepath);
		const char* fileclass = nullptr;

		if (fileext == "sf2" || fileext == "sfz")
		{
			fileclass = "Instrument";
		}
		else
		{
			if (fileext == "png" || fileext == "bmp" || fileext == "jpg")
			{
				fileclass = "Image";
			}
			else
			{
				if (fileext == "wav")
				{
					fileclass = "Audio";
				}
				else
				{
					if (fileext == "mid")
					{
						fileclass = "MIDI";
					}
				}
			}
		}

		if (fileclass)
		{
#if 0
			// try to find a shorter filename that SynthEdit can find.
			std::filesystem::path fullPath(filepath);

			std::vector<std::filesystem::path> pathParts;
			for (auto p : fullPath)
			{
				pathParts.push_back(p);
			}

			std::reverse(pathParts.begin(), pathParts.end());

			std::filesystem::path shortName;
			for (auto p : pathParts)
			{
				shortName = shortName.empty() ? p : p / shortName;

				const std::filesystem::path r = uiHost.FindResourceU(shortName.string().c_str(), fileclass);
				if (filepath == r)
				{
					filepath = shortName.string();
					break;
				}
			}
#else
			auto folderPath = filepath;
			std::vector<std::string> pathParts;

			while (!folderPath.empty())
			{
				auto p = folderPath.find_last_of("\\/");

				if (p == std::string::npos)
				{
					pathParts.push_back(folderPath);
					folderPath.clear();
				}
				else
				{
					pathParts.push_back(std::string(folderPath.c_str() + p + 1));
					folderPath = Left(folderPath, p);
				}
			}

			// If root folder is a network location, ignore that
			if (!pathParts.empty() && pathParts.back().find("\\\\") == 0)
			{
				pathParts.pop_back();
			}

			std::string shortName;
			for (auto p : pathParts)
			{
				shortName = shortName.empty() ? p : p + '/' + shortName;

				const std::string r = uiHost.FindResourceU(shortName.c_str(), fileclass);
				if (uniformPath(filepath) == uniformPath(r))
				{
					filepath = shortName;
					break;
				}
			}
#endif
		}

		pinFileName = filepath;
	}

	nativeFileDialog.setNull(); // release it.
}


 	void FileDialogExGui::onSetFileName()
	{
		// pinFileName changed
	}

 	void FileDialogExGui::onSetFileExtension()
	{
		// pinFileExtension changed
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

	pinDebug = debugOutput;
	}

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

		pinDebug = L"onSetItemsList()";
		//onSetChoice();
	}

 	void FileDialogExGui::onSetSaveMode()
	{
		// pinSaveMode changed
	}

 	void FileDialogExGui::onSetDirectory()
	{
		// pinDirectory changed
	}

 	void FileDialogExGui::onSetDebug()
	{
		// pinDebug changed
	}

 	void FileDialogExGui::onSetParentPath()
	{
		// pinParentPath changed
	}

 	void FileDialogExGui::onSetOpened()
	{
		// pinOpened changed
	}




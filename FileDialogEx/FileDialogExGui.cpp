#include "./FileDialogExGui.h"

#include "../shared/unicode_conversion.h"
#include "../shared/it_enum_list.h"
#include "../shared/string_utilities.h"
#include "../se_sdk3/MpString.h"

using namespace gmpi;
using namespace gmpi_gui;
using namespace gmpi_sdk;
using namespace JmUnicodeConversions;

GMPI_REGISTER_GUI(MP_SUB_TYPE_GUI2, FileDialogExGui, L"FileDialogEx");

// Helper function to extract directory path from a full file path
std::string getDirectoryFromPath(const std::string& filepath)
{
	size_t p = filepath.find_last_of("/\\");
	if (p != std::string::npos)
		return filepath.substr(0, p);
	else
		return ""; // No directory part found
}

FileDialogExGui::FileDialogExGui() :
	m_prev_trigger(false)
{
	// initialise pins.
	initializePin(pinFileName);
	initializePin(pinFileExtension);
	initializePin(pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&FileDialogExGui::onSetTrigger));
	initializePin(pinSaveMode);
	initializePin(pinDirectory);
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
		std::wstring filename = pinFileName;
		std::wstring file_extension = pinFileExtension;

		IMpGraphicsHostBase* dialogHost = 0;
		getHost()->queryInterface(SE_IID_GRAPHICS_HOST_BASE, reinterpret_cast<void**>(&dialogHost));

		if (dialogHost != 0)
		{
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
		// Replace filesystem path extraction with manual string manipulation
		std::string directoryPath = getDirectoryFromPath(pinFileName);
		pinDirectory = directoryPath; // Assign the extracted directory path
	}

	nativeFileDialog.setNull(); // release it.
}

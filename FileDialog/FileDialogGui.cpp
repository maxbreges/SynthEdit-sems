#include "./FileDialogGui.h"

#include "../shared/unicode_conversion.h"
#include "../shared/it_enum_list.h"
#include "../shared/string_utilities.h"
#include "../se_sdk3/MpString.h"

using namespace gmpi;
using namespace gmpi_gui;
using namespace gmpi_sdk;
using namespace JmUnicodeConversions;

GMPI_REGISTER_GUI(MP_SUB_TYPE_GUI2, FileDialogGui, L"FileDialog");

FileDialogGui::FileDialogGui() :
	m_prev_trigger(false)
{
	// initialise pins.
	initializePin(pinFileName);
	initializePin(pinFileExtension);
	initializePin(pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&FileDialogGui::onSetTrigger));
	initializePin(pinSaveMode);
	initializePin(pinOnDialogComplete);
}

std::string FileDialogGui::getDefaultFolder(std::wstring extension)
{
	const std::wstring searchFilename = L"dummy." + extension;
	const auto fullFileName = uiHost.resolveFilename(searchFilename.c_str());
	return JmUnicodeConversions::WStringToUtf8(fullFileName.substr(0, fullFileName.find(L"dummy") - 1));
}

void FileDialogGui::onSetTrigger()
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

void FileDialogGui::OnFileDialogComplete(int32_t result)
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

		pinOnDialogComplete = true;
		pinFileName = filepath;
	}
	pinOnDialogComplete = false;
	nativeFileDialog.setNull(); // release it.
}

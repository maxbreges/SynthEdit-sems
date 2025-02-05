#include "mp_sdk_gui2.h"
#include "mp_gui.h"

using namespace gmpi;
using namespace gmpi_gui;

class JustFileGui final : public SeGuiInvisibleBase
{
	bool m_prev_trigger = false;
	GmpiGui::FileDialog nativeFileDialog;

 	void onSetFileName()
	{
	}

 	void onSetFileExtension()
	{
		// pinFileExtension changed
	}

	void onSetTrigger()
	{
		// Check if the trigger has just been set (mouse button released)
		// 'pinTrigger' represents the current state of the trigger (pressed or not)
		// 'm_prev_trigger' holds the previous state of the trigger
		if (pinTrigger == false && m_prev_trigger == true) // dialog triggered on mouse-up (else dialog grabs focus, button never resets)
		{
			std::wstring filename = pinFileName;  // Store the current filename from the input pin
			std::wstring file_extension = pinFileExtension;  // Store the current file extension from the input pin

			IMpGraphicsHostBase* dialogHost = 0;  // Initialize a pointer for the dialog host interface
			getHost()->queryInterface(SE_IID_GRAPHICS_HOST_BASE, reinterpret_cast<void**>(&dialogHost)); // Query the host for the graphics interface

			// Check if we successfully obtained the dialog host interface
			if (dialogHost != 0)
			{
				int dialogMode = (int)pinSaveMode; // Get the dialog mode (whether to save or open)
				dialogHost->createFileDialog(dialogMode, nativeFileDialog.GetAddressOf()); // Create the file dialog

				// Check if the dialog was created successfully
				if (!nativeFileDialog.isNull())
				{
					nativeFileDialog.AddExtensionList(pinFileExtension); // Add the file extension filter to the dialog

					auto filename = pinFileName.getValue(); // Retrieve the filename value from the 'pinFileName' pin
					if (!filename.empty())  // Check if the filename is not empty
					{
						filename = uiHost.resolveFilename(filename); // Resolve the file name to an absolute path
						nativeFileDialog.SetInitialFullPath(JmUnicodeConversions::WStringToUtf8(filename)); // Set the resolved file name as the initial path in the dialog
					}
					else
					{
						// If the filename is empty, set the initial directory of the dialog to a default folder based on the file extension
						nativeFileDialog.setInitialDirectory(getDefaultFolder(pinFileExtension));
					}

					// Show the dialog asynchronously and pass the callback function to handle the result
					nativeFileDialog.ShowAsync([this](int32_t result) -> void { this->OnFileDialogComplete(result); });
				}
			}
		}

		// Update the previous trigger state for the next event
		m_prev_trigger = pinTrigger;
	}

 	void onSetSaveMode()
	{
		// pinSaveMode changed
	}

 	StringGuiPin pinFileName;
 	StringGuiPin pinFileExtension;
 	BoolGuiPin pinTrigger;
 	BoolGuiPin pinSaveMode;

public:
	JustFileGui()
	{
		initializePin( pinFileName, static_cast<MpGuiBaseMemberPtr2>(&JustFileGui::onSetFileName) );
		initializePin( pinFileExtension, static_cast<MpGuiBaseMemberPtr2>(&JustFileGui::onSetFileExtension) );
		initializePin( pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&JustFileGui::onSetTrigger) );
		initializePin( pinSaveMode, static_cast<MpGuiBaseMemberPtr2>(&JustFileGui::onSetSaveMode) );
	}

	std::string getDefaultFolder(std::wstring extension)
	{
		const std::wstring searchFilename = L"dummy." + extension;
		const auto fullFileName = uiHost.resolveFilename(searchFilename.c_str());
		return JmUnicodeConversions::WStringToUtf8(fullFileName.substr(0, fullFileName.find(L"dummy") - 1));
	}

	int32_t OnFileDialogComplete(int32_t result)
	{
		if (result == gmpi::MP_OK)
		{
			// Trim filename if in default folder.
			auto filepath = nativeFileDialog.GetSelectedFilename();
			auto fileext = GetExtension(filepath);
			const char* fileclass = nullptr;

			// File classification based on extension
			if (fileext == "sf2" || fileext == "sfz") {
				fileclass = "Instrument";
			}
			else if (fileext == "png" || fileext == "bmp" || fileext == "jpg") {
				fileclass = "Image";
			}
			else if (fileext == "wav") {
				fileclass = "Audio";
			}
			else if (fileext == "mid") {
				fileclass = "MIDI";
			}

			if (fileclass) {
				auto shortName = StripPath(filepath);
				const auto r = uiHost.FindResourceU(shortName.c_str(), fileclass);

				// If resource is found, use the short name
				if (filepath == r) {
					filepath = shortName;
				}
			}
		}

		nativeFileDialog.setNull(); // release it.

		return 0;
	}
};

namespace
{
	auto r = Register<JustFileGui>::withId(L"JustFile");
}

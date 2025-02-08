#include "mp_sdk_gui2.h"
#include "mp_gui.h"
#include <string>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace gmpi;
using namespace gmpi_gui;

class JustPathGui final : public SeGuiInvisibleBase
{
	bool m_prev_trigger = false;
	GmpiGui::FileDialog nativeFileDialog;
	std::string fileNameString;

 	void onSetDirectory()
	{
		// pinDirectory changed
	}

	void onSetTrigger()
	{
		// Check if the trigger has just been set (mouse button released)
		// 'pinTrigger' represents the current state of the trigger (pressed or not)
		// 'm_prev_trigger' holds the previous state of the trigger
		if (pinTrigger == false && m_prev_trigger == true) // dialog triggered on mouse-up (else dialog grabs focus, button never resets)
		{
			IMpGraphicsHostBase* dialogHost = 0;  // Initialize a pointer for the dialog host interface
			getHost()->queryInterface(SE_IID_GRAPHICS_HOST_BASE, reinterpret_cast<void**>(&dialogHost)); // Query the host for the graphics interface

			// Check if we successfully obtained the dialog host interface
			if (dialogHost != 0)
			{
				dialogHost->createFileDialog(0, nativeFileDialog.GetAddressOf()); // Create the file dialog

				// Check if the dialog was created successfully
				if (!nativeFileDialog.isNull())
				{
					// Show the dialog asynchronously and pass the callback function to handle the result
					nativeFileDialog.ShowAsync([this](int32_t result) -> void { this->OnFileDialogComplete(result); });
				}
			}
		}
		// Update the previous trigger state for the next event
		m_prev_trigger = pinTrigger;
	}

	BoolGuiPin pinTrigger;
 	StringGuiPin pinDirectory; 	

public:
	JustPathGui()
	{
		initializePin(pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&JustPathGui::onSetTrigger));
		initializePin( pinDirectory, static_cast<MpGuiBaseMemberPtr2>(&JustPathGui::onSetDirectory) );
	}

	int32_t OnFileDialogComplete(int32_t result)
	{
		if (result == gmpi::MP_OK)
		{
			fileNameString = nativeFileDialog.GetSelectedFilename(); // Assume this returns std::string

			Directory();
		}		

		nativeFileDialog.setNull(); // Release it.
		
		return 0;
	}
		
	int32_t Directory()
	{
		// Extracting directory path
		std::stringstream Directory;
		Directory << fs::path(fileNameString).parent_path().string() + "\\";
		std::string directory(Directory.str());
		pinDirectory = directory;
		return 0;
	}
};

namespace
{
	auto r = Register<JustPathGui>::withId(L"JustPath");
}

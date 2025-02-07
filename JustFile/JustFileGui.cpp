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

 	StringGuiPin pinFileName;
 	BoolGuiPin pinTrigger;

public:
	JustFileGui()
	{
		initializePin( pinFileName, static_cast<MpGuiBaseMemberPtr2>(&JustFileGui::onSetFileName) );
		initializePin( pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&JustFileGui::onSetTrigger) );
	}

	int32_t OnFileDialogComplete(int32_t result)
	{
		if (result == gmpi::MP_OK)
		{
			// Trim filename if in default folder.
			auto filepath = nativeFileDialog.GetSelectedFilename();	
			pinFileName = filepath;
		}

		nativeFileDialog.setNull(); // release it.

		return 0;
	}
};

namespace
{
	auto r = Register<JustFileGui>::withId(L"JustFile");
}

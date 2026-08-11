#include "mp_gui.h"

using namespace gmpi;
using namespace gmpi_gui;

GmpiGui::FileDialog nativeFileDialog;

class OpenFileGui final : public SeGuiInvisibleBase
{
	bool m_prev_trigger = false;

 	void onSetTrigger()
	{
		OnBrowseButton();
	}

	BoolGuiPin pinTrigger;
 	StringGuiPin pinFilePath;
	BoolGuiPin pinLed;	

public:
	OpenFileGui()
	{
		initializePin(pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&OpenFileGui::onSetTrigger));
		initializePin( pinFilePath );
		initializePin(pinLed);
	}

	void OnBrowseButton()
	{		
		if (!pinTrigger && m_prev_trigger == true)
		{
			pinLed = true;

			IMpGraphicsHost* dialogHost = 0;
			getHost()->queryInterface(SE_IID_GRAPHICS_HOST, reinterpret_cast<void**>(&dialogHost));
			
			if (dialogHost != 0)
			{
				dialogHost->createFileDialog(0, nativeFileDialog.GetAddressOf());
				
				// caclulate initial directory from file extension, or use default.
				{
					auto filename = pinFilePath.getValue();

					if (!filename.empty())
					{
						filename = uiHost.resolveFilename(filename);
						nativeFileDialog.SetInitialFullPath(JmUnicodeConversions::WStringToUtf8(filename));
					}
				}
			}
			nativeFileDialog.ShowAsync([this](int32_t result) -> void { this->OnPopupmenuComplete(result); });
		}
		m_prev_trigger = pinTrigger;
	}

	void OnPopupmenuComplete(int32_t result)
	{
		if (result == gmpi::MP_OK)
		{
			pinFilePath = nativeFileDialog.GetSelectedFilename();//full path
		}

		nativeFileDialog.setNull(); // release it.

		pinLed = false;
	}
};

namespace
{
	auto r = Register<OpenFileGui>::withId(L"My OpenFile");
}
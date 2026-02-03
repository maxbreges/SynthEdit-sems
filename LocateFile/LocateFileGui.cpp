#include "mp_sdk_gui2.h"
#include "mp_gui.h"

using namespace gmpi;
using namespace gmpi_gui;

class LocateFileGui final : public SeGuiInvisibleBase
{
	bool m_prev_trigger = false;
	GmpiGui::FileDialog nativeFileDialog;

 	void onSetTrigger()
	{
		// trigger on mouse-up
		if (pinTrigger == false && m_prev_trigger == true) // dialog triggered on mouse-up (else dialog grabs focus, button never resets)
		{
			IMpGraphicsHostBase* dialogHost = 0;
			getHost()->queryInterface(SE_IID_GRAPHICS_HOST_BASE, reinterpret_cast<void**>(&dialogHost));

			if (dialogHost != 0)
			{				
				dialogHost->createFileDialog(0, nativeFileDialog.GetAddressOf());

				if (!nativeFileDialog.isNull())
				{
					auto filename = pinFilePath.getValue();
					if (!filename.empty())
					{
						filename = uiHost.resolveFilename(filename);
						nativeFileDialog.SetInitialFullPath(JmUnicodeConversions::WStringToUtf8(filename));
					}
					else
					{
						nativeFileDialog.setInitialDirectory(getDefaultFolder(pinExtension));
					}

					nativeFileDialog.ShowAsync([this](int32_t result) -> void { this->OnFileDialogComplete(result); });
				}
			}
		}

		m_prev_trigger = pinTrigger;
	}

 	BoolGuiPin pinTrigger;
	StringGuiPin pinFilePath;
	StringGuiPin pinExtension;
 	BoolGuiPin pinDialogComplete;

public:
	LocateFileGui()
	{
		initializePin( pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&LocateFileGui::onSetTrigger) );
		initializePin( pinFilePath );
		initializePin(pinExtension);
		initializePin( pinDialogComplete );
	}
private:
	
	void OnFileDialogComplete(int32_t result)
	{
		if (result == gmpi::MP_OK)
		{
			pinFilePath = nativeFileDialog.GetSelectedFilename();
			pinDialogComplete = true;
		}
		nativeFileDialog.setNull(); // release it.	
		pinDialogComplete = false;
	}
	
	std::string getDefaultFolder(std::wstring extension)
	{
		const std::wstring searchFilename = L"dummy." + extension;
		const auto fullFileName = uiHost.resolveFilename(searchFilename.c_str());
		return JmUnicodeConversions::WStringToUtf8(fullFileName.substr(0, fullFileName.find(L"dummy") - 1));
	}
};

namespace
{
	auto r = Register<LocateFileGui>::withId(L"LocateFile");
}

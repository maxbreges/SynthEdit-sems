#include "../se_sdk3/mp_gui.h"

using namespace gmpi;
using namespace gmpi_gui;

GmpiGui::FileDialog nativeFileDialog;

class OpenFileGui final : public SeGuiInvisibleBase
{

 	void onSetFilePath()
	{		
	}

 	void onSetTrigger()
	{
		OnBrowseButton(0.f);
	}

 	StringGuiPin pinFilePath;
 	FloatGuiPin pinTrigger;
	BoolGuiPin pinLed;

public:
	OpenFileGui()
	{
		initializePin( pinFilePath, static_cast<MpGuiBaseMemberPtr2>(&OpenFileGui::onSetFilePath) );
		initializePin( pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&OpenFileGui::onSetTrigger) );
		initializePin(pinLed);
	}

	void OnBrowseButton(float newvalue)
	{		
		if (pinTrigger > 0 && newvalue == 0)
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
					else
					{
					}
				}
			}
			nativeFileDialog.ShowAsync([this](int32_t result) -> void { this->OnPopupmenuComplete(result); });
		}
	}

	void OnPopupmenuComplete(int32_t result)
	{
		if (result == gmpi::MP_OK)
		{
			// strip off path (or part of), if it's the default path
			std::string returnValue = nativeFileDialog.GetSelectedFilename();
			pinFilePath = returnValue;
		}

		nativeFileDialog.setNull(); // release it.
		
		pinTrigger = 0;
		pinLed = false;
	}
};

namespace
{
	auto r = Register<OpenFileGui>::withId(L"My OpenFile");
}

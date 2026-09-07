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

	// Helper: Get extension 
	std::wstring getExtension(const std::wstring& ext)
	{
		size_t dotPos = ext.find_last_of('.');
		if (dotPos != std::wstring::npos)
			return ext.substr(dotPos+1);
		return ext;
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
	std::wstring filename;
	void OnBrowseButton()
	{		
		if (!pinTrigger && m_prev_trigger == true)
		{
			pinLed = true;
			filename = pinFilePath;
			std::wstring file_extension = getExtension(pinFilePath);

			IMpGraphicsHost* dialogHost = 0;
			getHost()->queryInterface(SE_IID_GRAPHICS_HOST, reinterpret_cast<void**>(&dialogHost));
			
			if (dialogHost != 0)
			{
				dialogHost->createFileDialog(0, nativeFileDialog.GetAddressOf());

				if (!nativeFileDialog.isNull())
				{
#ifdef __APPLE__
#else
					nativeFileDialog.AddExtensionList(file_extension);
#endif
					// caclulate initial directory from file extension, or use default.		
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
#ifdef __APPLE__
		if (result == gmpi::MP_OK)
		{
			pinFilePath = nativeFileDialog.GetSelectedFilename();//full path
		}
		else if
		(result == gmpi::MP_CANCEL)
		{
			pinFilePath = filename;//full path
		}
#else
		if (result == gmpi::MP_OK)
		{
			pinFilePath = nativeFileDialog.GetSelectedFilename();//full path
		}
#endif

		nativeFileDialog.setNull(); // release it.
		pinLed = false;
	}
};

namespace
{
	auto r = Register<OpenFileGui>::withId(L"My OpenFile");
}
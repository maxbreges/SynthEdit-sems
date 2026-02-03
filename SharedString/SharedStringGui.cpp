#include "mp_sdk_gui2.h"
#include "mp_gui.h"
#include <fstream>
#include <sstream>

using namespace gmpi;
using namespace gmpi_gui;

class SharedStringGui final : public SeGuiInvisibleBase
{
	bool onDialogComplete = false;

	bool m_prev_trigger = false;
	GmpiGui::FileDialog nativeFileDialog;

	std::wstring UserPath; //for File Dialog (replacing pinFileName)
	std::string fileContent; //user custom path
	
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
					UserPath = uiHost.resolveFilename(UserPath);

					nativeFileDialog.ShowAsync([this](int32_t result) -> void { this->OnFileDialogComplete(result); });
				}
			}
		}
		m_prev_trigger = pinTrigger;
	}
	
	void OnFileDialogComplete(int32_t result)
	{
		if (result == gmpi::MP_OK)
		{
			fileContent = nativeFileDialog.GetSelectedFilename();
		}
		onDialogComplete = true;
		onSetAppDirPath();
		nativeFileDialog.setNull(); // release it.
	}

	void onSetAppDirPath()
	{
		if (onDialogComplete)
		{
			std::ofstream file(pinAppDirPath.getValue());
			if (file)
			{
				file << fileContent;				
			}
			onDialogComplete = false;			
		}
		readFile();
	}

	void readFile()
	{
			std::string filename = pinAppDirPath;

			// Open the file
			std::ifstream file(filename);
			if (file)
			{
				// Read entire file into a wstring
				std::stringstream buffer;
				buffer << file.rdbuf();

				// Output the file content to pinTextOut
				pinTextOut = buffer.str();
			}
			else
			{
				// If file can't be opened, output error message
				pinTextOut = "Locate the content file";
			}	
	}

 	StringGuiPin pinAppDirPath;
	BoolGuiPin pinTrigger;
 	StringGuiPin pinTextOut;

public:
	SharedStringGui()
	{
		initializePin( pinAppDirPath, static_cast<MpGuiBaseMemberPtr2>(&SharedStringGui::onSetAppDirPath) );
		initializePin(pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&SharedStringGui::onSetTrigger));
		initializePin(pinTextOut);
	}
};

namespace
{
	auto r = Register<SharedStringGui>::withId(L"sharedString");
}

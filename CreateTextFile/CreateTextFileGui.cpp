#include "mp_sdk_gui2.h"
#include <fstream>

using namespace gmpi;

class CreateTextFileGui final : public SeGuiInvisibleBase
{
	// Create and open a text file for writing
	std::wstring fullFilename;

	void onSetFileName()
	{
		fullFilename = pinFilePath;
	}

 	void onSetTextIn()
	{		
	}

 	void onSetCreate()
	{
		std::wofstream file(fullFilename);
		if (file)
		{
			file << pinTextIn.getValue();
			pinSetFlag = true;
		}		
	}

 	void onSetResetFlag()
	{
		pinSetFlag = false;
	}

 	StringGuiPin pinFilePath;
 	StringGuiPin pinTextIn;
 	BoolGuiPin pinCreate;
 	BoolGuiPin pinSetFlag;
 	BoolGuiPin pinResetFlag;

public:
	CreateTextFileGui()
	{
		initializePin( pinFilePath, static_cast<MpGuiBaseMemberPtr2>(&CreateTextFileGui::onSetFileName) );
		initializePin( pinTextIn, static_cast<MpGuiBaseMemberPtr2>(&CreateTextFileGui::onSetTextIn) );
		initializePin( pinCreate, static_cast<MpGuiBaseMemberPtr2>(&CreateTextFileGui::onSetCreate) );
		initializePin( pinSetFlag );
		initializePin( pinResetFlag, static_cast<MpGuiBaseMemberPtr2>(&CreateTextFileGui::onSetResetFlag) );
	}
};

namespace
{
	auto r = Register<CreateTextFileGui>::withId(L"CreateTextFile");
}

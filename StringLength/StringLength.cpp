#include "mp_sdk_gui2.h"

using namespace std;
using namespace gmpi;

class StringLength final : public SeGuiInvisibleBase
{
	std::string String;
	int pinLengthBothOuts;

	void onString()
	{
		std::string str = { String };
		int count = str.size();
		pinLengthBothOuts = count;
		onSetBothOuts();
	}

 	void onSetString()
	{
		String = pinString;
		onString();
	}

	void onSetStringIn()
	{
		String = pinStringIn;
		onString();
	}
	void onSetLength()
	{		
	}

	void onSetLengthLeftOut()
	{		
	}

	void onSetBothOuts()
	{
		pinLength = pinLengthBothOuts;
		pinLengthLeftOut = pinLengthBothOuts;
	}

	//StringGuiPin String;
 	StringGuiPin pinString;
	StringGuiPin pinStringIn;
 	IntGuiPin pinLength;
	IntGuiPin pinLengthLeftOut;	

public:

	StringLength()
	{
		//initializePin(String, static_cast<MpGuiBaseMemberPtr2>(&StringLength::onString));
		initializePin( pinString, static_cast<MpGuiBaseMemberPtr2>(&StringLength::onSetString) );
		initializePin(pinStringIn, static_cast<MpGuiBaseMemberPtr2>(&StringLength::onSetStringIn));
		initializePin( pinLength, static_cast<MpGuiBaseMemberPtr2>(&StringLength::onSetBothOuts) );
		initializePin(pinLengthLeftOut, static_cast<MpGuiBaseMemberPtr2>(&StringLength::onSetBothOuts));
	}		
};

namespace
{
	auto r = Register<StringLength>::withId(L"StringLength");
}

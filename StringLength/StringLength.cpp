#include "mp_sdk_gui2.h"

using namespace std;
using namespace gmpi;

std::string String;

class StringLength final : public SeGuiInvisibleBase
{
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

 	StringGuiPin pinString;
	StringGuiPin pinStringIn;
 	IntGuiPin pinLength;
	IntGuiPin pinLengthLeftOut;
	IntGuiPin pinLengthBothOuts;

public:

	StringLength()
	{
		initializePin( pinString, static_cast<MpGuiBaseMemberPtr2>(&StringLength::onSetString) );
		initializePin(pinStringIn, static_cast<MpGuiBaseMemberPtr2>(&StringLength::onSetStringIn));
		initializePin( pinLength, static_cast<MpGuiBaseMemberPtr2>(&StringLength::onSetBothOuts) );
		initializePin(pinLengthLeftOut, static_cast<MpGuiBaseMemberPtr2>(&StringLength::onSetBothOuts));
		initializePin(pinLengthBothOuts, static_cast<MpGuiBaseMemberPtr2>(&StringLength::onSetBothOuts));
	}		
};

namespace
{
	auto r = Register<StringLength>::withId(L"StringLength");
}
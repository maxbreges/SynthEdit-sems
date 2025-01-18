#include "mp_sdk_gui2.h"

using namespace gmpi;

class StringArrayGui final : public SeGuiInvisibleBase
{	
	int arr[' '] = {};
	int i = 0;
	int value = 0;

 	void onSetIndex()
	{
		i = pinIndex;
		pinStringOut = arr[i];
	}

 	void onSetString()
	{		
		value = pinString;
		arr[i] = value;  
		pinStringOut = arr[i];
	}

 	IntGuiPin pinIndex;
 	IntGuiPin pinString;
	IntGuiPin pinStringOut;

public:
	StringArrayGui()
	{
		initializePin( pinIndex, static_cast<MpGuiBaseMemberPtr2>(&StringArrayGui::onSetIndex) );
		initializePin( pinString, static_cast<MpGuiBaseMemberPtr2>(&StringArrayGui::onSetString) );
		initializePin(pinStringOut, static_cast<MpGuiBaseMemberPtr2>(&StringArrayGui::onSetIndex));
	}
};

namespace
{
	auto r = Register<StringArrayGui>::withId(L"IntArray");
}

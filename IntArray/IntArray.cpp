#include "mp_sdk_gui2.h"

using namespace gmpi;

class StringArrayGui final : public SeGuiInvisibleBase
{	
	int arr[' '] = {};
	int i = 0;
	int value = 0;
	int size = 0;

 	void onSetIndex()
	{
		i = pinIndex;
		if (i > size)
		{
			i = size;
		}
		if (i < 0)
		{
			i = 0;
		}
		pinStringOut = arr[i];
	}

 	void onSetString()
	{		
		value = pinString;
		arr[i] = value;  
		pinStringOut = arr[i];
	}

	void onSetSize()
	{
		size = pinSize;
	}

	void onSetSizeL()
	{
		size = pinSizeL;
	}

 	IntGuiPin pinIndex;
 	IntGuiPin pinString;
	IntGuiPin pinSize;
	IntGuiPin pinSizeL;
	IntGuiPin pinStringOut;

public:
	StringArrayGui()
	{
		initializePin( pinIndex, static_cast<MpGuiBaseMemberPtr2>(&StringArrayGui::onSetIndex) );
		initializePin( pinString, static_cast<MpGuiBaseMemberPtr2>(&StringArrayGui::onSetString) );
		initializePin(pinSize, static_cast<MpGuiBaseMemberPtr2>(&IntArrayGui::onSetSize));
		initializePin(pinSizeL, static_cast<MpGuiBaseMemberPtr2>(&IntArrayGui::onSetSizeL));
		initializePin(pinStringOut, static_cast<MpGuiBaseMemberPtr2>(&StringArrayGui::onSetIndex));
	}
};

namespace
{
	auto r = Register<StringArrayGui>::withId(L"IntArray");
}

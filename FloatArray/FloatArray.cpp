#include "mp_sdk_gui2.h"

using namespace gmpi;

class IntArrayGui final : public SeGuiInvisibleBase
{
	float arr[' '] = {};
	int i = 0;
	float value = 0.f;
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
		pinValueOut = arr[i];
		pinValueOutL = arr[i];
	}

 	void onSetValue()
	{
		value = pinValue;
		arr[i] = value;
		pinValueOut = arr[i];
		pinValueOutL = arr[i];
	}
	void onSetValueR()
	{
		value = pinValueR;
		arr[i] = value;
		pinValueOut = arr[i];
		pinValueOutL = arr[i];
	}

	void onSetSize()
	{
		size = pinSize;
	}

 	void onSetValueOut()
	{
		// pinValueOut changed
	}

 	IntGuiPin pinIndex;
 	FloatGuiPin pinValue;
	FloatGuiPin pinValueR;
	IntGuiPin pinSize;
 	FloatGuiPin pinValueOut;
	FloatGuiPin pinValueOutL;

public:
	IntArrayGui()
	{
		initializePin( pinIndex, static_cast<MpGuiBaseMemberPtr2>(&IntArrayGui::onSetIndex) );
		initializePin( pinValue, static_cast<MpGuiBaseMemberPtr2>(&IntArrayGui::onSetValue) );
		initializePin(pinValueR, static_cast<MpGuiBaseMemberPtr2>(&IntArrayGui::onSetValueR));
		initializePin(pinSize, static_cast<MpGuiBaseMemberPtr2>(&IntArrayGui::onSetSize));
		initializePin( pinValueOut, static_cast<MpGuiBaseMemberPtr2>(&IntArrayGui::onSetValueOut));
		initializePin(pinValueOutL, static_cast<MpGuiBaseMemberPtr2>(&IntArrayGui::onSetValueOut));
	}
};

namespace
{
	auto r = Register<IntArrayGui>::withId(L"FloatArray");
}

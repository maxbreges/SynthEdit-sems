#include "mp_sdk_gui2.h"

using namespace gmpi;

class IncrementGui final : public SeGuiInvisibleBase
{
	int value = {};
	int size = {};

	void onSetIncr()
	{
		if (pinIncr)
		{
			value++;

			if (pinWrap)
			{
				if (value >= size)
				{
					value = 0;
				}
			}
			if (!pinWrap)
			{
				if (value >= size)
				{
					value = size -1;
				}
			}
		}
		pinInt = value;
	}
	void onSetDecr()
	{
		if (pinDecr)
		{
			value--;

			if (pinWrap)
			{
				if (value <= 0)
				{
					value = size;
				}
				pinInt = value - 1;
			}
			if (!pinWrap)
			{
				if (value <= 0)
				{
					value = 0;
				}
				pinInt = value;
			}
		}
		
	}
	void onSetListSize()
	{
		size = pinListSize;
	}

	void onSetInt()
	{
	}

	void onSetWrap()
	{
	}

 	BoolGuiPin pinIncr;
	BoolGuiPin pinDecr;
 	IntGuiPin pinListSize;
	IntGuiPin pinInt;
	BoolGuiPin pinWrap;

public:
	IncrementGui()
	{
		initializePin( pinIncr, static_cast<MpGuiBaseMemberPtr2>(&IncrementGui::onSetIncr) );
		initializePin(pinDecr, static_cast<MpGuiBaseMemberPtr2>(&IncrementGui::onSetDecr));
		initializePin(pinListSize, static_cast<MpGuiBaseMemberPtr2>(&IncrementGui::onSetListSize));
		initializePin( pinInt, static_cast<MpGuiBaseMemberPtr2>(&IncrementGui::onSetInt) );
		initializePin(pinWrap, static_cast<MpGuiBaseMemberPtr2>(&IncrementGui::onSetWrap));
	}
};

namespace
{
	auto r = Register<IncrementGui>::withId(L"Increment");
}

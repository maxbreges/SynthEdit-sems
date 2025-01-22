#include "mp_sdk_gui2.h"

using namespace gmpi;

class IncrementGui final : public SeGuiInvisibleBase
{
	int value = {};
	int size = {};

	void onSetWrap()
	{
		if (pinWrap)
		{
			if (value >= size)
			{
				value = 0;
			}
			if (value <= -1)
			{
				value = size -1;
			}
		}

		if (!pinWrap)
		{
			if (value <= 0)
			{
				value = 0;
			}
			if (value >= size)
			{
				value = size -1;
			}			
		}		
	}

	void onSetIncr()
	{
		if (pinIncr)
		{
			value++;
			onSetWrap();
			pinChoice = value;
		}
	}
	void onSetDecr()
	{
		if (pinDecr)
		{
			value--;
			onSetWrap();
			pinChoice = value;
		}		
	}
	void onSetListSize()
	{
		size = pinListSize;
	}

	void onSetChoice()
	{
		value = pinChoice;
	}
	
	IntGuiPin pinListSize;
	BoolGuiPin pinWrap;
	IntGuiPin pinChoice;
 	BoolGuiPin pinIncr;
	BoolGuiPin pinDecr;
	

public:
	IncrementGui()
	{
		initializePin(pinListSize, static_cast<MpGuiBaseMemberPtr2>(&IncrementGui::onSetListSize));
		initializePin(pinWrap, static_cast<MpGuiBaseMemberPtr2>(&IncrementGui::onSetWrap));
		initializePin(pinChoice, static_cast<MpGuiBaseMemberPtr2>(&IncrementGui::onSetChoice));
		initializePin( pinIncr, static_cast<MpGuiBaseMemberPtr2>(&IncrementGui::onSetIncr) );
		initializePin(pinDecr, static_cast<MpGuiBaseMemberPtr2>(&IncrementGui::onSetDecr));				
	}
};

namespace
{
	auto r = Register<IncrementGui>::withId(L"Increment");
}

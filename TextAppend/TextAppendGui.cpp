#include "mp_sdk_gui2.h"

using namespace gmpi;

class TextAppendGui final : public SeGuiInvisibleBase
{
 	
 	StringGuiPin pinText1;
 	StringGuiPin pinText2;
	StringGuiPin pinText3;
	StringGuiPin pinText4;
 	StringGuiPin pinConcatenated;

	void onSetText1()
	{
		pinConcatenated = pinText1.getValue() + pinText2.getValue() + pinText3.getValue() + pinText4.getValue();
	}

	void onSetText2()
	{
		pinConcatenated = pinText1.getValue() + pinText2.getValue() + pinText3.getValue() + pinText4.getValue();
	}

	void onSetText3()
	{
		pinConcatenated = pinText1.getValue() + pinText2.getValue() + pinText3.getValue() + pinText4.getValue();
	}

	void onSetText4()
	{
		pinConcatenated = pinText1.getValue() + pinText2.getValue() + pinText3.getValue() + pinText4.getValue();
	}

	void onSetConcatenated()
	{
	}

public:
	TextAppendGui()
	{
		initializePin( pinText1, static_cast<MpGuiBaseMemberPtr2>(&TextAppendGui::onSetText1) );
		initializePin( pinText2, static_cast<MpGuiBaseMemberPtr2>(&TextAppendGui::onSetText2) );
		initializePin(pinText3, static_cast<MpGuiBaseMemberPtr2>(&TextAppendGui::onSetText3));
		initializePin(pinText4, static_cast<MpGuiBaseMemberPtr2>(&TextAppendGui::onSetText4));
		initializePin( pinConcatenated, static_cast<MpGuiBaseMemberPtr2>(&TextAppendGui::onSetConcatenated) );
	}

};

namespace
{
	auto r = Register<TextAppendGui>::withId(L"TextAppend");
}

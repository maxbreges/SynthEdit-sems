#include "mp_sdk_gui2.h"

using namespace gmpi;

class TextSplitterGui : public gmpi_gui::MpGuiInvisibleBase
{
 	void onSetTextIn()
	{
		pinTextOut = pinTextIn;
	}

 	void onSetTextOut()
	{
		// pinValue changed
	}

 	StringGuiPin pinTextIn;
 	StringGuiPin pinTextOut;


public:
	TextSplitterGui()
	{
		initializePin( pinTextIn, static_cast<MpGuiBaseMemberPtr2>(&TextSplitterGui::onSetTextIn) );
		initializePin( pinTextOut, static_cast<MpGuiBaseMemberPtr2>(&TextSplitterGui::onSetTextOut) );
	}
};

namespace
{
	auto r = Register<TextSplitterGui>::withId(L"TextRedirect");
}

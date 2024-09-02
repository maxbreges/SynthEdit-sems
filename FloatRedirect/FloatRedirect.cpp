#include "mp_sdk_gui2.h"

using namespace gmpi;

class FloatRedirectGui final : public SeGuiInvisibleBase
{
 	void onSetText()
	{
		pinText2 = pinText;
	}

 	void onSetText2()
	{
		onSetText();
	}

 	FloatGuiPin pinText;
 	FloatGuiPin pinText2;

public:
	FloatRedirectGui()
	{
		initializePin( pinText, static_cast<MpGuiBaseMemberPtr2>(&FloatRedirectGui::onSetText) );
		initializePin( pinText2, static_cast<MpGuiBaseMemberPtr2>(&FloatRedirectGui::onSetText2) );
	}

};

namespace
{
	auto r = Register<FloatRedirectGui>::withId(L"FloatRedirect");
}

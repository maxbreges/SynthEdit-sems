#include "mp_sdk_gui2.h"

using namespace gmpi;

class SlashConvertGui final : public SeGuiInvisibleBase
{
    std::string a;

 	void onSetText()
	{
        a = pinTextIn;
        backslashReverse();
	}

 	void onSetText2()
	{
        a = pinTextIn2;
        backslashReverse();
	}

 	StringGuiPin pinTextIn;
 	StringGuiPin pinTextIn2;
 	StringGuiPin pinTextOut;
    StringGuiPin pinTextOut2;

public:
	SlashConvertGui()
	{
		initializePin( pinTextIn, static_cast<MpGuiBaseMemberPtr2>(&SlashConvertGui::onSetText) );
		initializePin( pinTextIn2, static_cast<MpGuiBaseMemberPtr2>(&SlashConvertGui::onSetText2) );
		initializePin( pinTextOut);
        initializePin(pinTextOut2);
	}

    void backslashReverse()
    {
#if defined(_WIN32)
        // On Windows: normalize to backslashes
        
        std::replace(a.begin(), a.end(), '/', '\\');
        pinTextOut = a;
        pinTextOut2 = a;

#elif defined(__APPLE__)
        // On macOS: normalize to forward slashes
        std::replace(a.begin(), a.end(), '\\', '/');
        pinTextOut = a;
        pinTextOut2 = a;
#endif
    }

};

namespace
{
	auto r = Register<SlashConvertGui>::withId(L"My SlashConvert");
}

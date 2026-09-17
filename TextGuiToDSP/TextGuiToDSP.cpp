#include "mp_sdk_audio.h"

using namespace gmpi;

class TextGuiToDSP final : public MpBase2
{
    IntInPin pinIntIn; //fixed value for initialization
	StringOutPin pinStringOut;
    //IntOutPin pinDebug;

public:
	TextGuiToDSP()
	{
        initializePin(pinIntIn);
		initializePin(pinStringOut);  
      //  initializePin(pinDebug);
	}

    void onSetPins() override
    {
        // call a function when initializing the plugin
       // retrievePluginID(static_cast<int32_t>(pinIntIn));
        getHost()->sendMessageToGui(119599, sizeof(pinIntIn), &pinIntIn);
    }

    int32_t MP_STDCALL receiveMessageFromGui(int32_t id, int32_t size, const void* messageData)
    {
        if (id == 119234)
        {
            std::wstring message(reinterpret_cast<const wchar_t*>(messageData), size / sizeof(wchar_t));
            // Remove null terminator if present
            if (!message.empty() && message.back() == L'\0') {
                message.pop_back();
            }
            pinStringOut.setValue(message, getBlockPosition());
        }
        return gmpi::MP_OK;
    }

};

namespace
{
	auto r = Register<TextGuiToDSP>::withId(L"My TextGuiToDSP");
}

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

/*    int32_t retrievePluginID(int value)
    {
        auto handle = getHost()->getHandle(value);
        pinDebug = value;
        return handle; // or return value, depending on intent
    }*/

    void onSetPins() override
    {
        // call a function when initializing the plugin
       // retrievePluginID(static_cast<int32_t>(pinIntIn));
        getHost()->sendMessageToGui(119599, sizeof(pinIntIn), &pinIntIn);
    }

    int32_t MP_STDCALL receiveMessageFromGui(int32_t id, int32_t size, const void* messageData)
    {
        if (id == 119234) // Check if the message ID matches
        {
            // Construct std::wstring from messageData with explicit size
            std::wstring message(reinterpret_cast<const wchar_t*>(messageData), size / sizeof(wchar_t));

            // Find position of ".wav"
            size_t pos = message.find(L".wav");
            if (pos != std::wstring::npos)
            {
                message = message.substr(0, pos+4);
            }

            // Set the output string
            pinStringOut.setValue(message, getBlockPosition());
        }
        return gmpi::MP_OK;
    }

};

namespace
{
	auto r = Register<TextGuiToDSP>::withId(L"My TextGuiToDSP");
}

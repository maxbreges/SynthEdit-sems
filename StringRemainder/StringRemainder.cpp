#include "mp_sdk_audio.h"

using namespace gmpi;

class StringRemainder final : public MpBase2
{
    IntInPin pinIntIn; //fixed value for initialization

public:
    StringRemainder()
    {
        initializePin(pinIntIn);
    }

    void onSetPins() override
    {
        getHost()->sendMessageToGui(116599, sizeof(pinIntIn), &pinIntIn);//sent to gui to get value from gui input pins
    }
};

namespace
{
    auto r = Register<StringRemainder>::withId(L"My StringRemainder");
}

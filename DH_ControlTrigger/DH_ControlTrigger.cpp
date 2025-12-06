#include "mp_sdk_audio.h"

using namespace gmpi;

class DH_ControlTrigger final : public MpBase2
{

    BoolOutPin pinTrigger;

public:
    DH_ControlTrigger()
    {
        initializePin(pinTrigger);
    }

    int32_t receiveMessageFromGui(int32_t id, int32_t size, void* messageData)
    {
        if (id == 3232 && size == sizeof(int32_t))
        {
            pinTrigger.setValue(true, getBlockPosition());

            return 1; // handled
        }

        if (id == 3233 && size == sizeof(int32_t))
        {
            pinTrigger.setValue(false, getBlockPosition()+1);

            return 1; // handled
        }
        return 0; // not handled
    }
};

namespace
{
    auto r = Register<DH_ControlTrigger>::withId(L"My DH_ControlTrigger");
}
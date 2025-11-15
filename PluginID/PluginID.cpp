#include "mp_sdk_audio.h"

using namespace gmpi;

class PluginID final : public MpBase2
{
    // Unique ID for each instance
    uint64_t instanceId = {};

    // Static counter to generate unique IDs
    static uint64_t nextInstanceId;

    // Static counter for active instances
    static int activeInstanceCount;

    AudioInPin pinSignalin;
    IntOutPin pinIntID;
    IntOutPin pinInstanceID;
    IntOutPin pinActiveInstanceCount;

public:
    PluginID()
    {
        initializePin(pinSignalin);
        initializePin(pinIntID);
        initializePin(pinInstanceID);
        initializePin(pinActiveInstanceCount);
        instanceId = ++nextInstanceId;

        // Increment active instance count
         ++activeInstanceCount;
    }

    ~PluginID()
    {
        // Decrement active instance count
        --activeInstanceCount;
        --nextInstanceId;
    }

    static int getActiveInstanceCount()
    {
        return activeInstanceCount;
    }

    int32_t retrievePluginID(int value)
    {
        auto handle = getHost()->getHandle(value);
        pinIntID = value;
        pinInstanceID = instanceId;
        pinActiveInstanceCount = activeInstanceCount;
        return handle;
    }

    void onSetPins() override
    {
        if (pinSignalin.isStreaming())
        {
        }

        retrievePluginID(static_cast<int32_t>(pinSignalin));
    }
};

uint64_t PluginID::nextInstanceId = 0;
int PluginID::activeInstanceCount = 0; // Initialize the static counter

namespace
{
	auto r = Register<PluginID>::withId(L"PluginID");
}

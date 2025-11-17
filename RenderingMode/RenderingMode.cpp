#include "mp_sdk_audio.h"

using namespace gmpi;

class RenderingMode final : public MpBase2
{
	FloatInPin pinHostBpm;
	IntInPin pinHost;
	FloatOutPin pinBPMOut;
	IntOutPin pinIntOut;


public:
	RenderingMode()
	{
		initializePin(pinHostBpm);
		initializePin(pinHost);
		initializePin(pinBPMOut);
		initializePin(pinIntOut);
	}

	void onSetPins() override
	{
		if (pinHost.isUpdated())
			pinIntOut = pinHost;
		if (pinHostBpm.isUpdated())
			pinBPMOut = pinHostBpm;
	}
};

namespace
{
	auto r = Register<RenderingMode>::withId(L"RenderingMode");
}

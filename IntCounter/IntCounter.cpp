#include "mp_sdk_audio.h"

using namespace gmpi;

class IntCounter final : public MpBase2
{
	BoolInPin pinClock;
	BoolInPin pinDecr;
	BoolInPin pinReset;
	IntOutPin pinOutput;

public:
	IntCounter()
	{
		initializePin( pinClock );
		initializePin(pinDecr);
		initializePin( pinReset );
		initializePin( pinOutput );
	}

	int count = 0;

	void onSetPins() override
	{
		if (pinClock)
		{
			count++;
		}	
		if (pinDecr)
		{
			count--;
		}

		if( pinReset.isUpdated() )
		{
			if (pinReset)
			{
				count = 0;
			}				
		}
		pinOutput = count;
	}
};

class IntCounterStep final : public MpBase2
{
	BoolInPin pinIncr;
	BoolInPin pinDecr;
	IntInPin pinSteps;
	BoolInPin pinReset;
	BoolInPin pinWrap;
	IntOutPin pinOutput;

public:
	IntCounterStep()
	{
		initializePin(pinIncr);
		initializePin(pinDecr);
		initializePin(pinSteps);
		initializePin(pinReset);
		initializePin(pinWrap);
		initializePin(pinOutput);
	}

	int count = 0;
	int steps = 0;

	void onSetPins() override
	{
		steps = pinSteps.getValue();

		if (pinIncr)
		{
			count++;

			if (pinWrap)
			{
				if (count == steps)
					count = 0;
			}
			else if (count  >= steps)
			{
				count = steps - 1;
			}
		}

		if (pinDecr)
		{
			count--;

			if (pinWrap)
			{
				if (count == (- 1))
					count = steps - 1;
			}
			else if (count <= 0)
			{
				count = 0;
			}
		}

		if (pinReset.isUpdated())
		{
			if (pinReset)
			{
				count = 0;
			}
		}
		pinOutput = count;
	}
};

namespace
{
	bool r[] = { Register<IntCounter>::withId(L"IntCounter"),
	Register<IntCounterStep>::withId(L"IntCounterStep") };
}
#include "mp_sdk_audio.h"

using namespace gmpi;

class DivideInt final : public MpBase2
{
	IntInPin pinIn;
	IntInPin pinIn2;
	IntOutPin pinOut;

public:
	DivideInt()
	{
		initializePin(pinIn);
		initializePin(pinIn2);
		initializePin(pinOut);
	}

	void onSetPins() override
	{
		int in1 = pinIn.getValue();
		int in2 = pinIn2.getValue();

		// Check which pins are updated.
		if (pinIn.isUpdated())
		{
			if (in1 == 0)
			{
				return;
			}
			int result = in1 / in2;
			pinOut = result;
		}
	}
};

class SubtractInt final : public MpBase2
{
	IntInPin pinIn;
	IntInPin pinIn2;
	IntOutPin pinOut;

public:
	SubtractInt()
	{
		initializePin(pinIn);
		initializePin(pinIn2);
		initializePin(pinOut);
	}

	void onSetPins() override
	{
		int in1 = pinIn.getValue();
		int in2 = pinIn2.getValue();

		// Check which pins are updated.
		if (pinIn.isUpdated())
		{
			int result = in1 - in2;
			pinOut = result;
		}
	}
};

	class MultiplyInt final : public MpBase2
	{
		IntInPin pinIn;
		IntInPin pinIn2;
		IntOutPin pinOut;

	public:
		MultiplyInt()
		{
			initializePin(pinIn);
			initializePin(pinIn2);
			initializePin(pinOut);
		}

		void onSetPins() override
		{
			int in1 = pinIn.getValue();
			int in2 = pinIn2.getValue();

			// Check which pins are updated.
			if (pinIn.isUpdated())
			{
				int result = in1 * in2;
				pinOut = result;
			}
		}
	};
	class AddInt final : public MpBase2
	{
		IntInPin pinIn;
		IntInPin pinIn2;
		IntOutPin pinOut;

	public:
		AddInt()
		{
			initializePin(pinIn);
			initializePin(pinIn2);
			initializePin(pinOut);
		}

		void onSetPins() override
		{
			int in1 = pinIn.getValue();
			int in2 = pinIn2.getValue();

			// Check which pins are updated.
			if (pinIn.isUpdated())
			{
				int result = in1 + in2;
				pinOut = result;
			}
		}
	};

namespace
{
	bool r[] = 
	{
		Register<DivideInt>::withId(L"Divide(Int)"),
		Register<SubtractInt>::withId(L"Subtract(Int)"),
		Register<MultiplyInt>::withId(L"Multiply(Int)"),
		Register<AddInt>::withId(L"Add(Int)")
	};
}

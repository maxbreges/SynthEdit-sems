#include "mp_sdk_gui2.h"

using namespace gmpi;

class Modulus final : public::SeGuiInvisibleBase
{

	void onSetIn()
	{
		onSetOut();
	}

	void onSetOut()
	{

		float a = (pinInput * 0.08333333) - 0.75f;
		float b = a;		
		int c = a;
		float result = (b - c);
		pinOutput = result;
	}

	FloatGuiPin pinInput;	
	FloatGuiPin pinOutput;

public:
	Modulus()
	{
		initializePin(pinInput, static_cast<MpGuiBaseMemberPtr2>(&Modulus::onSetIn));
		initializePin(pinOutput, static_cast<MpGuiBaseMemberPtr2>(&Modulus::onSetOut));
	}

};

namespace
{
	auto r = Register<Modulus>::withId(L"ModulusGui");
}

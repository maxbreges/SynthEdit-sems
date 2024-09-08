#include "mp_sdk_gui2.h"

using namespace gmpi;

class SolomuteGui final : public SeGuiInvisibleBase
{

	bool StateM1 = false;
	bool StateM2 = false;
	bool StateM3 = false;
	bool StateM4 = false;

	bool StateS1 = false;
	bool StateS2 = false;
	bool StateS3 = false;
	bool StateS4 = false;

	void onSetMute()
	{
		if (!pinSoloStateReset)
		{
			if ((!pinmute1) && (!pinmute2) && (!pinmute3) && (!pinmute4))
			{
				pinMute = false;
				StateM1 = StateM2 = StateM3 = StateM4 = false;
			}

			if (pinmute1 || pinmute2 || pinmute3 || pinmute4)
			{
				pinMute = true;
			}
		}
	}

	void onReleaseSoloState()
	{
		if ((!pinsolo1) && (!pinsolo2) && (!pinsolo3) && (!pinsolo4)) //checking if all solos are released
		{
			pinSoloStateReset = false;
			StateS1 = StateS2 = StateS3 = StateS4 = false;

			pinmute1 = StateM1;
			pinmute2 = StateM2;
			pinmute3 = StateM3;
			pinmute4 = StateM4;

			onSetMute();
		}
	}

	void onSetUnmuteMouseDown()
	{
		if (!pinSoloStateReset)
		{
			pinmute1 = pinmute2 = pinmute3 = pinmute4 = pinMute = false;

			StateM1 = StateM2 = StateM3 = StateM4 = false;
		}
		if (pinSoloStateReset)
		{
			pinMute = true;
		}
		onSetChan1();
		onSetChan2();
		onSetChan3();
		onSetChan4();
	}

	void onSetSoloStateReset()
	{
		if ((!pinsolo1) && (!pinsolo2) && (!pinsolo3) && (!pinsolo4)) //checking if all solos are released
		{
			pinSoloStateReset = false;
			pinmute1 = StateM1;
			pinmute2 = StateM2;
			pinmute3 = StateM3;
			pinmute4 = StateM4;

			StateS1 = StateS2 = StateS3 = StateS4 = false;
		}

		if (pinsolo1 || pinsolo2 || pinsolo3 || pinsolo4) //checking if any solo is pressed
		{
			pinSoloStateReset = true;

			SoloStateTrue();
		}
		if (pinSoloStateReset)
		{
			pinMute = true;
		}
	}

	void SoloStateTrue()
	{
		if (pinSoloStateReset) //set mutes to true except soloed
		{
			if (!pinsolo1)
			{
				pinmute1 = true;
			}
			if (!pinsolo2)
			{
				pinmute2 = true;
			}
			if (!pinsolo3)
			{
				pinmute3 = true;
			}
			if (!pinsolo4)
			{
				pinmute4 = true;
			}
		}
	}

	void onSetSoloStateResetMD()
	{
		pinsolo1 = pinsolo2 = pinsolo3 = pinsolo4 = pinSoloStateReset = false;

		StateS1 = StateS2 = StateS3 = StateS4 = false;

		pinmute1 = StateM1;
		pinmute2 = StateM2;
		pinmute3 = StateM3;
		pinmute4 = StateM4;

		if (!pinSoloStateReset)
		{
			if ((!pinmute1) && (!pinmute2) && (!pinmute3) && (!pinmute4))
			{
				pinMute = false;
			}
		}
		onSetChan1();
		onSetChan2();
		onSetChan3();
		onSetChan4();
	}

	//=======================================
	//=======================================

	void onSetmDownM1()
	{
		if (pinMute && !pinSoloStateReset)
		{
			StateM1 = pinmute1;
		}

		if ((!pinsolo1) && (!pinsolo2) && (!pinsolo3) && (!pinsolo4)) //checking if all solos are released
		{
			if (pinmDownM1 && pinmute1)
			{
				bool newStateM1 = pinmute1;
				StateM1 = newStateM1;
			}
			onReleaseSoloState();
		}
		onSetChan1();
		onSetChan2();
		onSetChan3();
		onSetChan4();
	}

	void onSetmute1()
	{
		onSetMute();

		if (pinmute1)
		{
			pinsolo1 = !pinmute1;
		}
		if (!pinmute1)
		{
			pinsolo1 = StateS1;
		}
		if (!pinmute1 && pinSoloStateReset)
		{
			pinsolo1 = true;
		}
	}

	void onSetmDownS1()
	{
		if (pinmDownS1 && pinsolo1)
		{
			bool newStateS1 = pinsolo1;
			StateS1 = newStateS1;
		}
		onSetChan1();
		onSetChan2();
		onSetChan3();
		onSetChan4();
	}

	void onSetsolo1()
	{
		onSetSoloStateReset();

		if (pinsolo1)
		{
			pinmute1 = false;

			pinSoloStateReset = true;
		}

		onSetMute();
	}

	//=======================================

	void onSetmDownM2()
	{
		if (pinMute && !pinSoloStateReset)
		{
			StateM2 = pinmute2;
		}

		if ((!pinsolo1) && (!pinsolo2) && (!pinsolo3) && (!pinsolo4)) //checking if all solos are released
		{
			if (pinmDownM2 && pinmute2)
			{
				bool newStateM2 = pinmute2;
				StateM2 = newStateM2;
			}
			onReleaseSoloState();
		}
		onSetChan1();
		onSetChan2();
		onSetChan3();
		onSetChan4();
	}

	void onSetmute2()
	{
		onSetMute();

		if (pinmute2)
		{
			pinsolo2 = !pinmute2;
		}
		if (!pinmute2)
		{
			pinsolo2 = StateS2;
		}
		if (!pinmute2 && pinSoloStateReset)
		{
			pinsolo2 = true;
		}
	}

	void onSetmDownS2()
	{
		if (pinmDownS2 && pinsolo2)
		{
			bool newStateS2 = pinsolo2;
			StateS2 = newStateS2;
		}
		onSetChan1();
		onSetChan2();
		onSetChan3();
		onSetChan4();
	}

	void onSetsolo2()
	{
		onSetSoloStateReset();

		if (pinsolo2)
		{
			pinmute2 = false;

			pinSoloStateReset = true;
		}
		onSetMute();

	}

	//=======================================

	void onSetmDownM3()
	{
		if (pinMute && !pinSoloStateReset)
		{
			StateM3 = pinmute3;
		}

		if ((!pinsolo1) && (!pinsolo2) && (!pinsolo3) && (!pinsolo4)) //checking if all solos are released
		{
			if (pinmDownM3 && pinmute3)
			{
				bool newStateM3 = pinmute3;
				StateM3 = newStateM3;
			}
			onReleaseSoloState();
		}
		onSetChan1();
		onSetChan2();
		onSetChan3();
		onSetChan4();
	}

	void onSetmute3()
	{
		onSetMute();

		if (pinmute3)
		{
			pinsolo3 = !pinmute3;
		}
		if (!pinmute3)
		{
			pinsolo3 = StateS3;
		}
		if (!pinmute3 && pinSoloStateReset)
		{
			pinsolo3 = true;
		}
	}

	void onSetmDownS3()
	{
		if (pinmDownS3 && pinsolo3)
		{
			bool newStateS3 = pinsolo3;
			StateS3 = newStateS3;
		}
		onSetChan1();
		onSetChan2();
		onSetChan3();
		onSetChan4();
	}

	void onSetsolo3()
	{
		onSetSoloStateReset();

		if (pinsolo3)
		{
			pinmute3 = false;

			pinSoloStateReset = true;
		}
		onSetMute();
	}

	//=======================================

	void onSetmDownM4()
	{
		if (pinMute && !pinSoloStateReset)
		{
			StateM4 = pinmute4;
		}

		if ((!pinsolo1) && (!pinsolo2) && (!pinsolo3) && (!pinsolo4)) //checking if all solos are released
		{
			if (pinmDownM4 && pinmute4)
			{
				bool newStateM4 = pinmute4;
				StateM4 = newStateM4;
			}
			onReleaseSoloState();
		}
		onSetChan1();
		onSetChan2();
		onSetChan3();
		onSetChan4();
	}

	void onSetmute4()
	{
		onSetMute();

		if (pinmute4)
		{
			pinsolo4 = !pinmute4;
		}
		if (!pinmute4)
		{
			pinsolo4 = StateS4;
		}
		if (!pinmute4 && pinSoloStateReset)
		{
			pinsolo4 = true;
		}
	}

	void onSetmDownS4()
	{
		if (pinmDownS4 && pinsolo4)
		{
			bool newStateS4 = pinsolo4;
			StateS4 = newStateS4;
		}
		onSetChan1();
		onSetChan2();
		onSetChan3();
		onSetChan4();
	}

	void onSetsolo4()
	{
		onSetSoloStateReset();

		if (pinsolo4)
		{
			pinmute4 = false;

			pinSoloStateReset = true;
		}
		onSetMute();
	}

	//=======================================

	FloatGuiPin pinChanS1;
	FloatGuiPin pinChanS2;
	FloatGuiPin pinChanS3;
	FloatGuiPin pinChanS4;

	FloatGuiPin pinMute;
	BoolGuiPin pinUnmuteMouseDown;
	FloatGuiPin pinSoloStateReset;
	BoolGuiPin pinSoloStateResetMD;

	FloatGuiPin pinmute1;
	BoolGuiPin pinmDownM1;
	FloatGuiPin pinsolo1;
	BoolGuiPin pinmDownS1;

	FloatGuiPin pinmute2;
	BoolGuiPin pinmDownM2;
	FloatGuiPin pinsolo2;
	BoolGuiPin pinmDownS2;

	FloatGuiPin pinmute3;
	BoolGuiPin pinmDownM3;
	FloatGuiPin pinsolo3;
	BoolGuiPin pinmDownS3;

	FloatGuiPin pinmute4;
	BoolGuiPin pinmDownM4;
	FloatGuiPin pinsolo4;
	BoolGuiPin pinmDownS4;

	FloatGuiPin pinChanA1;
	FloatGuiPin pinChanA2;
	FloatGuiPin pinChanA3;
	FloatGuiPin pinChanA4;

	void onSetChan1()
	{
		if (pinsolo1)
		{
			pinChanA1 = 1;
		}
		if (!pinsolo1 && !pinmute1)
		{
			pinChanA1 = 1;
		}
		if (pinmute1)
		{
			pinChanA1 = 0;
		}
		pinChanS1 = pinChanA1;
	}

	void onSetChan2()
	{
		if (pinsolo2)
		{
			pinChanA2 = 1;
		}
		if (!pinsolo2 && !pinmute2)
		{
			pinChanA2 = 1;
		}
		if (pinmute2)
		{
			pinChanA2 = 0;
		}
		pinChanS2 = pinChanA2;
	}

	void onSetChan3()
	{
		if (pinsolo3)
		{
			pinChanA3 = 1;
		}
		if (!pinsolo3 && !pinmute3)
		{
			pinChanA3 = 1;
		}
		if (pinmute3)
		{
			pinChanA3 = 0;
		}
		pinChanS3 = pinChanA3;
	}

	void onSetChan4()
	{
		if (pinsolo4)
		{
			pinChanA4 = 1;
		}
		if (!pinsolo4 && !pinmute4)
		{
			pinChanA4 = 1;
		}
		if (pinmute4)
		{
			pinChanA4 = 0;
		}
		pinChanS4 = pinChanA4;
	}

public:
	SolomuteGui()
	{
		initializePin(pinChanS1, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetChan1));
		initializePin(pinChanS2, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetChan2));
		initializePin(pinChanS3, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetChan3));
		initializePin(pinChanS4, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetChan4));

		initializePin(pinMute, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetMute));
		initializePin(pinUnmuteMouseDown, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetUnmuteMouseDown));
		initializePin(pinSoloStateReset, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetSoloStateReset));
		initializePin(pinSoloStateResetMD, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetSoloStateResetMD));

		initializePin(pinmute1, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmute1));
		initializePin(pinmDownM1, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmDownM1));
		initializePin(pinsolo1, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetsolo1));
		initializePin(pinmDownS1, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmDownS1));

		initializePin(pinmute2, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmute2));
		initializePin(pinmDownM2, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmDownM2));
		initializePin(pinsolo2, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetsolo2));
		initializePin(pinmDownS2, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmDownS2));

		initializePin(pinmute3, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmute3));
		initializePin(pinmDownM3, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmDownM3));
		initializePin(pinsolo3, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetsolo3));
		initializePin(pinmDownS3, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmDownS3));

		initializePin(pinmute4, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmute4));
		initializePin(pinmDownM4, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmDownM4));
		initializePin(pinsolo4, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetsolo4));
		initializePin(pinmDownS4, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmDownS4));

		initializePin(pinChanA1, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetChan1));
		initializePin(pinChanA2, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetChan2));
		initializePin(pinChanA3, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetChan3));
		initializePin(pinChanA4, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetChan4));
	}
};

namespace
{
	auto r = Register<SolomuteGui>::withId(L"mxSOLOMUTE");
}

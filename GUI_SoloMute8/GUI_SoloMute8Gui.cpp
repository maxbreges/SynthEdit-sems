#include "mp_sdk_gui2.h"

using namespace gmpi;

class SolomuteGui final : public SeGuiInvisibleBase
{

	bool StateM1 = false;
	bool StateM2 = false;
	bool StateM3 = false;
	bool StateM4 = false;
	bool StateM5 = false;
	bool StateM6 = false;
	bool StateM7 = false;
	bool StateM8 = false;

	bool StateS1 = false;
	bool StateS2 = false;
	bool StateS3 = false;
	bool StateS4 = false;
	bool StateS5 = false;
	bool StateS6 = false;
	bool StateS7 = false;
	bool StateS8 = false;

	void onSetMute()
	{
		if (!pinSoloStateReset)
		{
			if ((!pinmute1) && (!pinmute2) && (!pinmute3) && (!pinmute4) && (!pinmute5) && (!pinmute6) && (!pinmute7) && (!pinmute8))
			{
				pinMute = false;
				StateM1 = StateM2 = StateM3 = StateM4 = StateM5 = StateM6 = StateM7 = StateM8 = false;
			}

			if (pinmute1 || pinmute2 || pinmute3 || pinmute4 || pinmute5 || pinmute6 || pinmute7 || pinmute8)
			{
				pinMute = true;
			}
		}
	}

	void onReleaseSoloState()
	{
		if ((!pinsolo1) && (!pinsolo2) && (!pinsolo3) && (!pinsolo4) && (!pinsolo5) && (!pinsolo6) && (!pinsolo7) && (!pinsolo8)) //checking if all solos are released
		{
			pinSoloStateReset = false;
			StateS1 = StateS2 = StateS3 = StateS4 = StateS5 = StateS6 = StateS7 = StateS8 = false;

			pinmute1 = StateM1;
			pinmute2 = StateM2;
			pinmute3 = StateM3;
			pinmute4 = StateM4;
			pinmute5 = StateM5;
			pinmute6 = StateM6;
			pinmute7 = StateM7;
			pinmute8 = StateM8;

			onSetMute();
		}
	}

	void onSetUnmuteMouseDown()
	{
		if (!pinSoloStateReset)
		{
			pinmute1 = pinmute2 = pinmute3 = pinmute4 = pinmute5 = pinmute6 = pinmute7 = pinmute8 = pinMute = false;

			StateM1 = StateM2 = StateM3 = StateM4 = StateM5 = StateM6 = StateM7 = StateM8 = false;
		}
		if (pinSoloStateReset)
		{
			pinMute = true;
		}
		onSetChan1();
		onSetChan2();
		onSetChan3();
		onSetChan4();
		onSetChan5();
		onSetChan6();
		onSetChan7();
		onSetChan8();
	}

	void onSetSoloStateReset()
	{
		if ((!pinsolo1) && (!pinsolo2) && (!pinsolo3) && (!pinsolo4) && (!pinsolo5) && (!pinsolo6) && (!pinsolo7) && (!pinsolo8)) //checking if all solos are released
		{
			pinSoloStateReset = false;
			pinmute1 = StateM1;
			pinmute2 = StateM2;
			pinmute3 = StateM3;
			pinmute4 = StateM4;
			pinmute5 = StateM5;
			pinmute6 = StateM6;
			pinmute7 = StateM7;
			pinmute8 = StateM8;

			StateS1 = StateS2 = StateS3 = StateS4 = StateS5 = StateS6 = StateS7 = StateS8 = false;
		}

		if (pinsolo1 || pinsolo2 || pinsolo3 || pinsolo4 || pinsolo5 || pinsolo6 || pinsolo7 || pinsolo8) //checking if any solo is pressed
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
			if (!pinsolo5)
			{
				pinmute5 = true;
			}
			if (!pinsolo6)
			{
				pinmute6 = true;
			}
			if (!pinsolo7)
			{
				pinmute7 = true;
			}
			if (!pinsolo8)
			{
				pinmute8 = true;
			}
		}
	}

	void onSetSoloStateResetMD()
	{
		pinsolo1 = pinsolo2 = pinsolo3 = pinsolo4 = pinsolo5 = pinsolo6 = pinsolo7 = pinsolo8 = pinSoloStateReset = false;

		StateS1 = StateS2 = StateS3 = StateS4 = StateS5 = StateS6 = StateS7 = StateS8 = false;

		pinmute1 = StateM1;
		pinmute2 = StateM2;
		pinmute3 = StateM3;
		pinmute4 = StateM4;
		pinmute5 = StateM5;
		pinmute6 = StateM6;
		pinmute7 = StateM7;
		pinmute8 = StateM8;

		if (!pinSoloStateReset)
		{
			if ((!pinmute1) && (!pinmute2) && (!pinmute3) && (!pinmute4) && (!pinmute5) && (!pinmute6) && (!pinmute7) && (!pinmute8))
			{
				pinMute = false;
			}
		}
		onSetChan1();
		onSetChan2();
		onSetChan3();
		onSetChan4();
		onSetChan5();
		onSetChan6();
		onSetChan7();
		onSetChan8();
	}

	//=======================================
	//=======================================

	void onSetmDownM1()
	{
		if (pinMute && !pinSoloStateReset)
		{
			StateM1 = pinmute1;
		}

		if ((!pinsolo1) && (!pinsolo2) && (!pinsolo3) && (!pinsolo4) && (!pinsolo5) && (!pinsolo6) && (!pinsolo7) && (!pinsolo8)) //checking if all solos are released
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
		onSetChan5();
		onSetChan6();
		onSetChan7();
		onSetChan8();
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
		onSetChan5();
		onSetChan6();
		onSetChan7();
		onSetChan8();
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

		if ((!pinsolo1) && (!pinsolo2) && (!pinsolo3) && (!pinsolo4) && (!pinsolo5) && (!pinsolo6) && (!pinsolo7) && (!pinsolo8)) //checking if all solos are released
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
		onSetChan5();
		onSetChan6();
		onSetChan7();
		onSetChan8();
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
		onSetChan5();
		onSetChan6();
		onSetChan7();
		onSetChan8();
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

		if ((!pinsolo1) && (!pinsolo2) && (!pinsolo3) && (!pinsolo4) && (!pinsolo5) && (!pinsolo6) && (!pinsolo7) && (!pinsolo8)) //checking if all solos are released
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
		onSetChan5();
		onSetChan6();
		onSetChan7();
		onSetChan8();
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
		onSetChan5();
		onSetChan6();
		onSetChan7();
		onSetChan8();
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

		if ((!pinsolo1) && (!pinsolo2) && (!pinsolo3) && (!pinsolo4) && (!pinsolo5) && (!pinsolo6) && (!pinsolo7) && (!pinsolo8)) //checking if all solos are released
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
		onSetChan5();
		onSetChan6();
		onSetChan7();
		onSetChan8();
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
		onSetChan5();
		onSetChan6();
		onSetChan7();
		onSetChan8();
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

	///==============////==================///

	void onSetmDownM5()
	{
		if (pinMute && !pinSoloStateReset)
		{
			StateM5 = pinmute5;
		}

		if ((!pinsolo1) && (!pinsolo2) && (!pinsolo3) && (!pinsolo4) && (!pinsolo5) && (!pinsolo6) && (!pinsolo7) && (!pinsolo8)) //checking if all solos are released
		{
			if (pinmDownM5 && pinmute5)
			{
				bool newStateM5 = pinmute5;
				StateM5 = newStateM5;
			}
			onReleaseSoloState();
		}
		onSetChan1();
		onSetChan2();
		onSetChan3();
		onSetChan4();
		onSetChan5();
		onSetChan6();
		onSetChan7();
		onSetChan8();
	}

	void onSetmute5()
	{
		onSetMute();

		if (pinmute5)
		{
			pinsolo5 = !pinmute5;
		}
		if (!pinmute5)
		{
			pinsolo5 = StateS5;
		}
		if (!pinmute5 && pinSoloStateReset)
		{
			pinsolo5 = true;
		}
	}

	void onSetmDownS5()
	{
		if (pinmDownS5 && pinsolo5)
		{
			bool newStateS5 = pinsolo5;
			StateS5 = newStateS5;
		}
		onSetChan1();
		onSetChan2();
		onSetChan3();
		onSetChan4();
		onSetChan5();
		onSetChan6();
		onSetChan7();
		onSetChan8();
	}

	void onSetsolo5()
	{
		onSetSoloStateReset();

		if (pinsolo5)
		{
			pinmute5 = false;

			pinSoloStateReset = true;
		}

		onSetMute();
	}

	//=======================================

	void onSetmDownM6()
	{
		if (pinMute && !pinSoloStateReset)
		{
			StateM6 = pinmute6;
		}

		if ((!pinsolo1) && (!pinsolo2) && (!pinsolo3) && (!pinsolo4) && (!pinsolo5) && (!pinsolo6) && (!pinsolo7) && (!pinsolo8)) //checking if all solos are released
		{
			if (pinmDownM6 && pinmute6)
			{
				bool newStateM6 = pinmute6;
				StateM6 = newStateM6;
			}
			onReleaseSoloState();
		}
		onSetChan1();
		onSetChan2();
		onSetChan3();
		onSetChan4();
		onSetChan5();
		onSetChan6();
		onSetChan7();
		onSetChan8();
	}

	void onSetmute6()
	{
		onSetMute();

		if (pinmute6)
		{
			pinsolo6 = !pinmute6;
		}
		if (!pinmute6)
		{
			pinsolo6 = StateS6;
		}
		if (!pinmute6 && pinSoloStateReset)
		{
			pinsolo6 = true;
		}
	}

	void onSetmDownS6()
	{
		if (pinmDownS6 && pinsolo6)
		{
			bool newStateS6 = pinsolo6;
			StateS6 = newStateS6;
		}
		onSetChan1();
		onSetChan2();
		onSetChan3();
		onSetChan4();
		onSetChan5();
		onSetChan6();
		onSetChan7();
		onSetChan8();
	}

	void onSetsolo6()
	{
		onSetSoloStateReset();

		if (pinsolo6)
		{
			pinmute6 = false;

			pinSoloStateReset = true;
		}
		onSetMute();

	}

	//=======================================

	void onSetmDownM7()
	{
		if (pinMute && !pinSoloStateReset)
		{
			StateM7 = pinmute7;
		}

		if ((!pinsolo1) && (!pinsolo2) && (!pinsolo3) && (!pinsolo4) && (!pinsolo5) && (!pinsolo6) && (!pinsolo7) && (!pinsolo8)) //checking if all solos are released
		{
			if (pinmDownM7 && pinmute7)
			{
				bool newStateM7 = pinmute7;
				StateM7 = newStateM7;
			}
			onReleaseSoloState();
		}
		onSetChan1();
		onSetChan2();
		onSetChan3();
		onSetChan4();
		onSetChan5();
		onSetChan6();
		onSetChan7();
		onSetChan8();
	}

	void onSetmute7()
	{
		onSetMute();

		if (pinmute7)
		{
			pinsolo7 = !pinmute7;
		}
		if (!pinmute7)
		{
			pinsolo7 = StateS7;
		}
		if (!pinmute7 && pinSoloStateReset)
		{
			pinsolo7 = true;
		}
	}

	void onSetmDownS7()
	{
		if (pinmDownS7 && pinsolo7)
		{
			bool newStateS7 = pinsolo7;
			StateS7 = newStateS7;
		}
		onSetChan1();
		onSetChan2();
		onSetChan3();
		onSetChan4();
		onSetChan5();
		onSetChan6();
		onSetChan7();
		onSetChan8();
	}

	void onSetsolo7()
	{
		onSetSoloStateReset();

		if (pinsolo7)
		{
			pinmute7 = false;

			pinSoloStateReset = true;
		}
		onSetMute();
	}

	//=======================================

	void onSetmDownM8()
	{
		if (pinMute && !pinSoloStateReset)
		{
			StateM8 = pinmute8;
		}

		if ((!pinsolo1) && (!pinsolo2) && (!pinsolo3) && (!pinsolo4) && (!pinsolo5) && (!pinsolo6) && (!pinsolo7) && (!pinsolo8)) //checking if all solos are released
		{
			if (pinmDownM8 && pinmute8)
			{
				bool newStateM8 = pinmute8;
				StateM8 = newStateM8;
			}
			onReleaseSoloState();
		}
		onSetChan1();
		onSetChan2();
		onSetChan3();
		onSetChan4();
		onSetChan5();
		onSetChan6();
		onSetChan7();
		onSetChan8();
	}

	void onSetmute8()
	{
		onSetMute();

		if (pinmute8)
		{
			pinsolo8 = !pinmute8;
		}
		if (!pinmute8)
		{
			pinsolo8 = StateS8;
		}
		if (!pinmute8 && pinSoloStateReset)
		{
			pinsolo8 = true;
		}
	}

	void onSetmDownS8()
	{
		if (pinmDownS8 && pinsolo8)
		{
			bool newStateS8 = pinsolo8;
			StateS8 = newStateS8;
		}
		onSetChan1();
		onSetChan2();
		onSetChan3();
		onSetChan4();
		onSetChan5();
		onSetChan6();
		onSetChan7();
		onSetChan8();
	}

	void onSetsolo8()
	{
		onSetSoloStateReset();

		if (pinsolo8)
		{
			pinmute8 = false;

			pinSoloStateReset = true;
		}
		onSetMute();
	}

	//=======================================

	FloatGuiPin pinChanS1;
	FloatGuiPin pinChanS2;
	FloatGuiPin pinChanS3;
	FloatGuiPin pinChanS4;

	FloatGuiPin pinChanS5;
	FloatGuiPin pinChanS6;
	FloatGuiPin pinChanS7;
	FloatGuiPin pinChanS8;

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

	FloatGuiPin pinmute5;
	BoolGuiPin pinmDownM5;
	FloatGuiPin pinsolo5;
	BoolGuiPin pinmDownS5;

	FloatGuiPin pinmute6;
	BoolGuiPin pinmDownM6;
	FloatGuiPin pinsolo6;
	BoolGuiPin pinmDownS6;

	FloatGuiPin pinmute7;
	BoolGuiPin pinmDownM7;
	FloatGuiPin pinsolo7;
	BoolGuiPin pinmDownS7;

	FloatGuiPin pinmute8;
	BoolGuiPin pinmDownM8;
	FloatGuiPin pinsolo8;
	BoolGuiPin pinmDownS8;

	

	void onSetChan1()
	{
		if (pinsolo1)
		{
			pinChanS1 = 1;
		}
		if (!pinsolo1 && !pinmute1)
		{
			pinChanS1 = 1;
		}
		if (pinmute1)
		{
			pinChanS1 = 0;
		}
		//pinChanS1 = pinChanA1;
	}

	void onSetChan2()
	{
		if (pinsolo2)
		{
			pinChanS2 = 1;
		}
		if (!pinsolo2 && !pinmute2)
		{
			pinChanS2 = 1;
		}
		if (pinmute2)
		{
			pinChanS2 = 0;
		}
		//pinChanS2 = pinChanA2;
	}

	void onSetChan3()
	{
		if (pinsolo3)
		{
			pinChanS3 = 1;
		}
		if (!pinsolo3 && !pinmute3)
		{
			pinChanS3 = 1;
		}
		if (pinmute3)
		{
			pinChanS3 = 0;
		}
		//pinChanS3 = pinChanA3;
	}

	void onSetChan4()
	{
		if (pinsolo4)
		{
			pinChanS4 = 1;
		}
		if (!pinsolo4 && !pinmute4)
		{
			pinChanS4 = 1;
		}
		if (pinmute4)
		{
			pinChanS4 = 0;
		}
		//pinChanS4 = pinChanA4;
	}
	void onSetChan5()
	{
		if (pinsolo5)
		{
			pinChanS5 = 1;
		}
		if (!pinsolo5 && !pinmute5)
		{
			pinChanS5 = 1;
		}
		if (pinmute5)
		{
			pinChanS5 = 0;
		}
	}
	void onSetChan6()
	{
		if (pinsolo6)
		{
			pinChanS6 = 1;
		}
		if (!pinsolo6 && !pinmute6)
		{
			pinChanS6 = 1;
		}
		if (pinmute6)
		{
			pinChanS6 = 0;
		}
	}
	void onSetChan7()
	{
		if (pinsolo7)
		{
			pinChanS7 = 1;
		}
		if (!pinsolo7 && !pinmute7)
		{
			pinChanS7 = 1;
		}
		if (pinmute7)
		{
			pinChanS7 = 0;
		}
	}
	void onSetChan8()
	{
		if (pinsolo8)
		{
			pinChanS8 = 1;
		}
		if (!pinsolo8 && !pinmute8)
		{
			pinChanS8 = 1;
		}
		if (pinmute8)
		{
			pinChanS8 = 0;
		}
	}

public:
	SolomuteGui()
	{
		initializePin(pinChanS1, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetChan1));
		initializePin(pinChanS2, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetChan2));
		initializePin(pinChanS3, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetChan3));
		initializePin(pinChanS4, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetChan4));

		initializePin(pinChanS5, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetChan5));
		initializePin(pinChanS6, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetChan6));
		initializePin(pinChanS7, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetChan7));
		initializePin(pinChanS8, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetChan8));

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

		initializePin(pinmute5, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmute5));
		initializePin(pinmDownM5, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmDownM5));
		initializePin(pinsolo5, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetsolo5));
		initializePin(pinmDownS5, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmDownS5));

		initializePin(pinmute6, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmute6));
		initializePin(pinmDownM6, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmDownM6));
		initializePin(pinsolo6, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetsolo6));
		initializePin(pinmDownS6, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmDownS6));

		initializePin(pinmute7, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmute7));
		initializePin(pinmDownM7, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmDownM7));
		initializePin(pinsolo7, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetsolo7));
		initializePin(pinmDownS7, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmDownS7));

		initializePin(pinmute8, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmute8));
		initializePin(pinmDownM8, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmDownM8));
		initializePin(pinsolo8, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetsolo8));
		initializePin(pinmDownS8, static_cast<MpGuiBaseMemberPtr2>(&SolomuteGui::onSetmDownS8));

		
	}
};

namespace
{
	auto r = Register<SolomuteGui>::withId(L"SOLOMUTE8");
}

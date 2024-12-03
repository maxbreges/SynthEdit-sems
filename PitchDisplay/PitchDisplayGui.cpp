#include "mp_sdk_gui2.h"
#include <sstream>

using namespace gmpi;

class PitchDisplayGui final : public SeGuiInvisibleBase
{
	std::string acc;

 	void onSetPitchIn()
	{	
		if (pinSwitch)
		{
			wchar_t accsh[] = { L'♯', L'\0' };
			pinAccidentals = accsh;;
			Note2CharSharps();
		}

		if (!pinSwitch)
		{
			wchar_t accfl[] = { L'♭', L'\0' };
			pinAccidentals = accfl;
			Note2CharFlats();
		}		
	}

	void onSetpinSwitch()
	{
		onSetPitchIn();
	}

 	IntGuiPin pinPitchIn; //receives dsp side
	StringGuiPin pinText;
	BoolGuiPin pinSwitch;
	StringGuiPin pinAccidentals;	

public:
	PitchDisplayGui()
	{
		initializePin(pinPitchIn, static_cast<MpGuiBaseMemberPtr2>(&PitchDisplayGui::onSetPitchIn) );
		initializePin(pinText, static_cast<MpGuiBaseMemberPtr2>(&PitchDisplayGui::onSetPitchIn));	
		initializePin(pinSwitch, static_cast<MpGuiBaseMemberPtr2>(&PitchDisplayGui::onSetpinSwitch));
		initializePin(pinAccidentals, static_cast<MpGuiBaseMemberPtr2>(&PitchDisplayGui::onSetPitchIn));
	}

	//char32_t ch0{ U266D};
	//char32_t ch1{ U266F };
	//char32_t ch2{ U266E };

	//(U266D) ♭
	//	sharp(U266F) ♯
	//	natural(U266E) ♮

	int32_t Note2CharSharps ()
	{
		int mi = pinPitchIn;
		signed int oct_v = mi / 12 - 1;		

		std::string nt3;
		acc = pinAccidentals;
		// with switch
		switch (mi % 12)
		{
		case 0: nt3 = "C"; break;
		case 1: nt3 = "C" + acc; break;
		case 2: nt3 = "D"; break;
		case 3: nt3 = "D" + acc; break;
		case 4: nt3 = "E"; break;
		case 5: nt3 = "F"; break;
		case 6: nt3 = "F" + acc; break;
		case 7: nt3 = "G"; break;
		case 8: nt3 = "G" + acc; break;
		case 9: nt3 = "A"; break;
		case 10: nt3 = "A" + acc; break;
		case 11: nt3 = "B"; break;
		}

		std::stringstream oct;		
		oct << std::fixed << oct_v;
		std::string oct_(oct.str());

		pinText = nt3 + oct_;

		return 0;
	}	

	int32_t Note2CharFlats()
	{
		int mi = pinPitchIn;
		signed int oct_v = mi / 12 - 1;
		
		std::string nt3;
		acc = pinAccidentals;
		// with switch
		switch (mi % 12)
		{
		case 0: nt3 = "C"; break;
		case 1: nt3 = "D"+acc; break;
		case 2: nt3 = "D"; break;
		case 3: nt3 = "E" + acc; break;
		case 4: nt3 = "E"; break;
		case 5: nt3 = "F"; break;
		case 6: nt3 = "G" + acc; break;
		case 7: nt3 = "G"; break;
		case 8: nt3 = "A" + acc; break;
		case 9: nt3 = "A"; break;
		case 10: nt3 = "B" + acc; break;
		case 11: nt3 = "B"; break;
		}

		std::stringstream oct;
		oct << std::fixed << oct_v;
		std::string oct_(oct.str());
		
		pinText = nt3 + oct_;

		return 0;
	}

};

namespace
{
	auto r = Register<PitchDisplayGui>::withId(L"PitchDisplay");
}

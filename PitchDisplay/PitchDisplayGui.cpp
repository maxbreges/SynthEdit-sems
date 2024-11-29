#include "mp_sdk_gui2.h"
#include <sstream>

using namespace gmpi;

class PitchDisplayGui final : public SeGuiInvisibleBase
{

 	void onSetPitchIn()
	{	
		if (pinSwitch)
		{
			Note2CharSharps();
		}

		if (!pinSwitch)
		{
			Note2CharFlats();
		}
		//int modulus = (pinPitchIn / 12);
		//pinOctave = modulus -1;
		//pinNote = pinPitchIn;
		//pinSemi = (((pinOctave+1) * 12) - pinNote) * (-1);
		
	}

	void onSetpinSwitch()
	{
		onSetPitchIn();
	}

 	IntGuiPin pinPitchIn;
	//IntGuiPin pinOctave;
	//IntGuiPin pinNote;
	//IntGuiPin pinSemi;
	StringGuiPin pinText;
	BoolGuiPin pinSwitch;

public:
	PitchDisplayGui()
	{
		initializePin(pinPitchIn, static_cast<MpGuiBaseMemberPtr2>(&PitchDisplayGui::onSetPitchIn) );
		//initializePin(pinOctave, static_cast<MpGuiBaseMemberPtr2>(&PitchDisplayGui::onSetPitchIn) );
		//initializePin(pinNote, static_cast<MpGuiBaseMemberPtr2>(&PitchDisplayGui::onSetPitchIn));
		//initializePin(pinSemi, static_cast<MpGuiBaseMemberPtr2>(&PitchDisplayGui::onSetPitchIn));
		initializePin(pinText, static_cast<MpGuiBaseMemberPtr2>(&PitchDisplayGui::onSetPitchIn));
		initializePin(pinSwitch, static_cast<MpGuiBaseMemberPtr2>(&PitchDisplayGui::onSetpinSwitch));
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
		std::string notes[] = { "C","C#","D","D#","E","F","F#","G","G#","A","A#","B" };
		std::string nt = notes[mi % 12];
		std::stringstream oct;		
		oct << std::fixed << oct_v;
		std::string oct_(oct.str());

		pinText = nt + oct_;
	}

	int32_t Note2CharFlats()
	{
		int mi = pinPitchIn;
		signed int oct_v = mi / 12 - 1;
		std::string notes[] = { "C","Db","D","Eb","E","F","Gb","G","Ab","A","Bb","B" };
		std::string nt = notes[mi % 12];
		std::stringstream oct;
		oct << std::fixed << oct_v;
		std::string oct_(oct.str());

		pinText = nt + oct_;
	}

};

namespace
{
	auto r = Register<PitchDisplayGui>::withId(L"PitchDisplay");
}

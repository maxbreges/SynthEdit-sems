#include "mp_sdk_gui2.h"
#include <sstream>

using namespace gmpi;

class MIDILearnGui final : public SeGuiInvisibleBase
{ 	

 	void onSetAnimationPosition()
	{
			pinGate = pinAnimationPosition;			
	}

	void onSetGate()
	{ 

	}

	void onSetGateIn()
	{
		if (pinGateIn)
			pinAnimationPosition = false;

		onSetAnimationPosition();
	}

	std::string acc;

	void onSetNoteIn()
	{
		pinNoteOut = pinNoteIn;

		if (!pinSwitch)
		{
			wchar_t accsh[] = { L'♯', L'\0' };
			pinAccidentals = accsh;;
			Note2CharSharps();
		}

		if (pinSwitch)
		{
			wchar_t accfl[] = { L'♭', L'\0' };
			pinAccidentals = accfl;
			Note2CharFlats();
		}
	}	
	
	void onSetSwitch()
	{

	}
 	
 	FloatGuiPin pinAnimationPosition;
	BoolGuiPin pinSwitch;
	BoolGuiPin pinGate;
	BoolGuiPin pinGateIn;
	StringGuiPin pinNote;
	IntGuiPin pinNoteIn;	
	StringGuiPin pinAccidentals; //hidden pin, memory cell of a string
	IntGuiPin pinNoteOut;

public:
	MIDILearnGui()
	{
		initializePin( pinAnimationPosition, static_cast<MpGuiBaseMemberPtr2>(&MIDILearnGui::onSetAnimationPosition) );
		initializePin(pinSwitch, static_cast<MpGuiBaseMemberPtr2>(&MIDILearnGui::onSetSwitch));
		initializePin(pinGate, static_cast<MpGuiBaseMemberPtr2>(&MIDILearnGui::onSetAnimationPosition));
		initializePin(pinGateIn, static_cast<MpGuiBaseMemberPtr2>(&MIDILearnGui::onSetGateIn));
		initializePin(pinNote, static_cast<MpGuiBaseMemberPtr2>(&MIDILearnGui::onSetAnimationPosition));
		initializePin(pinNoteIn, static_cast<MpGuiBaseMemberPtr2>(&MIDILearnGui::onSetNoteIn));
		initializePin(pinAccidentals, static_cast<MpGuiBaseMemberPtr2>(&MIDILearnGui::onSetSwitch));
		initializePin(pinNoteOut, static_cast<MpGuiBaseMemberPtr2>(&MIDILearnGui::onSetNoteIn));
	}

	int32_t Note2CharSharps()
	{
		int mi = pinNoteIn;
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

		pinNote = nt3 + oct_;

		return 0;
	}

	int32_t Note2CharFlats()
	{
		int mi = pinNoteIn;
		signed int oct_v = mi / 12 - 1;

		std::string nt3;
		acc = pinAccidentals;
		// with switch
		switch (mi % 12)
		{
		case 0: nt3 = "C"; break;
		case 1: nt3 = "D" + acc; break;
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

		pinNote = nt3 + oct_;

		return 0;
	}

};

namespace
{
	auto r = Register<MIDILearnGui>::withId(L"MIDI Learn2");
}

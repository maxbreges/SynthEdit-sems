#include "mp_sdk_gui2.h"

using namespace gmpi;

class MajorsAscGui final : public SeGuiInvisibleBase
{
	int root = 0;

	void onSetNote()
	{
		pinNote = (pinNoteIn - root + 12) % 12;
	}

	void onSetRoot()
	{
		root = pinRootIn;
	}

 	void onSetMode()
	{
		switch (pinMode)
		{
		case 0: 
		{
			switch (root)
			{
			case 0: pinKeyList = "C,D♭,D,E♭,E,F,F♯,G,A♭,A,B♭,B"; break;
			case 1: pinKeyList = "C♯,D♮,D♯,E♮,E♯,F♯,F𝄪,G♯,A♮,A♯,B♮,B♯"; break;
			case 2: pinKeyList = "D,E♭,E,F♮,F♯,G,G♯,A,B♭,B,C♮,C♯"; break;
			case 3: pinKeyList = "E♭,F♭,F,G♭,G,A♭,A♮,B♭,C♭,C,D♭,D"; break;
			case 4: pinKeyList = "E,F♮,F♯,G♮,G♯,A,A♯,B,C♮,C♯,D♮,D♯"; break;
			case 5: pinKeyList = "F,G♭,G,A♭,A,B♭,B♮,C,D♭,D,E♭,E"; break;
			case 6: pinKeyList = "F♯,G♮,G♯,A♮,A♯,B,B♯,C♯,D♮,D♯,E♮,E♯"; break;
			case 7: pinKeyList = "G,A♭,A,B♭,B,C,C♯,D,E♭,E,F♮,F♯"; break;
			case 8: pinKeyList = "A♭,B𝄫,B♭,C♭,C,D♭,D♮,E♭,F♭,F,G♭,G"; break;
			case 9: pinKeyList = "A,B♭,B,C♮,C♯,D,D♯,E,F♮,F♯,G♮,G♯"; break;
			case 10: pinKeyList = "B♭,C♭,C,D♭,D,E♭,E♮,F,G♭,G,A♭,A "; break;
			case 11: pinKeyList = "B,C♮,C♯,D♮,D♯,E,E♯,F♯,G♮,G♯,A♮,A♯"; break;
			}
			pinDegreeList = "Ⅰ,Ⅱ♭,Ⅱ,Ⅲ♭,Ⅲ,Ⅳ,Ⅳ♯,Ⅴ,Ⅵ♭,Ⅵ,Ⅶ♭,Ⅶ"; break;
		}
		case 1: 
		{
			switch (root)
			{
			case 0: pinKeyList = "C,C♯,D,D♯,E,F,F♯,G,G♯,A,B♭,B"; break;
			case 1: pinKeyList = "C♯,C𝄪,D♯,D𝄪,E♯,F♯,F𝄪,G♯,G𝄪,A♯,B♮,B♯"; break;
			case 2: pinKeyList = "D,D♯,E,E♯,F♯,G,G♯,A,A♯,B,C♮,C♯"; break;
			case 3: pinKeyList = "E♭,E♮,F,F♯,G,A♭,A♮,B♭,B♮,C,D♭,D"; break;
			case 4: pinKeyList = "E,E♯,F♯,F𝄪,G♯,A,A♯,B,B♯,C♯,D♮,D♯"; break;
			case 5: pinKeyList = "F,F♯,G,G♯,A,B♭,B♮,C,C♯,D,E♭,E"; break;
			case 6: pinKeyList = "F♯,F𝄪,G♯,G𝄪,A♯,B,B♯,C♯,C𝄪,D♯,E♮,E♯"; break;
			case 7: pinKeyList = "G,G♯,A,A♯,B,C,C♯,D,D♯,E,F♮,F♯"; break;
			case 8: pinKeyList = "A♭,A♮,B♭,B♮,C,D♭,D♮,E♭,E♮,F,G♭,G"; break;
			case 9: pinKeyList = "A,A♯,B,B♯,C♯,D,D♯,E,E♯,F♯,G♮,G♯"; break;
			case 10: pinKeyList = "B♭,B♮,C,C♯,D,E♭,E♮,F,F♯,G,A♭,A"; break;
			case 11: pinKeyList = "B,B♯,C♯,C𝄪,D♯,E,E♯,F♯,F𝄪,G♯,A♮,A♯"; break;
			}
			pinDegreeList = "Ⅰ, Ⅰ♯, Ⅱ, Ⅱ♯, Ⅲ, Ⅳ, Ⅳ♯, Ⅴ, Ⅴ♯, Ⅵ, Ⅶ♭, Ⅶ"; break;			
		}
		case 2:
		{
			switch (root)
			{
			case 0: pinKeyList = "C,D♭,D,E♭,E♮,F,F♯,G,A♭,A♮,B♭,B♮"; break;
			case 1: pinKeyList = "C♯,D♮,D♯,E,E♯,F♯,F𝄪,G♯,A,A♯,B,B♯"; break;
			case 2: pinKeyList = "D,E♭,E,F,F♯,G,G♯,A,B♭,B♮,C,C♯"; break;
			case 3: pinKeyList = "E♭,F♭,F,G♭,G♮,A♭,A♮,B♭,C♭,C♮,D♭,D♮"; break;
			case 4: pinKeyList = "E,F♮,F♯,G,G♯,A,A♯,B,C,C♯,D,D♯"; break;
			case 5: pinKeyList = "F,G♭,G,A♭,A♮,B♭,B♮,C,D♭,D♮,E♭,E♮"; break;
			case 6: pinKeyList = "F♯,G♮,G♯,A,A♯,B,B♯,C♯,D,D♯,E,E♯"; break;
			case 7: pinKeyList = "G,A♭,A,B♭,B♮,C,C♯,D,E♭,E♮,F,F♯"; break;
			case 8: pinKeyList = "G♯,A♮,A♯,B,B♯,C♯,C𝄪,D♯,E,E♯,F♯,F𝄪"; break;
			case 9: pinKeyList = "A,B♭,B,C,C♯,D,D♯,E,F,F♯,G,G♯"; break;
			case 10: pinKeyList = "B♭,C♭,C,D♭,D♮,E♭,E♮,F,G♭,G♮,A♭,A♮"; break;
			case 11: pinKeyList = "B,C♮,C♯,D,D♯,E,E♯,F♯,G,G♯,A,A♯"; break;
			}
			pinDegreeList = "Ⅰ, Ⅱ♭, Ⅱ, Ⅲ, Ⅲ♯, Ⅳ, Ⅳ♯, Ⅴ, Ⅵ, Ⅵ♯, Ⅶ, Ⅶ♯";	 break;
		}
		}
	}
	
	IntGuiPin pinNoteIn;
	IntGuiPin pinMode;
	IntGuiPin pinRootIn;	 
	IntGuiPin pinNote;
 	StringGuiPin pinKeyList;
	StringGuiPin pinDegreeList;

public:
	MajorsAscGui()
	{
		initializePin(pinNoteIn, static_cast<MpGuiBaseMemberPtr2>(&MajorsAscGui::onSetNote));
		initializePin(pinMode, static_cast<MpGuiBaseMemberPtr2>(&MajorsAscGui::onSetMode));
		initializePin(pinRootIn, static_cast<MpGuiBaseMemberPtr2>(&MajorsAscGui::onSetRoot));				
		initializePin(pinNote);
		initializePin( pinKeyList );	
		initializePin(pinDegreeList);
	}
};

namespace
{
	auto r = Register<MajorsAscGui>::withId(L"majors asc");
}

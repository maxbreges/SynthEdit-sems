#include "mp_sdk_gui2.h"
//#include <numeric>
//#include <vector>

using namespace gmpi;

class MinorsGui final : public SeGuiInvisibleBase
{
	void onSetDelta()
	{
		onSetIndex();
	}
	void onSetScale()
	{
		if (!pinScale)
		{
			pinKeys = "C,C♯,D,E♭,E,F,F♯,G,G♯,A,B♭,B";
		}
		if (pinScale)
		{
			pinKeys = "C,C♯,D,E♭,E,F,F♯,G,A♭,A,B♭,B";
		}
		onSetIndex();
	}
	void onSetKeys()
	{
		onSetScale();
	}

 	void onSetIndex()
	{
		if (!pinScale) //minors both ascending and descending
		{
			switch (pinIndex)
			{
			case 0: pinList = "C,D♭,D,E♭,E♮,F,F♯,G,A♭,A♮,B♭,B♮"; break;
			case 1: pinList = "B♯,C♯,D♮,D♯,E,E♯,F♯,F♯♯,G♯,A,A♯,B"; break;
			case 2: pinList = "C,C♯,D,E♭,E,F,F♯,G,G♯,A,B♭,B♮"; break;
			case 3: pinList = "C♮,D♭,D♮,E♭,F♭,F,G♭,G♮,A♭,A♮,B♭,C♭"; break;
			case 4: pinList = "C,C♯,D,D♯,E,F♮,F♯,G,G♯,A,A♯,B"; break;
			case 5: pinList = "C,D♭,D♮,E♭,E♮,F,G♭,G,A♭,A♮,B♭,B♮"; break;
			case 6: pinList = "B♯,C♯,D,D♯,E,E♯,F♯,G♮,G♯,A,A♯,B"; break;
			case 7: pinList = "C,C♯,D,E♭,E♮,F,F♯,G,A♭,A,B♭,B♮"; break;
			case 8: pinList = "B♯,C♯,C♯♯,D♯,E,E♯,F♯,F♯♯,G♯,A♮,A♯,B"; break;
			case 9: pinList = "C,C♯,D,D♯,E,F,F♯,G,G♯,A,B♭,B"; break;
			case 10: pinList = "C,D♭,D♮,E♭,E♮,F,G♭,G♮,A♭,A♮,B♭,C♭"; break;
			case 11: pinList = "C♮,C♯,D,D♯,E,E♯,F♯,G,G♯,A,A♯,B"; break;
			}
		}

		if (pinScale) //majors
		{
			switch (pinDelta)
			{

			case 0: // descending

				switch (pinIndex)
				{
				case 0: pinList = "C,D♭,D,E♭,E,F,F♯,G,A♭,A,B♭,B"; break; //C major
				case 1: pinList = "B♯,C♯,D♮,D♯,E♮,E♯,F♯,F♯♯,G♯,A♮,A♯,B♮"; break; //C sharp major
				case 2: pinList = "C♮,C♯,D,E♭,E,F♮,F♯,G,G♯,A,B♭,B"; break; //D major
				case 3: pinList = "C,D♭,D,E♭,F♭,F,G♭,G,A♭,A♮,B♭,C♭"; break; // E flat major
				case 4: pinList = "C♮,C♯,D♮,D♯,E,F♮,F♯,G♮,G♯,A,A♯,B"; break; //E major
				case 5: pinList = "C,D♭,D,E♭,E,F,G♭,G,A♭,A,B♭,B♮"; break; //F major
				case 6: pinList = "B♯,C♯,D♮,D♯,E♮,E♯,F♯,G♮,G♯,A♮,A♯,B"; break; // F sharp major
				case 7: pinList = "C,C♯,D,E♭,E,F♮,F♯,G,A♭,A,B♭,B"; break; //G major
				case 8: pinList = "C,D♭,D♮,E♭,F♭,F,G♭,G,A♭,B♭♭,B♭,C♭"; break; //A♭ major
				case 9: pinList = "C♮,C♯,D,D♯,E,F♮,F♯,G♮,G♯,A,B♭,B"; break; //A major
				case 10: pinList = "C,D♭,D,E♭,E♮,F,G♭,G,A♭,A,B♭,C♭"; break; //B♭ major
				case 11: pinList = "C♮,C♯,D♮,D♯,E,E♯,F♯,G♮,G♯,A♮,A♯,B"; break; // B major
				};
				break;

			case 1:
				switch (pinIndex) // ascending
				{
				case 0: pinList = "C,C♯,D,D♯,E,F,F♯,G,G♯,A,B♭,B"; break;
				case 1: pinList = "B♯,C♯,C♯♯,D♯,D♯♯,E♯,F♯,F♯♯,G♯,G♯♯,A♯,B♮"; break;
				case 2: pinList = "C♮,C♯,D,D♯,E,E♯,F♯,G,G♯,A,A♯,B"; break;
				case 3: pinList = "C,D♭,D,E♭,E♮,F,F♯,G,A♭,A♮,B♭,B♮"; break;
				case 4: pinList = "B♯,C♯,D♮,D♯,E,E♯,F♯,F♯♯,G♯,A,A♯,B"; break;
				case 5: pinList = "C,C♯,D,E♭,E,F,F♯,G,G♯,A,B♭,B♮"; break;
				case 6: pinList = "B♯,C♯,C♯♯,D♯,E♮,E♯,F♯,F♯♯,G♯,G♯♯,A♯,B"; break;
				case 7: pinList = "C,C♯,D,D♯,E,F♮,F♯,G,G♯,A,A♯,B"; break;
				case 8: pinList = "C,D♭,D♮,E♭,E♮,F,G♭,G,A♭,A♮,B♭,B♮"; break;
				case 9: pinList = "B♯,C♯,D,D♯,E,E♯,F♯,G♮,G♯,A,A♯,B"; break;
				case 10: pinList = "C,C♯,D,E♭,E♮,F,F♯,G,A♭,A,B♭,B♮"; break;
				case 11: pinList = "B♯,C♯,C♯♯,D♯,E,E♯,F♯,F♯♯,G♯,A♮,A♯,B"; break;
				}; break;						
			}			
		}
	}

	void onSetChar()
	{
		/*std::vector<std::string> acc = {"D\xf0\x90\x84\xAB","\xe2\x9a\x81","\xe2\x9a\x82","\xe2\x9a\x83","\xf0\x90\x84\xAB"};
		
		std::string s = std::accumulate(acc.begin(), acc.end(), std::string{});

		pinChar = s;*/
	}

	IntGuiPin pinDelta;
	IntGuiPin pinScale;
	StringGuiPin pinKeys;
 	IntGuiPin pinIndex;
 	StringGuiPin pinList;
	//StringGuiPin pinChar;

public:
	MinorsGui()
	{
		initializePin(pinDelta, static_cast<MpGuiBaseMemberPtr2>(&MinorsGui::onSetDelta));
		initializePin(pinScale, static_cast<MpGuiBaseMemberPtr2>(&MinorsGui::onSetScale));
		initializePin(pinKeys, static_cast<MpGuiBaseMemberPtr2>(&MinorsGui::onSetKeys));
		initializePin( pinIndex, static_cast<MpGuiBaseMemberPtr2>(&MinorsGui::onSetIndex) );
		initializePin( pinList, static_cast<MpGuiBaseMemberPtr2>(&MinorsGui::onSetScale) );
	//	initializePin(pinChar, static_cast<MpGuiBaseMemberPtr2>(&MinorsGui::onSetChar));
	}
};

namespace
{
	auto r = Register<MinorsGui>::withId(L"minors");
}

#include "mp_sdk_gui2.h"

using namespace gmpi;

class StringRemainderGui final : public SeGuiInvisibleBase
{
 	void onSetString1()
	{
		stringRemainder();
	}

 	void onSetString2()
	{
		stringRemainder();
	}


 	StringGuiPin pinString1;
 	StringGuiPin pinString2;
 	StringGuiPin pinStringOut;

public:
	StringRemainderGui()
	{
		initializePin( pinString1, static_cast<MpGuiBaseMemberPtr2>(&StringRemainderGui::onSetString1) );
		initializePin( pinString2, static_cast<MpGuiBaseMemberPtr2>(&StringRemainderGui::onSetString2) );
		initializePin( pinStringOut );
	}

	void stringRemainder()
	{
		std::string s1 = pinString1;
		std::string s2 = pinString2;

		// Check if s1 is a prefix of s2
		if (s2.compare(0, s1.size(), s1) == 0) {
			// Output the remainder
			std::string remainder = s2.substr(s1.size());
			pinStringOut = remainder; // Output: Apple
		}
		else {
			pinStringOut = pinString2;
		}
	}
};

namespace
{
	auto r = Register<StringRemainderGui>::withId(L"My StringRemainder");
}

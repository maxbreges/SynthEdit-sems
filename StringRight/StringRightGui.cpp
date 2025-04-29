#include "mp_sdk_gui2.h"

using namespace std;
using namespace gmpi;

class StringRightGui final : public SeGuiInvisibleBase
{
	std::string StringIn;
	int StringLength = 0;
	std::string StringOut;

	void onSetStringIn()
	{
		StringIn = pinStringIn;
		if (!pinStringIn.getValue().empty() && StringLength > 0)
		{
			StringOut = StringIn.substr(StringIn.length() - StringLength);
		}
		else
		{
			StringOut.clear(); // Clear output if no length is set or input is empty
		}
		pinStringOut = StringOut;
	}

 	void onSetStringLength()
	{
		StringLength = pinStringLength.getValue();
		onSetStringIn();
	}

	void onSetStringOut()
	{
		// pinFloatVal changed
	}

 	StringGuiPin pinStringIn;
 	IntGuiPin pinStringLength;
	StringGuiPin pinStringOut;

public:
	StringRightGui()
	{
		initializePin(pinStringIn, static_cast<MpGuiBaseMemberPtr2>(&StringRightGui::onSetStringIn) );
		initializePin(pinStringLength, static_cast<MpGuiBaseMemberPtr2>(&StringRightGui::onSetStringLength) );
		initializePin(pinStringOut, static_cast<MpGuiBaseMemberPtr2>(&StringRightGui::onSetStringOut));
	}

};

namespace
{
	auto r = Register<StringRightGui>::withId(L"StringRight");
}

#include "StringRemainderGui.h"

REGISTER_GUI_PLUGIN(StringRemainderGui, L"My StringRemainder");

StringRemainderGui::StringRemainderGui (IMpUnknown* host) : MpGuiBase(host)
,choice(0)

	{
		initializePin( pinString1, static_cast<MpGuiBaseMemberPtr>(&StringRemainderGui::onSetString1) );
		initializePin( pinString2, static_cast<MpGuiBaseMemberPtr>(&StringRemainderGui::onSetString2) );
		initializePin( pinStringOut );
		initializePin(pinSwitch);
	}

	void StringRemainderGui::onSetString1()
	{
		s1 = pinString1;
	}

	void StringRemainderGui::onSetString2()
	{
		s2 = pinString2;
		stringRemainder();
	}

	int32_t StringRemainderGui::receiveMessageFromAudio(int32_t id, int32_t size, void* messageData)
	{
		if (id == 116599)
		stringRemainder();
		return gmpi::MP_OK;
	}

	void StringRemainderGui::stringRemainder()
	{
		// Check if s1 is a prefix of s2

		if (s2.compare(0, s1.size(), s1) == 0) {
			// Output the remainder
			std::string remainder = s2.substr(s1.size());
			pinStringOut = remainder; 
			choice = 1;
			pinSwitch = choice;
		}
		else {

			pinStringOut = "";
			choice = 0;
			pinSwitch = choice;
		}		
	}

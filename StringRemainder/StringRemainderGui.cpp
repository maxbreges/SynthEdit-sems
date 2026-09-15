#include "StringRemainderGui.h"

REGISTER_GUI_PLUGIN(StringRemainderGui, L"My StringRemainder");

StringRemainderGui::StringRemainderGui (IMpUnknown* host) : MpGuiBase(host)
	{
		initializePin( pinString1, static_cast<MpGuiBaseMemberPtr>(&StringRemainderGui::onSetString1) );
		initializePin( pinString2, static_cast<MpGuiBaseMemberPtr>(&StringRemainderGui::onSetString2) );
		initializePin( pinStringOut );
		initializePin(pinRelativePath, static_cast<MpGuiBaseMemberPtr>(&StringRemainderGui::onSetRelativePath));
		initializePin(pinDebug);		
	}

	int32_t StringRemainderGui::receiveMessageFromAudio(int32_t id, int32_t size, void* messageData)
	{
		if (id == 116599)
			//pinDebug = "from Audio";
			onSetString1();		
		return gmpi::MP_OK;
	}

	void StringRemainderGui::onSetString1()
	{
		//pinDebug = "onSetString1";
		s1 = pinString1; //App Directory	
		onSetString2();
	}

	void StringRemainderGui::onSetString2()
	{
		s2 = pinString2; //User Path

		stringRemainder();	
	}
	std::string relativePath;
	void StringRemainderGui::stringRemainder()
	{
		// Check if s1 is a prefix of s2
		if (s2.compare(0, s1.size(), s1) == 0) {
			// Output the remainder
			std::string remainder = s2.substr(s1.size());			
			if (remainder == s2)
			{
				s1 = pinString1;
				pinStringOut = s1 + std::string(pinRelativePath);
				return;
			}
			pinRelativePath = remainder;
			onSetRelativePath();
			pinStringOut = s1 + relativePath; //relative path
			pinDebug = "Check if s1 is a prefix of s2\n" + std::string("String1  ") + s1 + "\n" + "String2  " + s2 + "\n" + remainder;
		}
		else 
		{
			pinStringOut = s2;
			pinDebug = "else part";
		}		
	}
	
	void StringRemainderGui::onSetRelativePath()
	{
		relativePath = pinRelativePath;
	}
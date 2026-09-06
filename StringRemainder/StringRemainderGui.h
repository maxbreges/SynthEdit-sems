#pragma once
#ifndef STRINGREMAINDERGUI_H_INCLUDED
#define STRINGREMAINDERGUI_H_INCLUDED

#include "mp_sdk_gui2.h"

using namespace gmpi;

class StringRemainderGui : public MpGuiBase
{
public:
	StringRemainderGui(IMpUnknown* host);

	// overrides
	virtual int32_t MP_STDCALL receiveMessageFromAudio(int32_t id, int32_t size, void* messageData);
	void onSetString1();
	void onSetString2();
	void stringRemainder();

	StringGuiPin pinString1;
	StringGuiPin pinString2;
	StringGuiPin pinStringOut;
	IntGuiPin pinSwitch;

private:
	int choice;
	std::string s1;
	std::string s2;
};

#endif

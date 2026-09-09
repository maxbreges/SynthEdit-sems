#pragma once
#ifndef TEXTGUITODSPGUI_H_INCLUDED
#define TEXTGUITODSPGUI_H_INCLUDED

#include "mp_sdk_gui2.h"

using namespace gmpi;

class TextGuiToDSPGui : public MpGuiBase
{
public:
	TextGuiToDSPGui (IMpUnknown* host);

	// overrides
	virtual int32_t MP_STDCALL receiveMessageFromAudio(int32_t id, int32_t size, void* messageData);
	void onSetStringIn();

	StringGuiPin pinStringIn;
	StringGuiPin pinStringOut; //debug pin

	std::string a;
};

#endif



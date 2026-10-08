#pragma once
#ifndef RESOLVEFILENAMEGUI_H_INCLUDED
#define RESOLVEFILENAMEGUI_H_INCLUDED

#include "mp_sdk_gui2.h"

using namespace gmpi;

class ResolveFilenameGui : public MpGuiBase
{
public:
	ResolveFilenameGui(IMpUnknown* host);

	// overrides
	virtual int32_t MP_STDCALL receiveMessageFromAudio(int32_t id, int32_t size, void* messageData);;
	void onSetString();

	StringGuiPin pinFromParameter;
	StringGuiPin pinCommonPath;
	BoolGuiPin pinBool;
};

#endif
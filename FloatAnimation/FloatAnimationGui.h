#pragma once
#ifndef FLOATANIMATIONGUI_H_INCLUDED
#define FLOATANIMATIONGUI_H_INCLUDED

#include "mp_sdk_gui2.h"

using namespace gmpi;

class FloatAnimationGui : public MpGuiBase
{
public:
	FloatAnimationGui(IMpUnknown* host);

	// overrides
	virtual int32_t MP_STDCALL receiveMessageFromAudio(int32_t id, int32_t size, void* messageData);
	void onSetOnOff();
	void onSetSpeed();

private:
	BoolGuiPin pinOnOff;
	FloatGuiPin pinSpeed;
	FloatGuiPin pinAnimPos;

	float speed;
	float mult;

};

#endif

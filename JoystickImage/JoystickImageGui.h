#ifndef JOYSTICKIMAGEGUI_H_INCLUDED
#define JOYSTICKIMAGEGUI_H_INCLUDED

#include "../sharedLegacyWidgets/ImageBase.h"
#include "mp_sdk_gui2.h"

class JoystickImageGui : public ImageBase
{
	FloatGuiPin pinAnimationPosition;
	IntGuiPin pinFrameCount;
	IntGuiPin pinFrameCountLegacy;
	BoolGuiPin pinJumpToMouse;
	FloatGuiPin pinPositionX;
	FloatGuiPin pinPositionY;
	BoolGuiPin pinMouseDown2;
	FloatGuiPin pinResetValue;
	BoolGuiPin pinMouseOver;
	StringGuiPin pinHintOut;

public:
	JoystickImageGui();
	
	virtual float getAnimationPos() override {
		return pinAnimationPosition;
	}
	virtual void setAnimationPos(float p) override;
	virtual void onLoaded() override;
	virtual std::wstring getHint() override

	{
		return pinHint;
	}
	// overrides.
	int32_t MP_STDCALL setHover(bool isMouseOverMe) override;
	virtual int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override;
	virtual int32_t MP_STDCALL onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point) override;
	virtual int32_t MP_STDCALL onPointerMove(int32_t flags, GmpiDrawing_API::MP1_POINT point) override;
	virtual int32_t MP_STDCALL onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{
		auto r = ImageBase::onPointerUp(flags, point);
		pinMouseDown2 = false;
		return r;
	}

	// MP_OK = hit, MP_UNHANDLED/MP_FAIL = miss.
	// Default to MP_OK to allow user to select by clicking.
	// point will always be within bounding rect.
	virtual int32_t MP_STDCALL hitTest(GmpiDrawing_API::MP1_POINT point) override;

};

#endif



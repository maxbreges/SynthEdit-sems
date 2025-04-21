#ifndef FloatEntryGUI_H_INCLUDED
#define FloatEntryGUI_H_INCLUDED

#include "TextSubcontrol.h"
#include "FontCache.h"
/*#include <Windows.h>*/

class FloatEntryGui : public TextSubcontrol
{
    GmpiGui::TextEdit nativeEdit;

public:
    FloatEntryGui();

    void onSetStyle();
    int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override;
    int32_t MP_STDCALL onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point) override;
    int32_t MP_STDCALL onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point) override;
/*    int32_t MP_STDCALL onPointerMove(int32_t flags, GmpiDrawing_API::MP1_POINT point);*/
    virtual int32_t MP_STDCALL arrange(GmpiDrawing_API::MP1_RECT finalRect) override;
    virtual int32_t MP_STDCALL measure(GmpiDrawing_API::MP1_SIZE availableSize, GmpiDrawing_API::MP1_SIZE* returnDesiredSize) override;

    std::string getDisplayText() override;

private:
   /* POINT lastDragPoint;  // Use the namespaced structure*/
    void OnTextEnteredComplete(int32_t result);
    void onSetText();
    void redraw();
    void onSetWidth();

    StringGuiPin pinText; // Ensure 'pinText' is a member variable.
    BoolGuiPin pinMultiline;
    BoolGuiPin pinMouseDown_LEGACY;
    BoolGuiPin pinMouseDown;
    FloatGuiPin pinFloatIn;
    FloatGuiPin pinFloatOut;
    FloatGuiPin pinFloat;
    StringGuiPin pinString;
    IntGuiPin pinWidth;

    GmpiDrawing::TextFormat_readonly textFormat;
    FontMetadata* fontmetadata = nullptr;
};

#endif // FloatEntryGUI_H_INCLUDED
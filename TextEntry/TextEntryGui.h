#ifndef TEXTENTRYGUI_H_INCLUDED
#define TEXTENTRYGUI_H_INCLUDED

//#include <functional>
#include "TextSubcontrol.h"
#include "FontCache.h"

class TextXGui : public TextSubcontrol
{
    GmpiGui::TextEdit nativeEdit;

public:
    TextXGui();

    void onSetStyle();
    int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override;
    int32_t MP_STDCALL onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point) override;
    int32_t MP_STDCALL onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point) override;

    std::string getDisplayText() override;

private:

    void OnTextEnteredComplete(int32_t result);
    void onSetTextColor();
    void onSetTopColor();
    void onSetBgColor();
    void redraw();
/*    void onSetFontSize();
    void onSetFont();
    void onSetTextAlignment();*/

    StringGuiPin pinText; // Ensure 'pinText' is a member variable.
    StringGuiPin pinTextColor;
    StringGuiPin pinTopColor;
    StringGuiPin pinBgColor;
    BoolGuiPin pinWrapping;
    BoolGuiPin pinMouseDown;

/*    IntGuiPin pinFontSize;
    StringGuiPin pinFont;
    IntGuiPin pinAlignV;*/

    GmpiDrawing::TextFormat_readonly textFormat;
    FontMetadata* fontmetadata = nullptr;
};

#endif // TEXTXGUI_H_INCLUDED
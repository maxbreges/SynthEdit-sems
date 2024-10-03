#include "GuiSwitches.h"
#include <string>

using namespace gmpi;

typedef GuiSwitch<bool> GuiSwitchBool;
typedef GuiSwitch<float> GuiSwitchFloat;
typedef GuiSwitch<MpBlob> GuiSwitchBlob;
typedef GuiSwitch<int32_t> GuiSwitchInt;
typedef GuiSwitch<std::wstring> GuiSwitchText;

GMPI_REGISTER_GUI(MP_SUB_TYPE_GUI2, GuiSwitchBool, L"GuiSwitch > (Bool)");
GMPI_REGISTER_GUI(MP_SUB_TYPE_GUI2, GuiSwitchInt, L"GuiSwitch > (Float)");
GMPI_REGISTER_GUI(MP_SUB_TYPE_GUI2, GuiSwitchBlob, L"GuiSwitch > (Blob)");
GMPI_REGISTER_GUI(MP_SUB_TYPE_GUI2, GuiSwitchInt, L"GuiSwitch > (Int)");
GMPI_REGISTER_GUI(MP_SUB_TYPE_GUI2, GuiSwitchText, L"GuiSwitch > (Text)");

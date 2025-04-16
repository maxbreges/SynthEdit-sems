#include "./TextAppendGui.h"
#include <string>

using namespace gmpi;

typedef TextAppend<std::wstring> GuiSwitchText;

GMPI_REGISTER_GUI(MP_SUB_TYPE_GUI2, GuiSwitchText, L"TextAppendGuiR");



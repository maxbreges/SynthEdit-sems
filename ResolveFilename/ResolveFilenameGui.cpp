#include "ResolveFileNameGui.h"

REGISTER_GUI_PLUGIN (ResolveFilenameGui, L"resolveFilename");

ResolveFilenameGui::ResolveFilenameGui (IMpUnknown* host) : MpGuiBase(host)
{
    initializePin(pinFromParameter, static_cast<MpGuiBaseMemberPtr>(&ResolveFilenameGui::onSetString));
    initializePin(pinCommonPath);
        initializePin(pinBool);
}
void ResolveFilenameGui::onSetString()
{
    pinCommonPath = pinFromParameter;
}

int32_t ResolveFilenameGui::receiveMessageFromAudio(int32_t id, int32_t size, void* messageData)
{
    pinBool = true;
    pinBool = false;
    return gmpi::MP_OK;
}
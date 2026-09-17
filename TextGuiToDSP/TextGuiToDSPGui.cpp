#include "TextGuiToDSPGui.h"

REGISTER_GUI_PLUGIN (TextGuiToDSPGui, L"My TextGuiToDSP");

TextGuiToDSPGui::TextGuiToDSPGui(IMpUnknown* host) : MpGuiBase(host)

	{
		initializePin(pinStringIn, static_cast<MpGuiBaseMemberPtr>(&TextGuiToDSPGui::onSetStringIn) );	
		initializePin(pinStringOut);
	}

	int32_t TextGuiToDSPGui::receiveMessageFromAudio(int32_t id, int32_t size, void* messageData)
	{
		if (id == 119599)
			onSetStringIn();
		return gmpi::MP_OK;
	}

	void TextGuiToDSPGui::onSetStringIn()
	{
		auto messageData = pinStringIn.getValue();
		messageData.push_back(L'\0');
		size_t sizeInBytes = messageData.size() * sizeof(wchar_t);
		int32_t sizeInBytesInt32 = static_cast<int32_t>(sizeInBytes);
		getHost()->sendMessageToAudio(119234, sizeInBytesInt32, messageData.data());
	}

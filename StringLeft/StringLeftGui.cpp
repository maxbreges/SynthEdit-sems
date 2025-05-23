#include "mp_sdk_gui2.h"

using namespace gmpi;

class StringLeftGui final : public SeGuiInvisibleBase
{
    std::string StringIn;
    int StringLength = 0;
    std::string StringOut;

    void onSetStringIn()
    {
        StringIn = pinStringIn;
        if (!pinStringIn.getValue().empty() && StringLength > 0)
        {
            // Extract the left part of the string based on StringLength
            StringOut = StringIn.substr(0, StringLength);
        }
        else
        {
            StringOut.clear(); // Clear output if no length is set or input is empty
        }
        pinStringOut = StringOut;
    }

    void onSetStringLength()
    {
        StringLength = pinStringLength.getValue();
        onSetStringIn();
    }

    void onSetStringOut()
    {
        // pinStringOut changed
    }

    StringGuiPin pinStringIn;
    IntGuiPin pinStringLength;
    StringGuiPin pinStringOut;

public:
    StringLeftGui()
    {
        initializePin(pinStringIn, static_cast<MpGuiBaseMemberPtr2>(&StringLeftGui::onSetStringIn));
        initializePin(pinStringLength, static_cast<MpGuiBaseMemberPtr2>(&StringLeftGui::onSetStringLength));
        initializePin(pinStringOut, static_cast<MpGuiBaseMemberPtr2>(&StringLeftGui::onSetStringOut));
    }
};

namespace
{
    auto r = Register<StringLeftGui>::withId(L"StringLeft");
}
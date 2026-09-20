#include "mp_sdk_gui2.h"

using namespace gmpi;

class SubStringGui final : public SeGuiInvisibleBase
{
    std::string s1;
    std::string s2;
    std::string subString;

    void onSetS2()
    {
        s2 = pinString2;
        if(s1.empty())
        {
            pinString1 = s2 + subString;
        }        
    }

    void onSetSubString()
    {
        s1 = pinString1;
        s2 = pinString2;

        if (s1.compare(0, s2.size(), s2) == 0)
        {
            // Output the remainder
            subString = s1.substr(s2.size());
        }
        else
        {
            subString = s1;
        }

        pinSubString = subString;
    }

    StringGuiPin pinString1;
    StringGuiPin pinString2;
    StringGuiPin pinSubString;

public:
    SubStringGui()
    {
        initializePin(pinString1, static_cast<MpGuiBaseMemberPtr2>(&SubStringGui::onSetSubString));
        initializePin(pinString2, static_cast<MpGuiBaseMemberPtr2>(&SubStringGui::onSetS2));
        initializePin(pinSubString, static_cast<MpGuiBaseMemberPtr2>(&SubStringGui::onSetPatchValue));
    }
    void onSetPatchValue()
    {
        subString = pinSubString;
        pinString1 = s2 + subString;
    }
};

namespace
{
    auto r = Register<SubStringGui>::withId(L"My SubString");
}
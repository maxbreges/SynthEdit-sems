#include "mp_sdk_gui2.h"

using namespace gmpi;

class SubStringGui final : public SeGuiInvisibleBase
{
    std::string s1;
    std::string s2;
    std::string subString;

    void onSetS1()
    {
        s1 = pinString1;
        s2 = pinString2;

        if (s1.empty())
        {
            onSetResetIn();            
        }

        if (s1.compare(0, s2.size(), s2) == 0)
        {
            // Output the remainder
            subString = s1.substr(s2.size());

            pinSubString = s2 + subString;
        }
        if (s1.compare(0, s2.size(), s2) != 0)
        {
            pinSubString = s1;
        }
    }

    void onSetS2()
    {
        if(s1.empty())
        {
          pinString1 = s2 + subString;
        }
    }

    StringGuiPin pinString1;
    StringGuiPin pinString2;
    StringGuiPin pinSubString;
    BoolGuiPin pinResetIn;

public:
    SubStringGui()
    {
        initializePin(pinString1, static_cast<MpGuiBaseMemberPtr2>(&SubStringGui::onSetS1));
        initializePin(pinString2, static_cast<MpGuiBaseMemberPtr2>(&SubStringGui::onSetS2));
        initializePin(pinSubString, static_cast<MpGuiBaseMemberPtr2>(&SubStringGui::onSetPatchValue));
        initializePin(pinResetIn, static_cast<MpGuiBaseMemberPtr2>(&SubStringGui::onSetResetIn));
    }
    void onSetPatchValue()
    {
    }

    void onSetResetIn()
    {
        pinString1 = pinSubString;
    }
};

namespace
{
    auto r = Register<SubStringGui>::withId(L"My SubString");
}
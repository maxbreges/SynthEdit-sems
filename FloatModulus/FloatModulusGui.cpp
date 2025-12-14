#include "mp_sdk_gui2.h"

using namespace gmpi;

class FloatModulusGui final : public SeGuiInvisibleBase
{
    void updateOutput()
    {
        // Check if all pins are connected/initialized before calculation
        
        
            double sum = pinValue1.getValue() + pinValue2.getValue();
            double modulus = pinModulus.getValue();

            if (modulus != 0)
            {
                double result = fmod(sum, modulus);
                pinValueOut = result;
            }
            else
            {
                pinValueOut = sum;
            }
        
    }

    void onSetValue1()
    {
        updateOutput();
    }

    void onSetValue2()
    {
        updateOutput();
    }

    void onSetModulus()
    {
        updateOutput();
    }

    void onSetValueOut()
    {
        // Typically, output pin change doesn't trigger calculation
        // but you can leave it empty or handle if needed
    }

    FloatGuiPin pinValue1;
    FloatGuiPin pinValue2;
    FloatGuiPin pinModulus;
    FloatGuiPin pinValueOut;

public:
    FloatModulusGui()
    {
        initializePin(pinValue1, static_cast<MpGuiBaseMemberPtr2>(&FloatModulusGui::onSetValue1));
        initializePin(pinValue2, static_cast<MpGuiBaseMemberPtr2>(&FloatModulusGui::onSetValue2));
        initializePin(pinModulus, static_cast<MpGuiBaseMemberPtr2>(&FloatModulusGui::onSetModulus));
        initializePin(pinValueOut);
    }
};

namespace
{
    auto r = Register<FloatModulusGui>::withId(L"FloatModulus");
}
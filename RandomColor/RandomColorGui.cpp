#include "mp_sdk_gui2.h"
#include <sstream>
#include <iomanip>
#include <cstdlib> // for rand()

using namespace gmpi;

class RandomColorGui final : public SeGuiInvisibleBase
{
    void onSetBool()
    {
        if (pinBool)
        {
            // Generate a random 32-bit number
            uint32_t randomColor = (static_cast<uint32_t>(rand()) << 9) | rand();

            // Convert to hex string with leading zeros
            std::stringstream ss;
            ss << std::hex << std::setfill('0') << std::setw(6) << randomColor;

            // Assign to pinColorHex
            pinColorHex = "FF" + ss.str();
        }
    }

 	BoolGuiPin pinBool;
 	StringGuiPin pinColorHex;

public:
	RandomColorGui()
	{
		initializePin( pinBool, static_cast<MpGuiBaseMemberPtr2>(&RandomColorGui::onSetBool) );
		initializePin( pinColorHex);
	}
};

namespace
{
	auto r = Register<RandomColorGui>::withId(L"RandomColor");
}

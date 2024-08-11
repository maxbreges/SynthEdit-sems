#include "mp_sdk_gui2.h"
#include <iomanip>
#include <sstream>

using namespace gmpi;
using namespace std;

class FloatToHexGui final : public SeGuiInvisibleBase
{
 	
 	void onSetText()
	{	
	}

 	FloatGuiPin pinFloat;
 	StringGuiPin pinText;
	IntGuiPin pinInt;
	
public:
	FloatToHexGui()
	{
		initializePin( pinFloat, static_cast<MpGuiBaseMemberPtr2>(&FloatToHexGui::onSetFloat) );
		initializePin( pinText, static_cast<MpGuiBaseMemberPtr2>(&FloatToHexGui::onSetText) );
		initializePin(pinInt, static_cast<MpGuiBaseMemberPtr2>(&FloatToHexGui::onSetFloat));
	}

	void onSetFloat()
	{
		pinInt = pinFloat;

		std::stringstream ss;
		ss << std::setfill('0') << std::setw(sizeof(int) - 2) << std::hex << pinInt;
		std::string res(ss.str());

		pinText = res;
	}
	
};

namespace
{
	auto r = Register<FloatToHexGui>::withId(L"FloatToHex");
}

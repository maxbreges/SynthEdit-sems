#include "mp_sdk_gui2.h"
#include <iomanip>
#include <sstream>

using namespace gmpi;
using namespace std;

class FloatToHexGui final : public SeGuiInvisibleBase
{
 	void onSetFloat()
	{		
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(sizeof(int)-2) << std::hex << pinFloat;
		std::string res(ss.str());

		pinText = res;	
	}

 	void onSetText()
	{	
	}

 	FloatGuiPin pinFloat;
 	StringGuiPin pinText;

public:
	IntToHexGui()
	{
		initializePin( pinFloat, static_cast<MpGuiBaseMemberPtr2>(&IntToHexGui::onSetFloat) );
		initializePin( pinText, static_cast<MpGuiBaseMemberPtr2>(&IntToHexGui::onSetText) );
	}

};

namespace
{
	auto r = Register<FloatToHexGui>::withId(L"Float to Hex");
}

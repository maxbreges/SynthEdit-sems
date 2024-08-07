#include "mp_sdk_gui2.h"
#include <iomanip>
#include <sstream>

using namespace gmpi;
using namespace std;

class IntToHexGui final : public SeGuiInvisibleBase
{
 	void onSetInt()
	{		
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(sizeof(int)-2) << std::hex << pinInt;
		std::string res(ss.str());

		pinText = res;	
	}

 	void onSetText()
	{	
	}

 	IntGuiPin pinInt;
 	StringGuiPin pinText;

public:
	IntToHexGui()
	{
		initializePin( pinInt, static_cast<MpGuiBaseMemberPtr2>(&IntToHexGui::onSetInt) );
		initializePin( pinText, static_cast<MpGuiBaseMemberPtr2>(&IntToHexGui::onSetText) );
	}

};

namespace
{
	auto r = Register<IntToHexGui>::withId(L"Int to Hex");
}

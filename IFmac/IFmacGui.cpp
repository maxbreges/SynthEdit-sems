#include "mp_sdk_gui2.h"

using namespace gmpi;

class IFmacGui final : public SeGuiInvisibleBase
{
 	void onSetVal()
	{

#ifdef _WIN32
		pinVal = -1;
#else
		pinVal = 10;
#endif
		
	}

 	IntGuiPin pinVal;

public:
	IFmacGui()
	{
		initializePin( pinVal, static_cast<MpGuiBaseMemberPtr2>(&IFmacGui::onSetVal) );
	}

};

namespace
{
	auto r = Register<IFmacGui>::withId(L"Ifmac");
}

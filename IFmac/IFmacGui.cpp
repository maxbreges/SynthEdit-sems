#include "mp_sdk_gui2.h"

using namespace gmpi;

class IFmacGui final : public SeGuiInvisibleBase
{
 	void onSetVal()
	{
#ifdef _MAC

		pinVal = 10;

#endif // !WIN32

		
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

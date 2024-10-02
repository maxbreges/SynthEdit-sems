#include "mp_sdk_gui2.h"

using namespace gmpi;

template<class T>
class GenericSplitterGui : public gmpi_gui::MpGuiInvisibleBase
{
	MpGuiPin<T> pinOut;
	std::vector< std::unique_ptr< MpGuiPin<T> > > pinIns;
	static const int fixedPinCount = 1;
	int autoPinCount;

public:
	GenericSplitterGui() :
		autoPinCount(0)
	{
		initializePin(pinOut, static_cast<MpGuiBaseMemberPtr2>(&GenericSplitterGui::onSetOut));
	}

	void onSetOut()
	{
		for (auto& p : pinIns)
		{
			*p = pinOut;
		}
	}
	virtual int32_t MP_STDCALL setPin(int32_t pinId, int32_t voice, int32_t size, const void* data) override
	{
		int plugIndex = pinId - fixedPinCount; // Calc index of autoduplicating pin.

		// Add autoduplicate pins as needed.
		while ((int)pinIns.size() < plugIndex + 1)
		{
			const int pinId = pinIns.size() + fixedPinCount;
			pinIns.push_back(std::make_unique< MpGuiPin<T> >());
			initializePin(pinId, *pinIns.back());
			autoPinCount = pinIns.size();
		}

		auto result = MpGuiBase2::setPin(pinId, voice, size, data);

		if (autoPinCount > 0 && pinId >= fixedPinCount)
		{
			pinOut = *pinIns[plugIndex];
			onSetOut();
		}

		return result;
	}
};

class IntSplitterXGui : public GenericSplitterGui<int>
{
};
namespace
{
	auto r = Register<IntSplitterXGui>::withId(L"IntSplitterX");
}







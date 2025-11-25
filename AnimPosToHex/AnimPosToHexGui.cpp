#include "mp_sdk_gui2.h"
#include <iomanip>
#include <sstream>

using namespace gmpi;
using namespace std;

const int HEX_WIDTH = 2; // Width for hex color string

class PatchMemoryFloatGui final : public SeGuiInvisibleBase
{
	void onSetScaleIn()
	{
		pinScale = pinScaleIn;
	}
	void onSetAnimationIn()
	{
		pinAnimationPosition = pinAnimationIn;
	}
	void onSetScale()
	{
		pinScaleIn = pinScale;
	}
	void onSetAnimationPosition()
	{
		pinAnimationIn = pinAnimationPosition;

		int x = pinAnimationPosition * 1535;
		int R, G, B;

		/*std::stringstream stream;
		stream << std::hex << x;
		std::string result(stream.str());

		pinHexIn = result;*/

		//--------------------------------

		if ((x >= 255) && (x < 768))
			G = 255;
		if (x > 1023)
			G = 0;

		if ((x >= 1279) || (x < 255))
			R = 255;
		if ((x > 510) && (x <= 1024))
			R = 0;

		if ((x >= 767) && (x < 1280))
			B = 255;
		if (x <= 511)
			B = 0;

		//-----------------------------

		if ((x >= 0) && (x <= 255))
		{
			G = x;
		}
		//-------------------

		if ((x >= 256) && (x <= 511))
		{
			R = 255 - (x - 256);
		}

		//-------------------
		if ((x >= 512) && (x <= 767))
		{
			B = x - 512;
		}

		//-------------------
		if ((x >= 768) && (x <= 1023))
		{
			G = 255 - (x - 768);
		}

		//-------------------

		if ((x >= 1024) && (x <= 1279))
		{
			R = x - 1024;
		}

		//-------------------
		if ((x >= 1280) && (x <= 1535))
		{
			B = 255 - (x - 1280);
		}

		std::stringstream ssR, ssG, ssB;
		ssR << std::setfill('0') << std::setw(HEX_WIDTH) << std::hex << R;
		ssG << std::setfill('0') << std::setw(HEX_WIDTH) << std::hex << G;
		ssB << std::setfill('0') << std::setw(HEX_WIDTH) << std::hex << B;

		pinHexIn = "ff" + ssR.str() + ssG.str() + ssB.str();

	}
	void onSetHexIn()
	{
		pinHex = pinHexIn;
	}
	void onSetHex()
	{
		pinHexIn = pinHex;
	}

	FloatGuiPin pinScaleIn;
	FloatGuiPin pinAnimationIn;
	FloatGuiPin pinScale;
	FloatGuiPin pinAnimationPosition;
	StringGuiPin pinHexIn;
	StringGuiPin pinHex;

public:
	PatchMemoryFloatGui()
	{
		initializePin(pinScaleIn, static_cast<MpGuiBaseMemberPtr2>(&PatchMemoryFloatGui::onSetScaleIn));
		initializePin(pinAnimationIn, static_cast<MpGuiBaseMemberPtr2>(&PatchMemoryFloatGui::onSetAnimationIn));
		initializePin(pinScale, static_cast<MpGuiBaseMemberPtr2>(&PatchMemoryFloatGui::onSetScale));
		initializePin(pinAnimationPosition, static_cast<MpGuiBaseMemberPtr2>(&PatchMemoryFloatGui::onSetAnimationPosition));
		initializePin(pinHexIn, static_cast<MpGuiBaseMemberPtr2>(&PatchMemoryFloatGui::onSetHexIn));
		initializePin(pinHex, static_cast<MpGuiBaseMemberPtr2>(&PatchMemoryFloatGui::onSetHex));
	}
};

namespace
{
	auto r = Register<PatchMemoryFloatGui>::withId(L"AnimPos to Hex");
}


/*namespace
{
	auto r = Register<AnimPosToHexGui>::withId(L"AnimPos to Hex");
}*/

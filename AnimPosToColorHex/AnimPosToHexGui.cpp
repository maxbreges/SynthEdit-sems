// Copyright 2025 Max Brezhestovski

#include "mp_sdk_gui2.h"
#include <iomanip>
#include <sstream>
#include <cmath> // for roundf

using namespace gmpi;
using namespace std;

class AnimPosToHex final : public SeGuiInvisibleBase
{
	void onSetAnimationPosition()
	{
		int x = static_cast<int>(roundf(pinAnimationPosition * 1535));
		int R=0, G=0, B=0;

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
		ssR << std::setfill('0') << std::setw(2) << std::hex << R;
		ssG << std::setfill('0') << std::setw(2) << std::hex << G;
		ssB << std::setfill('0') << std::setw(2) << std::hex << B;

		pinHex = "ff" + ssR.str() + ssG.str() + ssB.str();
	}

    FloatGuiPin pinAnimationPosition;
    StringGuiPin pinHex;

public:
    AnimPosToHex()
    {
        initializePin(pinAnimationPosition, static_cast<MpGuiBaseMemberPtr2>(&AnimPosToHex::onSetAnimationPosition));
        initializePin(pinHex);
    }
};

namespace
{
    auto r = Register<AnimPosToHex>::withId(L"AnimPosToHex");
}
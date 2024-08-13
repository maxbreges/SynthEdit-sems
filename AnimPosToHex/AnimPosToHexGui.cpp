#include "mp_sdk_gui2.h"
#include <iomanip>
#include <sstream>

using namespace gmpi;
using namespace std;

class AnimPosToHexGui final : public SeGuiInvisibleBase
{
 	void onSetAnimPos()
	{
		pinValueIn = pinAnimPos * 1535;

		//--------------------------------

		if ((pinValueIn >= 255) && (pinValueIn < 768))
			pinG = 255;
		if (pinValueIn > 1023)
			pinG = 0;

		if ((pinValueIn >= 1279) || (pinValueIn < 255))
			pinR = 255;
		if ((pinValueIn > 510) && (pinValueIn <= 1024))
			pinR = 0;

		if ((pinValueIn >= 767) && (pinValueIn < 1280))
			pinB = 255;
		if (pinValueIn <= 511)
			pinB = 0;


		//-----------------------------

		if ((pinValueIn >= 0) && (pinValueIn <= 255))
		{
			pinG = pinValueIn;
		}
		//-------------------

		if ((pinValueIn >= 256) && (pinValueIn <= 511))
		{
			pinR = 255 - (pinValueIn - 256);
		}

		//-------------------
		if ((pinValueIn >= 512) && (pinValueIn <= 767))
		{
			pinB = pinValueIn - 512;
		}

		//-------------------
		if ((pinValueIn >= 768) && (pinValueIn <= 1023))
		{
			pinG = 255 - (pinValueIn - 768);
		}

		//-------------------

		if ((pinValueIn >= 1024) && (pinValueIn <= 1279))
		{
			pinR = pinValueIn - 1024;
		}

		//-------------------
		if ((pinValueIn >= 1280) && (pinValueIn <= 1535))
		{
			pinB = 255 - (pinValueIn - 1280);
		}

		std::stringstream ssR;
		ssR << std::setfill('0') << std::setw(sizeof(int) - 2) << std::hex << pinR;
		std::string resR(ssR.str());

		R = resR;

		std::stringstream ssG;
		ssG << std::setfill('0') << std::setw(sizeof(int) - 2) << std::hex << pinG;
		std::string resG(ssG.str());

		G = resG;

		std::stringstream ssB;
		ssB << std::setfill('0') << std::setw(sizeof(int) - 2) << std::hex << pinB;
		std::string resB(ssB.str());

		B = resB;

		pinHex = R.getValue() + G.getValue() + B.getValue();

	}

 	void onSetHex()
	{
		// pinHex changed
	}


 	FloatGuiPin pinAnimPos;
 	StringGuiPin pinHex;
	FloatGuiPin pinValueIn;
	IntGuiPin pinR;
	IntGuiPin pinG;
	IntGuiPin pinB;
	StringGuiPin R;
	StringGuiPin G;
	StringGuiPin B;

public:
	AnimPosToHexGui()
	{
		initializePin( pinAnimPos, static_cast<MpGuiBaseMemberPtr2>(&AnimPosToHexGui::onSetAnimPos) );
		initializePin( pinHex, static_cast<MpGuiBaseMemberPtr2>(&AnimPosToHexGui::onSetHex) );
		initializePin(pinValueIn, static_cast<MpGuiBaseMemberPtr2>(&AnimPosToHexGui::onSetAnimPos) );
		initializePin(pinR, static_cast<MpGuiBaseMemberPtr2>(&AnimPosToHexGui::onSetAnimPos));
		initializePin(pinG, static_cast<MpGuiBaseMemberPtr2>(&AnimPosToHexGui::onSetAnimPos));
		initializePin(pinB, static_cast<MpGuiBaseMemberPtr2>(&AnimPosToHexGui::onSetAnimPos));
		initializePin(R, static_cast<MpGuiBaseMemberPtr2>(&AnimPosToHexGui::onSetAnimPos));
		initializePin(G, static_cast<MpGuiBaseMemberPtr2>(&AnimPosToHexGui::onSetAnimPos));
		initializePin(B, static_cast<MpGuiBaseMemberPtr2>(&AnimPosToHexGui::onSetAnimPos));
	}
};

namespace
{
	auto r = Register<AnimPosToHexGui>::withId(L"AnimPos to Hex");
}

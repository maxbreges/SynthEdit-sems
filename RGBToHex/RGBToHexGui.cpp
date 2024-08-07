#include "mp_sdk_gui2.h"
#include <iomanip>
#include <sstream>

using namespace gmpi;
using namespace std;

class RGBToHexGui final : public SeGuiInvisibleBase
{	
	FloatGuiPin pinRin;
	FloatGuiPin pinGin;
	FloatGuiPin pinBin;
	StringGuiPin pinAlpha;
 	StringGuiPin pinR;
	StringGuiPin pinG;
	StringGuiPin pinB;
	StringGuiPin pinHex;	

public:

	RGBToHexGui()
	{
		initializePin(pinRin, static_cast<MpGuiBaseMemberPtr2>(&RGBToHexGui::onSetR));
		initializePin(pinGin, static_cast<MpGuiBaseMemberPtr2>(&RGBToHexGui::onSetG));
		initializePin(pinBin, static_cast<MpGuiBaseMemberPtr2>(&RGBToHexGui::onSetB));
		initializePin(pinAlpha, static_cast<MpGuiBaseMemberPtr2>(&RGBToHexGui::onSetHex));
		initializePin(pinR, static_cast<MpGuiBaseMemberPtr2>(&RGBToHexGui::onSetR));
		initializePin(pinG, static_cast<MpGuiBaseMemberPtr2>(&RGBToHexGui::onSetG));
		initializePin(pinB, static_cast<MpGuiBaseMemberPtr2>(&RGBToHexGui::onSetB));
		initializePin(pinHex, static_cast<MpGuiBaseMemberPtr2>(&RGBToHexGui::onSetHex));
	}

	void onSetR()
	{
		int R = pinRin;		
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(sizeof(int) - 2) << std::hex << R;
		std::string resR(ss.str());		
		pinR = resR;	

		pinHex = pinAlpha.getValue() + pinR.getValue() + pinG.getValue() + pinB.getValue();
	}

	void onSetG()
	{
		int G = pinGin;
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(sizeof(int) - 2) << std::hex << G;
		std::string resG(ss.str());
		pinG = resG;	

		pinHex = pinAlpha.getValue() + pinR.getValue() + pinG.getValue() + pinB.getValue();
	}

	void onSetB()
	{
		int B = pinBin;
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(sizeof(int) - 2) << std::hex << B;
		std::string resB(ss.str());
		pinB = resB;	

		pinHex = pinAlpha.getValue() + pinR.getValue() + pinG.getValue() + pinB.getValue();
	}

	void onSetHex()
	{
	
	}

};

namespace
{
	auto r = Register<RGBToHexGui>::withId(L"RGB to Hex");
}

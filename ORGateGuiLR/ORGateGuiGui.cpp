#include "mp_sdk_gui2.h"
#include <vector>
//#include <sstream>
//#include <fstream>

using namespace gmpi;

class ORGateGuiGui final : public SeGuiInvisibleBase
{
    void onSetBoolOut()
    {
        bool orResult = false;
        for (size_t i = 1; i < inValues.size(); ++i) { // Skip index 0 if it's not used for input
            orResult |= inValues[i];
        }
        // getHost()->pinTransmit(0, sizeof(orResult), &orResult);
        pinBoolOut = orResult;
    }

 	BoolGuiPin pinBoolOut;

    MpGuiPin<bool> pinSpareIn;
    std::vector<bool> inValues;

public:

    // Declare static member
  //  static std::ofstream logFile;

	ORGateGuiGui()
	{
		initializePin( pinBoolOut);

/*        // Initialize the log file
        if (!logFile.is_open())
        {
            logFile.open("pinId_log.txt", std::ios::app);
            if (!logFile)
            {
                // handle error if needed
            }
        }*/
	}
    virtual int32_t MP_STDCALL setPin(int32_t pinId, int32_t voice, int32_t size, const void* data) override
    {
/*        // Log info
        std::stringstream ss;
        ss << "pinId: " << pinId << ", size: " << size << std::endl;
        if (logFile.is_open()) {
            logFile << ss.str() << "\n";
            logFile.flush();
        }*/

        if (pinId == 0) {
            // This is the output pin, do not store in inValues
            bool value;
            VariableFromRaw<bool>(size, data, value);
            // Directly transmit the output value
            getHost()->pinTransmit(pinId, size, data);
            // Optionally, update internal state if needed
            // e.g., store last output value if necessary
        }
        else {
            // This is an input pin, store its value
            if (pinId >= (int)inValues.size()) {
                inValues.resize(pinId + 1, false);
            }
            bool value;
            VariableFromRaw<bool>(size, data, value);
            inValues[pinId] = value;

            // Recalculate OR result
            onSetBoolOut();
        }
        return 0;
    }
};

// Define static member
//std::ofstream ORGateGuiGui::logFile;

namespace
{
	auto r = Register<ORGateGuiGui>::withId(L"OR Gate GuiLR");
}

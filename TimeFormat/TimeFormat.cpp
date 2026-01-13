#include "mp_sdk_audio.h"
#include <string>
#include <sstream>
#include <iomanip>

using namespace gmpi;

class TimeFormat final : public MpBase2
{
    IntInPin pinMilliseconds;
    StringOutPin pinTextOut; // outputs time as a string in format M:SS.mmm

public:
    TimeFormat()
    {
        initializePin(pinMilliseconds);
        initializePin(pinTextOut);
    }

    void onSetPins() override
    {
        long long currentMilliseconds = pinMilliseconds;

        // Convert to minutes, seconds, milliseconds
        long long minutes = currentMilliseconds / 60000;
        long long seconds = (currentMilliseconds % 60000) / 1000;
        long long milliseconds = (currentMilliseconds % 1000) / 10;

        // Format time string: "M:SS.mmm"
        std::ostringstream oss;
        oss << minutes << ":"  // minutes can be variable length
            << std::setfill('0') << std::setw(2) << seconds << "."  // seconds always 2 digits
            << std::setfill('0') << std::setw(2) << milliseconds; // milliseconds 3 digits

        // Output the formatted time string
        pinTextOut = oss.str();
    }
};

namespace
{
    auto r = Register<TimeFormat>::withId(L"TimeFormat");
}
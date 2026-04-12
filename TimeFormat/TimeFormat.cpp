#include "mp_sdk_audio.h"
#include <string>
#include <sstream>
#include <iomanip>

using namespace gmpi;

class TimeFormat final : public MpBase2
{
    IntInPin pinMilliseconds;
    StringOutPin pinTextOut; // outputs time as a string in format M:SS.mm or H:MM:SS.mm

public:
    TimeFormat()
    {
        initializePin(pinMilliseconds);
        initializePin(pinTextOut);
    }

    void onSetPins() override
    {
        long long currentMilliseconds = pinMilliseconds;

        // Convert total milliseconds into components
        long long totalSeconds = currentMilliseconds / 1000;
        long long minutes = totalSeconds / 60;
        long long seconds = totalSeconds % 60;
        long long milliseconds = (currentMilliseconds % 1000) / 10; // first two digits of milliseconds

        std::ostringstream oss;

        if (minutes >= 60)
        {
            long long hours = minutes / 60;
            long long remainingMinutes = minutes % 60;

            // Format: H:MM:SS.mm
            oss << hours << ":"
                << std::setfill('0') << std::setw(2) << remainingMinutes << ":"
                << std::setfill('0') << std::setw(2) << seconds << "."
                << std::setfill('0') << std::setw(2) << milliseconds;
        }
        else
        {
            // Format: M:SS.mm
            oss << minutes << ":"
                << std::setfill('0') << std::setw(2) << seconds << "."
                << std::setfill('0') << std::setw(2) << milliseconds;
        }

        // Output the formatted time string
        pinTextOut = oss.str();
    }
};

namespace
{
    auto r = Register<TimeFormat>::withId(L"TimeFormat");
}
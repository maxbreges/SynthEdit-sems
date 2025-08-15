#include "mp_sdk_gui2.h"

#ifdef _WIN32
#include <windows.h>
#include <string>
#ifndef MAX_PATH
#define MAX_PATH 260
#endif
#endif

#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <limits.h>
#include <string>
#endif

using namespace gmpi;

class LocationGui final : public SeGuiInvisibleBase
{
    void onSetPath()
    {
        // pinPath changed
    }

    StringGuiPin pinPath;

public:
    LocationGui()
    {
        initializePin(pinPath, static_cast<MpGuiBaseMemberPtr2>(&LocationGui::onSetPath));
        // Don't set pinPath here if it causes issues
    }

   int32_t initialize()
    {
        // Call this after construction
        std::string path = getModulePath();
        if (!path.empty())
        {
            pinPath = path;
        }
        return gmpi::MP_OK;
    }

private:
    std::string getModulePath()
    {
        char buffer[MAX_PATH];

#ifdef _WIN32
        DWORD length = GetModuleFileNameA(NULL, buffer, MAX_PATH);
        if (length == 0 || length == MAX_PATH)
            return "";
        return std::string(buffer, length);
#elif __APPLE__
        uint32_t size = sizeof(buffer);
        if (_NSGetExecutablePath(buffer, &size) != 0)
        {
            std::vector<char> buf(size);
            if (_NSGetExecutablePath(buf.data(), &size) == 0)
                return std::string(buf.data(), size);
            else
                return "";
        }
        else
        {
            return std::string(buffer, size);
        }
#else
        return "";
#endif
    }
};

namespace
{
    auto r = Register<LocationGui>::withId(L"My Location");
}
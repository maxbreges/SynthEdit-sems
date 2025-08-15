#include "mp_sdk_gui2.h"

#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

using namespace gmpi;

class LocationGui final : public SeGuiInvisibleBase
{
    void onSetPathOut()
    {
        std::string path;

#ifdef _WIN32
        HMODULE hModule = NULL;
        // Get handle of current module (DLL)
        hModule = GetModuleHandleA(NULL);
        char buffer[MAX_PATH];
        if (GetModuleFileNameA(hModule, buffer, MAX_PATH))
        {
            path = std::string(buffer);
        }
#elif __APPLE__
        CFURLRef pluginURL = CFBundleCopyBundleURL(CFBundleGetMainBundle());
        char buffer[PATH_MAX];
        if (CFURLGetFileSystemRepresentation(pluginURL, true, (UInt8*)buffer, PATH_MAX))
        {
            path = std::string(buffer);
        }
        CFRelease(pluginURL);
#else
        path = "Platform not supported.";
#endif

        pinPathOut = path;
    }

    StringGuiPin pinPathOut;

public:
    LocationGui()
    {
        initializePin(pinPathOut, static_cast<MpGuiBaseMemberPtr2>(&LocationGui::onSetPathOut));
    }

    int32_t initialize()
    {
        // Call onSetPathOut() once at construction to initialize the path
        onSetPathOut();

        return gmpi::MP_OK;
    }
};

namespace
{
    auto r = Register<LocationGui>::withId(L"Location");
}
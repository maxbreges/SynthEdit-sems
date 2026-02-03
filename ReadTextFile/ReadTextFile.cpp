#include "mp_sdk_gui2.h"
#include <fstream>
#include <sstream>

using namespace gmpi;

class ReadTextFile final : public SeGuiInvisibleBase
{
    StringGuiPin pinFilenameIn;
    StringGuiPin pinTextOut;
    BoolGuiPin pinBoolIn;

public:
    ReadTextFile()
    {
        initializePin(pinFilenameIn, static_cast<MpGuiBaseMemberPtr2>(&ReadTextFile::onSetFilenameIn));
        initializePin(pinTextOut);
        initializePin(pinBoolIn, static_cast<MpGuiBaseMemberPtr2>(&ReadTextFile::onSetBoolIn));

    }

    void onSetFilenameIn()
    {
        readFile();
    }

    void onSetBoolIn()
    {
        readFile();
    }

private:
    void readFile()
    {
        std::wstring filename = pinFilenameIn;

        // Open the file
        std::wifstream file(filename);
        if (file)
        {
            // Read entire file into a wstring
            std::wstringstream buffer;
            buffer << file.rdbuf();

            // Output the file content to pinTextOut
            pinTextOut = buffer.str();
        }
        else
        {
            // If file can't be opened, output error message
            pinTextOut = L"Locate the content file";
        }
    }
};

namespace
{
    auto r = Register<ReadTextFile>::withId(L"readTextFile");
}
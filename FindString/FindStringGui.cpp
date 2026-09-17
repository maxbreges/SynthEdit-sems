#include "mp_sdk_gui2.h"

using namespace gmpi;

class FindStringGui final : public SeGuiInvisibleBase
{
    std::string inputString;
    std::string stringToFind;

    // Helper function to get prefix or suffix based on 'getPrefix' flag
    std::string getPartOfString(const std::string& path, bool getPrefix)
    {
        size_t found_pos = path.find(stringToFind);
        if (found_pos != std::string::npos)
        {
            pinPosition = static_cast<int>(found_pos);
            if (getPrefix)
                return path.substr(0, found_pos);
            else
                return path.substr(found_pos);
        }
        else
        {
            pinPosition = -1; // not found
            return "";
        }
    }

    void onSetString()
    {
        inputString = pinString;
        stringToFind = pinStringToFind;

        pin0Substr = getPartOfString(inputString, true);  // prefix
        pinSubstr = getPartOfString(inputString, false); // suffix

        if (pinIncludePreviousFolder)
        {
            pinSubstr = getLastFolderName(pin0Substr) + stringToFind;
        }
        else
        {
            pinSubstr = stringToFind;
        }
    }

    void onSetIncludePreviousFolder()
    {
        onSetString();
    }

public:
    StringGuiPin pinString;
    StringGuiPin pinStringToFind;
    StringGuiPin pin0Substr;
    StringGuiPin pinSubstr;
    IntGuiPin pinPosition;
    BoolGuiPin pinIncludePreviousFolder;

    FindStringGui()
    {
        initializePin(pinString, static_cast<MpGuiBaseMemberPtr2>(&FindStringGui::onSetString));
        initializePin(pinStringToFind, static_cast<MpGuiBaseMemberPtr2>(&FindStringGui::onSetString));
        initializePin(pin0Substr);
        initializePin(pinSubstr);
        initializePin(pinPosition);
        initializePin(pinIncludePreviousFolder, static_cast<MpGuiBaseMemberPtr2>(&FindStringGui::onSetIncludePreviousFolder));
    }

    std::string getLastFolderName(const std::string& path)
    {
        // Remove trailing slashes
        size_t endPos = path.size();
        while (endPos > 0 && (path[endPos - 1] == '/' || path[endPos - 1] == '\\'))
            --endPos;

        // Find the last slash before endPos
        size_t lastSlashPos = path.rfind('/', endPos - 1);
#if defined(_WIN32)
        size_t lastBackslashPos = path.rfind('\\', endPos - 1);
        lastSlashPos = std::max<int>(lastSlashPos, lastBackslashPos);
#endif

        if (lastSlashPos == std::string::npos)
            return ""; // no slash found, no folder

        // Extract the folder name after last slash
        return path.substr(lastSlashPos + 1, endPos - lastSlashPos);
    }
};

namespace
{
    auto r = Register<FindStringGui>::withId(L"My FindString");
}
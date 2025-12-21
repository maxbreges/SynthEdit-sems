#include "mp_sdk_gui2.h"
#include <vector>
#include <string>
#include <sstream>

using namespace gmpi;

class ListToTextGui final : public SeGuiInvisibleBase
{
    void onSetChoice()
    {
        updateItemText();
    }

    void onSetItemList()
    {
        updateItemText();
    }

    IntGuiPin pinChoice;
    StringGuiPin pinItemList;
    StringGuiPin pinItemText;

#ifdef _WIN32
    // Helper function to split UTF-8 string into wstring based on commas
    std::vector<std::wstring> splitString(const std::wstring& s, wchar_t delimiter)
    {
        std::vector<std::wstring> tokens;
        std::wstringstream ss(s);
        std::wstring item;
        while (std::getline(ss, item, delimiter))
        {
            tokens.push_back(item);
        }
        return tokens;
}
#else
    // Helper function to split UTF-8 string into wstring based on commas
    std::vector<std::string> splitString(const std::string& s, char delimiter)
    {
        std::vector<std::string> tokens;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delimiter))
        {
            tokens.push_back(item);
        }
        return tokens;
    }
#endif

    void updateItemText()
    {
        // Convert the UTF-8 string from pinItemList to wstring
        std::wstring listW = pinItemList; // Assuming pinItemList supports implicit conversion to wstring

        auto items = splitString(listW, L',');

        int choiceIndex = static_cast<int>(pinChoice);

        if (choiceIndex >= 0 && choiceIndex < static_cast<int>(items.size()))
        {
            pinItemText = items[choiceIndex];
        }
        else
        {
            pinItemText = L"";
        }
    }

public:
    ListToTextGui()
    {
        initializePin(pinChoice, static_cast<MpGuiBaseMemberPtr2>(&ListToTextGui::onSetChoice));
        initializePin(pinItemList, static_cast<MpGuiBaseMemberPtr2>(&ListToTextGui::onSetItemList));
        initializePin(pinItemText);
    }
};

namespace
{
    auto r = Register<ListToTextGui>::withId(L"ListToText");
}
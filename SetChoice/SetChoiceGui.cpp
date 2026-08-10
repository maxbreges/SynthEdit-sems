#include "mp_sdk_gui2.h"
#include <string>
#include <sstream>

using namespace gmpi;

class SetChoiceGui final : public SeGuiInvisibleBase
{
    std::string filename;

 	void onSetFileName()
	{		
        filename = pinFileName;
        names.clear();
        onSetItemList();
	}

    std::vector<std::string> names;

    void onSetItemList()
    {
        // read the list of names from the first pin
        std::string itemList = pinItemList;

        // split the list into individual names

        std::stringstream ss(itemList);
        std::string token;
        while (std::getline(ss, token, ',')) {
            // Remove leading/trailing whitespace if necessary
            token.erase(0, token.find_first_not_of(" \t\n\r\f\v"));
            token.erase(token.find_last_not_of(" \t\n\r\f\v") + 1);
            names.push_back(token);
        }
        onSetChoiceOutput();
    }

    void onSetChoiceOutput()
    {
        // find the index of filename in names
        int index = -1; // default if not found
        for (size_t i = 0; i < names.size(); ++i)
        {
            if (names[i] == filename)
            {
                index = static_cast<int>(i);
                break;
            }
        }
        pinChoice = index;
    }

    void onSetChoice()
    {
        filename = names[pinChoice];
        pinFileName = filename;
    }

 	StringGuiPin pinItemList;
 	StringGuiPin pinFileName;
 	IntGuiPin pinChoice;

public:
	SetChoiceGui()
	{
		initializePin( pinItemList, static_cast<MpGuiBaseMemberPtr2>(&SetChoiceGui::onSetItemList) );
		initializePin( pinFileName, static_cast<MpGuiBaseMemberPtr2>(&SetChoiceGui::onSetFileName) );
		initializePin( pinChoice, static_cast<MpGuiBaseMemberPtr2>(&SetChoiceGui::onSetChoice) );
	}
};

namespace
{
	auto r = Register<SetChoiceGui>::withId(L"My SetChoice");
}

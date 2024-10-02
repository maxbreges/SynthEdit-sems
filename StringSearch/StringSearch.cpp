#include "mp_sdk_gui2.h"

using namespace std;
using namespace gmpi;

class StringSearchGui final : public SeGuiInvisibleBase
{
	std::string String;
	std::string Search;
	int position = 0;

 	void onSetString()
	{	
		String = pinString;
		StringSearch();
	}
	void onSetStringL()
	{
		String = pinStringL;
		StringSearch();
	}

 	void onSetSearch()
	{
		Search = pinSearch;
		StringSearch();
	}
	void onSetSearchL()
	{
		Search = pinSearchL;
		StringSearch();
	}

 	void onSetIndex()
	{	
		pinIndex = position;
	}

	void onSetIndexLout()
	{
		pinIndexLout = position;
	}

 	StringGuiPin pinString;
	StringGuiPin pinStringL;
 	StringGuiPin pinSearch;
	StringGuiPin pinSearchL;
 	IntGuiPin pinIndex;
 	IntGuiPin pinIndexLout;

public:
	StringSearchGui()
	{
		initializePin( pinString, static_cast<MpGuiBaseMemberPtr2>(&StringSearchGui::onSetString) );
		initializePin(pinStringL, static_cast<MpGuiBaseMemberPtr2>(&StringSearchGui::onSetStringL));
		initializePin( pinSearch, static_cast<MpGuiBaseMemberPtr2>(&StringSearchGui::onSetSearch) );
		initializePin(pinSearchL, static_cast<MpGuiBaseMemberPtr2>(&StringSearchGui::onSetSearchL));
		initializePin( pinIndex, static_cast<MpGuiBaseMemberPtr2>(&StringSearchGui::onSetIndex) );
		initializePin( pinIndexLout, static_cast<MpGuiBaseMemberPtr2>(&StringSearchGui::onSetIndexLout) );
	}

	int StringSearch()
	{
		std::string str = String;
		std::string chr = Search;

		std::size_t found = str.find(chr) + 1;
		
		position = found;

		onSetIndexLout();
		onSetIndex();

		return gmpi::MP_OK;
	}
};

namespace
{
	auto r = Register<StringSearchGui>::withId(L"StringSearch");
}

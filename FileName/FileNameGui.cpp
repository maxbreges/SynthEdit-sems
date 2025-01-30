#include "mp_sdk_gui2.h"
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

using namespace gmpi;

class FileNameGui final : public SeGuiInvisibleBase
{
 	void onSetPath()
	{
		std::string filepath = pinPath;

		std::stringstream ss;
		std::stringstream ssEx;

		ss << fs::path(filepath).stem().string();
		ssEx << fs::path(filepath).extension().string();

		std::string filename(ss.str());
		std::string fileExtension(ssEx.str());
	
		pinFileName = filename;
		pinExt = fileExtension;
	}
 	

 	StringGuiPin pinPath;
 	StringGuiPin pinFileName;
	StringGuiPin pinExt;

public:
	FileNameGui()
	{
		initializePin( pinPath, static_cast<MpGuiBaseMemberPtr2>(&FileNameGui::onSetPath) );
		initializePin( pinFileName, static_cast<MpGuiBaseMemberPtr2>(&FileNameGui::onSetPath) );
		initializePin(pinExt, static_cast<MpGuiBaseMemberPtr2>(&FileNameGui::onSetPath));
	}

};

namespace
{
	auto r = Register<FileNameGui>::withId(L"FileName");
}

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
		std::stringstream Directory;
		std::stringstream ssEx;
		std::stringstream ssRPath;

		ss << fs::path(filepath).stem().string();	
		Directory << fs::path(filepath).parent_path().string();
		ssEx << fs::path(filepath).extension().string();
		ssRPath << fs::path(filepath).relative_path().string();

		std::string filename(ss.str());
		std::string directory(Directory.str());
		std::string fileExtension(ssEx.str());
		std::string RelativePath(ssRPath.str());
	
		pinFileName = filename;
		pinDirectory = directory;
		pinExt = fileExtension;
		pinRelativePath = RelativePath;
	}
 	
 	StringGuiPin pinPath;
	StringGuiPin pinDirectory;
 	StringGuiPin pinFileName;
	StringGuiPin pinExt;
	StringGuiPin pinRelativePath;

public:
	FileNameGui()
	{
		initializePin( pinPath, static_cast<MpGuiBaseMemberPtr2>(&FileNameGui::onSetPath) );
		initializePin(pinDirectory, static_cast<MpGuiBaseMemberPtr2>(&FileNameGui::onSetPath));
		initializePin( pinFileName, static_cast<MpGuiBaseMemberPtr2>(&FileNameGui::onSetPath) );
		initializePin(pinExt, static_cast<MpGuiBaseMemberPtr2>(&FileNameGui::onSetPath));
		initializePin(pinRelativePath, static_cast<MpGuiBaseMemberPtr2>(&FileNameGui::onSetPath));
	}
};

namespace
{
	auto r = Register<FileNameGui>::withId(L"FileName");
}

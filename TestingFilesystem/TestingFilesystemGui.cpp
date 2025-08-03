#include "mp_sdk_gui2.h"
#include <filesystem>

namespace fs = std::filesystem;

using namespace gmpi;

class TestingFilesystemGui final : public SeGuiInvisibleBase
{
 	void onSetFilePath()
	{
		auto parentPath = fs::path(pinFilePath).parent_path();
		pinDirectory = parentPath;

		auto filename = fs::path(pinFilePath).stem().wstring();
		pinFileName = filename;

		auto file_extension = fs::path(pinFilePath).extension().string();
		if (!file_extension.empty() && file_extension.front() == '.')
			file_extension.erase(0, 1);
		pinExtension = file_extension;
	}

 	void onSetDirectory()
	{
		// pinDirectory changed
	}

	void onSetFileName()
	{
		// pinDirectory changed
	}

	void onSetExtension()
	{
		// pinDirectory changed
	}

 	StringGuiPin pinFilePath;
	StringGuiPin pinDirectory;
	StringGuiPin pinFileName;
	StringGuiPin pinExtension;

public:
	TestingFilesystemGui()
	{
		initializePin( pinFilePath, static_cast<MpGuiBaseMemberPtr2>(&TestingFilesystemGui::onSetFilePath) );
		initializePin(pinDirectory);
		initializePin(pinFileName);
		initializePin(pinExtension);
	}

};

namespace
{
	auto r = Register<TestingFilesystemGui>::withId(L"TestingFilesystem");
}

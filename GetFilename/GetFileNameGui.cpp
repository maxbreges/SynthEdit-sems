#include "mp_sdk_gui2.h"

using namespace gmpi;

class GetFilenameGui final : public SeGuiInvisibleBase
{
    std::string get_directory(const std::string& path) {
        size_t found = path.find_last_of("/\\");
        if (found == std::string::npos) return "";
        return path.substr(0, found + 1);
    }

    std::string get_filename(const std::string& path) {
        size_t found = path.find_last_of("/\\");
        if (found == std::string::npos) return "";
        return path.substr(found + 1);
    }

    std::string get_extension(const std::string& p_filename)
    {
        size_t found = p_filename.find_last_of(".");
        if (found == std::string::npos) return "";
        return p_filename.substr(found);
    }

   std::string StripExtension(const std::string& p_filename)
    {
       size_t found = p_filename.find_last_of(".");
       if (found == std::string::npos) return "";
       return p_filename.substr(0,found);
    }

   void onSetFilePathIn()
   {
       pinDirectory = get_directory(pinFilePathIn);

       // Retrieve the input value
       std::string filename = get_filename(pinFilePathIn);
       // Strip extension
       std::string filenameWithoutExtension = StripExtension(filename);
       pinFilename = filenameWithoutExtension;

       pinExt = get_extension(filename);
   }

    StringGuiPin pinFilePathIn;
    StringGuiPin pinDirectory;
    StringGuiPin pinFilename;
    StringGuiPin pinExt;

public:
    GetFilenameGui()
    {
        initializePin(pinFilePathIn, static_cast<MpGuiBaseMemberPtr2>(&GetFilenameGui::onSetFilePathIn));
        initializePin(pinDirectory);
        initializePin(pinFilename);
        initializePin(pinExt);
    }
};

namespace
{
    auto r = Register<GetFilenameGui>::withId(L"GetFilename");
}
#include "mp_sdk_gui2.h"

using namespace gmpi;

class StripExtensionGui final : public SeGuiInvisibleBase
{
    std::string get_filename(const std::string& path) {
        size_t found = path.find_last_of("/\\");
        if (found == std::string::npos) return "";
        return path.substr(found + 1);
    }

   std::string StripExtension(const std::string& p_filename)
    {
       size_t found = p_filename.find_last_of(".");
       if (found == std::string::npos) return "";
       return p_filename.substr(0,found);
    }

   void onSetFilePathIn()
   {
       // Retrieve the input value
       std::string filename = get_filename(pinFilePathIn);

       // Strip extension
       std::string filenameWithoutExtension = StripExtension(filename);

       // Output the result
       pinFilename = filenameWithoutExtension;
   }

    StringGuiPin pinFilePathIn;
    StringGuiPin pinFilename;

public:
    StripExtensionGui()
    {
        initializePin(pinFilePathIn, static_cast<MpGuiBaseMemberPtr2>(&StripExtensionGui::onSetFilePathIn));
        initializePin(pinFilename);
    }
};

namespace
{
    auto r = Register<StripExtensionGui>::withId(L"StripExtension");
}
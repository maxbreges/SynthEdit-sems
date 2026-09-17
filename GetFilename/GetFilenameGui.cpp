#include "mp_sdk_gui2.h"

using namespace gmpi;

class GetFilenameGui final : public SeGuiInvisibleBase
{
    std::string get_directory(const std::string& path)
    {
        size_t sepPos = path.find_last_of("/\\");
        if (sepPos != std::string::npos)
            return path.substr(0, sepPos+1);
        return "";
    }

    std::string get_filename(const std::string& name)
    {
        size_t sepPos = name.find_last_of("/\\");
        if (sepPos != std::string::npos)
            return name.substr(sepPos + 1);
        return name;
    }

    std::string get_extension(std::string& ext)
    {
        size_t found = ext.find_last_of(".");
        if (found == std::string::npos) return ext.substr(found);
        return ext.substr(found);
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
       std::string ext = get_extension(filename);
 
           // Convert extension to lowercase
           std::transform(ext.begin(), ext.end(), ext.begin(),
               [](unsigned char c) { return std::tolower(c); });
       
           pinExt = ext;

       // Strip extension
       std::string filenameWithoutExtension = StripExtension(filename);
       pinFilename = filenameWithoutExtension;       
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
        initializePin(pinFilename, static_cast<MpGuiBaseMemberPtr2>(&GetFilenameGui::onSetFileName));
        initializePin(pinExt);
    }
    void onSetFileName()
    {
        std::string Name = pinFilename;
        std::string Directory = pinDirectory;
        std::string Extension = pinExt;
        std::string fullPath = Directory + Name + Extension;

        pinFilePathIn = fullPath;
    }
};

namespace
{
    auto r = Register<GetFilenameGui>::withId(L"GetFilename");
}
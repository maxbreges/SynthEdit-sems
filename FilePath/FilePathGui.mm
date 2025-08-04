#include "mp_sdk_gui2.h"

// Include Objective-C headers for macOS dialog
#ifdef __APPLE__
#import <Cocoa/Cocoa.h>
#endif

using namespace gmpi;

class FilePathGui final : public SeGuiInvisibleBase
{
    void onSetFilePath()
    {
        // Pin file name changed
        // Handle update logic here if needed
    }

    void onSetTrigger()
    {
        if (pinTrigger.getValue())  // Assuming true/false trigger
        {
            pinTrigger = false;

#ifdef __APPLE__
            // Call the method directly
            NSString* filename = openFileDialog();
            if (filename)
            {
                // Convert NSString to std::wstring
                const char* utf8Str = [filename UTF8String];
                size_t len = strlen(utf8Str);
                std::wstring wfilename;
                wfilename.resize(len);
                mbstowcs(&wfilename[0], utf8Str, len);
                pinFilePath.setValue(wfilename);
            }
#endif
        }
    }

    StringGuiPin pinFilePath;
    BoolGuiPin pinTrigger;

public:
    FilePathGui()
    {
        initializePin(pinFilePath, static_cast<MpGuiBaseMemberPtr2>(&FilePathGui::onSetFilePath));
        initializePin(pinTrigger, static_cast<MpGuiBaseMemberPtr2>(&FilePathGui::onSetTrigger));
    }

#ifdef __APPLE__
private:
    NSString* openFileDialog()
    {
        __block NSString* selectedFile = nil;

        dispatch_sync(dispatch_get_main_queue(), ^ {
            NSOpenPanel * panel = [NSOpenPanel openPanel];

            [panel setCanChooseFiles : YES] ;
            [panel setCanChooseDirectories : NO] ;
            [panel setAllowsMultipleSelection : NO] ;

            if ([panel runModal] == NSModalResponseOK)
            {
                NSURL* url = [[panel URLs]firstObject];
                selectedFile = [url path];
            }
            });

        return selectedFile;
    }
    #endif
};

// Register plugin
namespace
{
    auto r = Register<FilePathGui>::withId(L"FilePath");
}
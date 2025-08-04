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
        // You can handle updates here if needed
    }

    void onSetTrigger()
    {
        // When trigger pin is activated, open file dialog
        if (pinTrigger.getValue())  // Assuming true/false trigger
        {
            // Reset trigger pin to false (assuming active-high trigger)
            pinTrigger = false;

#ifdef __APPLE__
            // Call to open native file dialog
            NSString* filename = [self openFileDialog];
            if (filename)
            {
                // Convert NSString to std::wstring or std::string
                std::wstring wfilename((const wchar_t*)[filename UTF8String]);
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
        // Use Objective-C to open a native macOS file dialog
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
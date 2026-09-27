#include "mp_sdk_gui2.h"

using namespace gmpi;

class SubStringGui final : public SeGuiInvisibleBase
{
    std::string folderDialogPath;
    std::string appDir;
    std::string relativePath;
    bool boolFlag = false;

    void onSetFolderDialogPath() //user folder pin
    {
        onSetAppDir();
    }

    void onSetAppDir() //app dir pin
    {
        appDir = pinAppDir;
        folderDialogPath = pinFolderDialogPath;
        if (folderDialogPath.empty())
        {
            pinFullPath = appDir + relativePath;
            return;
        }
        onCompare();
    }

    void onCompare()
    {
        if (folderDialogPath.compare(0, appDir.size(), appDir) == 0)
        {
            boolFlag = false;
            pinBoolFlag = boolFlag;
            relativePath = folderDialogPath.substr(appDir.size());
            pinRelativePath = relativePath;
        }

        if (folderDialogPath.compare(0, appDir.size(), appDir) != 0)
        {
            boolFlag = true;
            pinBoolFlag = boolFlag;
            pinUserPath = folderDialogPath;
        }
        onSetFullPath();
    }

    void onSetFullPath()
    {
        if (boolFlag)
        {
            pinFolderDialogPath = pinUserPath;
            pinFullPath = pinUserPath;
        }
        if (!boolFlag)
        {
            pinFullPath = appDir + relativePath;
        }
    }

    StringGuiPin pinFolderDialogPath;
    StringGuiPin pinAppDir;
    StringGuiPin pinRelativePath;
    StringGuiPin pinUserPath;
    BoolGuiPin pinBoolFlag;
    StringGuiPin pinFullPath;
    BoolGuiPin pinRescan;

public:
    SubStringGui()
    {
        initializePin(pinFolderDialogPath, static_cast<MpGuiBaseMemberPtr2>(&SubStringGui::onSetFolderDialogPath));
        initializePin(pinAppDir, static_cast<MpGuiBaseMemberPtr2>(&SubStringGui::onSetAppDir));
        initializePin(pinRelativePath, static_cast<MpGuiBaseMemberPtr2>(&SubStringGui::onSetRelativePath));
        initializePin(pinUserPath, static_cast<MpGuiBaseMemberPtr2>(&SubStringGui::onSetUserPath));
        initializePin(pinBoolFlag, static_cast<MpGuiBaseMemberPtr2>(&SubStringGui::onSetBoolFlag));
        initializePin(pinFullPath);
        initializePin(pinRescan, static_cast<MpGuiBaseMemberPtr2>(&SubStringGui::onSetRescan));
    }
    void onSetRelativePath()
    {
        relativePath = pinRelativePath;
        onSetFullPath();
    }

    void onSetUserPath()
    {
    }

    void onSetBoolFlag()
    {
        boolFlag = pinBoolFlag;
        onSetFullPath();
    }

    void onSetRescan()
    {
        onSetBoolFlag();              
    }
};

namespace
{
    auto r = Register<SubStringGui>::withId(L"My SubString");
}
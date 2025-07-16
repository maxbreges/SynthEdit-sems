cmake_minimum_required(VERSION 3.19)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE) 
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version")
set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Build architectures for Mac OS X" FORCE)
set(SE_LOCAL_BUILD FALSE CACHE BOOL "Execute extra build steps for developers machine")

# This is for macOS commandline only, because it uses a single-target generator. for other targets, ref: target_compile_definitions
# this point of this is to ensure NDEBUG macro is set
if(NOT GENERATOR_IS_MULTI_CONFIG)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
endif()

include(FetchContent)

#note: SOURCE_SUBDIR is a subfolder with NO cmake file (so we don't needlessly include the examples)
FetchContent_Declare(
  se_sdk3_repo
  GIT_REPOSITORY https://github.com/JeffMcClintock/SynthEdit_SDK
  GIT_TAG origin/master
  SOURCE_SUBDIR se_sdk3
)

FetchContent_MakeAvailable(se_sdk3_repo)

project(MyModules)

enable_testing()

set(se_sdk_folder
    ${se_sdk3_repo_SOURCE_DIR}/se_sdk3
    )
set(se_shared_folder
    ${se_sdk3_repo_SOURCE_DIR}/shared
    )

set(sdk_folder ${se_sdk_folder})

include_directories(
    ${se_sdk_folder}
    ${se_shared_folder}
    )

set(CMAKE_CXX_STANDARD 17)

add_definitions(-D_UNICODE)
add_definitions(-DSE_TARGET_SEM)

if (MSVC)
    # Floating Point Model: Fast (/fp:fast)
    # Buffer Security Check: No (/GS-)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:fast /GS-")
endif()

include(plugin_helper.cmake)

# here is where you list your modules
#add_subdirectory(DisplayKnob)
#add_subdirectory(TextHold)
#add_subdirectory(IntRGBToHex)
#add_subdirectory(MidiLearn)
#add_subdirectory(MidiLearnX)
#add_subdirectory(DisplayList) 
#add_subdirectory(Modulus)
#add_subdirectory (PitchDisplay)
#add_subdirectory (DrumTrigger)
#add_subdirectory (NoteName)
#add_subdirectory (mxXYText)
#add_subdirectory (MatchPitchGate)
#add_subdirectory (MatchPitch)
#add_subdirectory (PopUp)
#add_subdirectory (MIDITranspose)
#add_subdirectory (MIDIDelta)
#add_subdirectory (MIDI2Fret)
#add_subdirectory (FretLed)
#add_subdirectory (Minors)
#add_subdirectory (Increment)
#add_subdirectory (FileName)
#add_subdirectory (FolderList)
#add_subdirectory (LocateFolder)
#add_subdirectory (GuiTextToText)
#add_subdirectory (JustFile)
#add_subdirectory (SetFolder)
#add_subdirectory (JustPath)
#add_subdirectory (FilesFilter)
#add_subdirectory (FloatAnimation)
#add_subdirectory (FretFormula)
#add_subdirectory (MIDI2Frets)
#add_subdirectory (MidiToBlobs)
#add_subdirectory (FretLedX)
#add_subdirectory (setPin)
#add_subdirectory (DoubleClick)
#add_subdirectory (MonostableGui)
#add_subdirectory(ColorKnob)
#add_subdirectory (BooleanAND)
#add_subdirectory (TextAppend)
#add_subdirectory (TextAppendGuiR)
#add_subdirectory (PinCountGui)
#add_subdirectory (BooleanNOT)
#add_subdirectory (AutoFixedValuesInt)
#add_subdirectory (AutoFixedValues)
#add_subdirectory (SpinCtrl)
#add_subdirectory (MonoMode)
#add_subdirectory (SwitchFloat)
#add_subdirectory (IntEntry)
#add_subdirectory (FloatEntry)
#add_subdirectory (MidiToHex)
#add_subdirectory (BoolTriggerGui)
#add_subdirectory (FileDialog)
#add_subdirectory (StepCounterGui)
#add_subdirectory (StringRight)
#add_subdirectory (GrayscaleHex)
#add_subdirectory (Brightness)
#add_subdirectory (VectorRing)
#add_subdirectory (ImageX)
#add_subdirectory (JoystickImage)
#add_subdirectory (TextEntry)
#add_subdirectory (Button)
#add_subdirectory (PulseGen)
#add_subdirectory (SquareOsc)
#add_subdirectory (MonostableX)
#add_subdirectory (QueLoader) //failed
#add_subdirectory (TimestampMidi)
#add_subdirectory (StringLeft)
#add_subdirectory (ORGateGui)
#add_subdirectory (ORGateGuiR)
#add_subdirectory (ORGateGuiLR)
#add_subdirectory (WaveRecorderX)
#add_subdirectory (PopUpMenu)
add_subdirectory (mxSOLOMUTE)







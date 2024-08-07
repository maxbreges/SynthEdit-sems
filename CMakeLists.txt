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
add_subdirectory(IntToHex)

cmake_minimum_required(VERSION 3.19)

project(TestingFilesystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Your existing macro
build_gmpi_plugin(
    PROJECT_NAME ${PROJECT_NAME}
    HAS_GUI
    SOURCE_FILES
      TestingFilesystemGui.cpp
)


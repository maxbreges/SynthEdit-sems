cmake_minimum_required(VERSION 3.19)

project(LocateFolder)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_GUI
	SOURCE_FILES
	  LocateFolderGui.cpp
)
if (APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    target_link_libraries(your_target_name ${COCOA_LIBRARY})
endif()

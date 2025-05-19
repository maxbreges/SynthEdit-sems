cmake_minimum_required(VERSION 3.19)

project(QueLoader)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	HAS_GUI
	SOURCE_FILES
	  QueLoader.h
	  QueLoader.cpp
	  QueLoaderGui.h
	  QueLoaderGui.cpp	  
)

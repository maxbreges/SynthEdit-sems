cmake_minimum_required(VERSION 3.19)

project(ShowInt)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	HAS_GUI
	SOURCE_FILES
	  ShowInt.cpp
	  ShowIntGui.cpp
)

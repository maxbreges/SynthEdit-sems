cmake_minimum_required(VERSION 3.19)

project(StringRight)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_GUI
	SOURCE_FILES
	  StringRightGui.cpp
)

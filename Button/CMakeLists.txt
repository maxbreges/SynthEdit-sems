cmake_minimum_required(VERSION 3.15)

project(Button)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_GUI
	SOURCE_FILES
	  ButtonGui.cpp
)

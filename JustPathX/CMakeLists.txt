cmake_minimum_required(VERSION 3.15)

project(JustPathX)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_GUI
	SOURCE_FILES
	  JustPathXGui.cpp
)

cmake_minimum_required(VERSION 3.19)

project(FloatGui2Float)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	HAS_GUI
	SOURCE_FILES
	  FloatGui2Float.cpp
	  FloatGui2FloatGui.cpp
)

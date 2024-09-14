cmake_minimum_required(VERSION 3.19)

project(Float2FloatGui)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	HAS_GUI
	SOURCE_FILES
	  Float2FloatGui.cpp
	  Float2FloatGuiGui.cpp
)

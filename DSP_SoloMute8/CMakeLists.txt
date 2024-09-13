cmake_minimum_required(VERSION 3.19)

project(DSP_SoloMute8)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	HAS_GUI
	SOURCE_FILES
	  DSP_SoloMute8.cpp
	  DSP_SoloMute8Gui.cpp
)

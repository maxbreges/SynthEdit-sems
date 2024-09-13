cmake_minimum_required(VERSION 3.19)

project(GUI_SoloMute8)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_GUI
	SOURCE_FILES
	  GUI_SoloMute8Gui.cpp
)

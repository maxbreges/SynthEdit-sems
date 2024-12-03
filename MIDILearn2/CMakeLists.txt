cmake_minimum_required(VERSION 3.19)

project(MIDILearn2)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	HAS_GUI
	SOURCE_FILES
	  MIDILearn2.cpp
	  MIDILearn2Gui.cpp
)

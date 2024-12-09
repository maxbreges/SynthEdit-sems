cmake_minimum_required(VERSION 3.19)

project(NoteName)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	HAS_GUI
	SOURCE_FILES
	  NoteName.cpp
	  NoteNameGui.cpp
)

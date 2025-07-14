cmake_minimum_required(VERSION 3.19)

project(MidiLearn)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	HAS_GUI
	SOURCE_FILES
	  MidiLearn.cpp
	  MidiLearnGui.h
	  MidiLearnGui.cpp
)

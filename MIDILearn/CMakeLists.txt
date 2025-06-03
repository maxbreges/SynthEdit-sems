cmake_minimum_required(VERSION 3.19)

project(MidiLearn)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	HAS_GUI
	SOURCE_FILES
	  MIDI_Learn.cpp
	  MIDI_LearnGui.h
	  MIDI_LearnGui.cpp
)

cmake_minimum_required(VERSION 3.19)

project(MIDI2Fret)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	SOURCE_FILES
	  MIDI2Fret.cpp
)

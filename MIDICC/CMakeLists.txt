cmake_minimum_required(VERSION 3.15)

project(MIDICC)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	SOURCE_FILES
	  MIDICC.cpp
)

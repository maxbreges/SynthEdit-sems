cmake_minimum_required(VERSION 3.30)

project(TriggerToMIDI)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	SOURCE_FILES
	  TriggerToMIDI.cpp
)

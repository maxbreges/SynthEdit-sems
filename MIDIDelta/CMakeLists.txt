cmake_minimum_required(VERSION 3.19)

project(MIDIDelta)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	SOURCE_FILES
	  MIDIDelta.cpp
)

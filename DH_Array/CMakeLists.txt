cmake_minimum_required(VERSION 3.30)

project(DH_Array)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	SOURCE_FILES
	  DH_Array.cpp
)

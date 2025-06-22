cmake_minimum_required(VERSION 3.19)

project(WaveRecorderX)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	SOURCE_FILES
	  WaveRecorderX.h
	  WaveRecorderX.cpp
)

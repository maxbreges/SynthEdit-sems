cmake_minimum_required(VERSION 3.19)

project(SampleDelay)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	SOURCE_FILES
	  SampleDelay.cpp
)

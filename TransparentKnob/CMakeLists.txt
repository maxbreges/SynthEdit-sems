cmake_minimum_required(VERSION 3.15)

project(TransparentKnob)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	HAS_GUI
	SOURCE_FILES
	  TransparentKnob.cpp
	  TransparentKnobGui.cpp
)

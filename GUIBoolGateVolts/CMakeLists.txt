cmake_minimum_required(VERSION 3.19)

project(GUIBoolGateVolts)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	HAS_GUI
	SOURCE_FILES
	  GUIBoolGateVolts.cpp
	  GUIBoolGateVoltsGui.cpp
)

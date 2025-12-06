cmake_minimum_required(VERSION 3.19)

project(DH_ControlTrigger)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	HAS_GUI
	SOURCE_FILES
	  DH_ControlTrigger.cpp
	  DH_ControlTriggerGui.cpp
)

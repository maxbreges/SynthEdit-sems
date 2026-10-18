cmake_minimum_required(VERSION 3.19)

project(PulsesOnHold)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_GUI
	SOURCE_FILES
	  PulsesOnHoldGui.cpp
	${se_sdk_folder}/TimerManager.h
        ${se_sdk_folder}/TimerManager.cpp
)

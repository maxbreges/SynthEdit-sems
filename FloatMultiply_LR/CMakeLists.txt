cmake_minimum_required(VERSION 3.19)

project(FloatMultiply_LR)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_GUI
	SOURCE_FILES
	  FloatMultiply_LR.cpp
)

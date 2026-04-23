cmake_minimum_required(VERSION 3.19)

project(CC2Note)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
	SOURCE_FILES
	  CC2Note.cpp
)

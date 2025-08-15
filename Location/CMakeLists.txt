cmake_minimum_required(VERSION 3.19)

project(Location)

build_gmpi_plugin(
	PROJECT_NAME Location
	HAS_GUI
	SOURCE_FILES
	  LocationGui.cpp
)

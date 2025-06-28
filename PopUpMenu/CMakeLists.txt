cmake_minimum_required(VERSION 3.19)

project(PopUpMenu)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_GUI
	SOURCE_FILES
	  PopUpMenuGui.h
	  PopUpMenuGui.cpp
	  ${se_sdk_folder}/it_enum_list.h
          ${se_sdk_folder}/it_enum_list.cpp
)

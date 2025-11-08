cmake_minimum_required(VERSION 3.19)

project(ExKnob)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_GUI
	SOURCE_FILES
	  ExKnobGui.cpp
	${se_sdk_folder}/it_enum_list.h
        ${se_sdk_folder}/it_enum_list.cpp
	${se_shared_folder}/unicode_conversion.h
)

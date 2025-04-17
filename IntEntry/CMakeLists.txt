cmake_minimum_required(VERSION 3.19)

project(IntEntry)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_GUI
	SOURCE_FILES
	IntEntryGui.h
	  IntEntryGui.cpp
	${se_sdk_folder}/it_enum_list.h
        ${se_sdk_folder}/it_enum_list.cpp
	${se_sdk_folder}./shared/FontCache.h
        ${se_sdk_folder}./shared/FontCache.cpp
)

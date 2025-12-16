cmake_minimum_required(VERSION 3.19)

project(DisplayText)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_GUI
	SOURCE_FILES
	  DisplayTextGui.cpp
	${se_sdk_folder}/it_enum_list.h
        ${se_sdk_folder}/it_enum_list.cpp
	${se_shared_folder}/FontCache.h
	${se_shared_folder}/FontCache.cpp
	${se_shared_folder}/ImageMetadata.h
	${se_shared_folder}/ImageMetadata.cpp
	${se_shared_folder}/unicode_conversion.h
	${se_shared_folder}/unicode_conversion2.h
)

cmake_minimum_required(VERSION 3.19)

project(IntEntry)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_GUI
	SOURCE_FILES
	  IntEntryGui.h
	  IntEntryGui.cpp
	TextSubcontrol.h
	TextSubcontrol.cpp
	${se_sdk_folder}/it_enum_list.h
        ${se_sdk_folder}/it_enum_list.cpp
	${se_shared_folder}/FontCache.h
	${se_shared_folder}/FontCache.cpp
	${se_shared_folder}/ImageCache.h
	${se_shared_folder}/ImageCache.cpp
	${se_shared_folder}/ImageMetadata.h
	${se_shared_folder}/ImageMetadata.cpp
	${se_sdk_folder}/mp_api.h
)

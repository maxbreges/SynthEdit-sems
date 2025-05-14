cmake_minimum_required(VERSION 3.19)

project(TextEntry)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_GUI
	SOURCE_FILES
	  TextEntryGui.h
	  TextEntryGui.cpp
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
)

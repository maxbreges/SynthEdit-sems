cmake_minimum_required(VERSION 3.19)

project(ImageX)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_GUI
	SOURCE_FILES
	ImageXGui.h
	  ImageXGui.cpp
	${se_sdk_folder}/it_enum_list.h
        ${se_sdk_folder}/it_enum_list.cpp
	${se_sharedLegacyWidgets_folder}/ImageBase.h
	${se_sharedLegacyWidgets_folder}/ImageBase.cpp
	${se_shared_folder}/ImageCache.h
	${se_shared_folder}/ImageCache.cpp
	${se_shared_folder}/ImageMetadata.h
	${se_shared_folder}/ImageMetadata.cpp
)

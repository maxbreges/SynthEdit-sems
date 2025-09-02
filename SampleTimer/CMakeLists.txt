cmake_minimum_required(VERSION 3.19)

project(SampleTimer)

build_gmpi_plugin(
    PROJECT_NAME ${PROJECT_NAME}
    IS_OFFICIAL_MODULE
    HAS_DSP
    SOURCE_FILES 
        SampleTimer.h
        SampleTimer.cpp
 )

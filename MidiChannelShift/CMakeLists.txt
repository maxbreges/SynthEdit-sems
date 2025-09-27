cmake_minimum_required(VERSION 3.30)

project(MidiChannelShift)

build_gmpi_plugin(
	PROJECT_NAME ${PROJECT_NAME}
	HAS_DSP
FORMATS_LIST GMPI VST3 AU CLAP
	SOURCE_FILES
	  MidiChannelShift.cpp
)

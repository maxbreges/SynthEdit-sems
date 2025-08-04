cmake_minimum_required(VERSION 3.19)
project(FilePath)

add_library(${PROJECT_NAME} SHARED
    FilePathGui.mm
    # other source files if any
)

# Ensure C++ standard
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)

# Link frameworks if needed
target_link_libraries(${PROJECT_NAME}
    "-framework Cocoa"
)
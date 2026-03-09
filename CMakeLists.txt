cmake_minimum_required(VERSION 3.15)
project(FramelessFoldPoC LANGUAGES CXX)

# Add all source files to the library target to be part of the build process. 
# This includes the main controller, mocks, and NvM handling.
add_library(FramelessFoldLib
    package/ExteriorMirrorLibrary/src/FramelessFoldController.cpp
    package/ExteriorMirrorLibrary/src/Mocks.cpp
    package/ExteriorMirrorLibrary/src/NvMHandling.cpp
)
target_include_directories(FramelessFoldLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/package/ExteriorMirrorLibrary/config
    ${CMAKE_CURRENT_SOURCE_DIR}/package/ExteriorMirrorLibrary/dataTypes
    ${CMAKE_CURRENT_SOURCE_DIR}/package/ExteriorMirrorLibrary/src
)

add_executable(poc
    package/ExteriorMirrorLibrary/src/main.cpp
)
target_link_libraries(poc PRIVATE FramelessFoldLib)
# ==============================================================================
# AraPersistency Library
# ==============================================================================
# ARA::PER persistency implementation for storing mirror positions and
# user preferences.
# ==============================================================================

cmake_minimum_required(VERSION 3.16)

project(AraPersistency
    VERSION 1.0.0
    DESCRIPTION "ARA::PER Persistency Layer"
    LANGUAGES CXX
)

# ==============================================================================
# Library Definition
# ==============================================================================

add_library(AraPersistency STATIC
    src/PersistencyManager.cpp
    src/KeyValueStorage.cpp
)

add_library(ExteriorMirror::AraPersistency ALIAS AraPersistency)

# ==============================================================================
# Include Directories
# ==============================================================================

target_include_directories(AraPersistency
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# ==============================================================================
# Dependencies
# ==============================================================================

find_package(Threads REQUIRED)

target_link_libraries(AraPersistency
    PRIVATE
        Threads::Threads
        ExteriorMirror::Core # TBD: DO not use emblInternalTypesn and param this way
)

# ==============================================================================
# Installation
# ==============================================================================

install(TARGETS AraPersistency
    EXPORT ExteriorMirrorCoreTargets 
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include
)
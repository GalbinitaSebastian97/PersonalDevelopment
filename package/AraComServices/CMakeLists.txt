# ==============================================================================
# AraComServices Library
# ==============================================================================
# Contains ARA::COM service proxies and skeletons for inter-process
# communication. Implements shared memory IPC for service discovery.
# ==============================================================================

cmake_minimum_required(VERSION 3.16)

project(AraComServices
    VERSION 1.0.0
    DESCRIPTION "ARA::COM Service Layer with Shared Memory IPC"
    LANGUAGES CXX
)

# ==============================================================================
# Library Definition
# ==============================================================================

add_library(AraComServices STATIC
    src/MirrorFoldServiceProxy.cpp
    src/MirrorFoldServiceSkeleton.cpp
)

add_library(ExteriorMirror::AraComServices ALIAS AraComServices)

# ==============================================================================
# Include Directories
# ==============================================================================

target_include_directories(AraComServices
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ==============================================================================
# Dependencies
# ==============================================================================

find_package(Threads REQUIRED)

target_link_libraries(AraComServices
    PUBLIC
        ExteriorMirror::AraPersistency
    PRIVATE
        Threads::Threads
        ExteriorMirror::Core
)

# Link POSIX realtime library for shared memory (shm_open, etc.)
if(ENABLE_SHARED_MEMORY_IPC)
    target_link_libraries(AraComServices
        PRIVATE
            rt                       # POSIX realtime library
    )
    target_compile_definitions(AraComServices
        PUBLIC
            ENABLE_SHARED_MEMORY_IPC
    )
endif()

# ==============================================================================
# Installation
# ==============================================================================

install(TARGETS AraComServices
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include
)
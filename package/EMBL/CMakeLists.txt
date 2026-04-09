# ==============================================================================
# ExteriorMirrorCore Library
# ==============================================================================
# Contains core business logic for mirror control, independent of communication
# layer (ARA::COM) and platform services.
# ==============================================================================

cmake_minimum_required(VERSION 3.16)

project(ExteriorMirrorCore
    VERSION 1.0.0
    DESCRIPTION "Exterior Mirror Core Business Logic"
    LANGUAGES CXX
)

# ==============================================================================
# Library Definition
# ==============================================================================

add_library(ExteriorMirrorCore STATIC
    # Source files
    appl/EMBLC/src/FramelessFoldControl.cpp
)       

# Alias for cleaner CMake targets
add_library(ExteriorMirror::Core ALIAS ExteriorMirrorCore)

# ==============================================================================
# Include Directories
# ==============================================================================

target_include_directories(ExteriorMirrorCores
    PUBLIC
        # Public headers (available to consumers of this library)
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/appl/EMBLC/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dataTypes>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/config>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        # Private headers (only for internal use)
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ==============================================================================
# Dependencies
# ==============================================================================

# Link to pthread for mutex support
find_package(Threads REQUIRED)
target_link_libraries(ExteriorMirrorCore 
    PRIVATE 
        Threads::Threads
        AraPersistency
)

get_target_property(PER_INC AraPersistency INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "AraPersistency Include Paths: ${PER_INC}")
# ==============================================================================
# Compile Definitions
# ==============================================================================

target_compile_definitions(ExteriorMirrorCore
    PRIVATE
        # Add library-specific defines here
        EXTERIOR_MIRROR_CORE_VERSION="${PROJECT_VERSION}"
)

# ==============================================================================
# Installation
# ==============================================================================

install(TARGETS ExteriorMirrorCore
    EXPORT ExteriorMirrorCoreTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/
    DESTINATION include
)

# ==============================================================================
# Export for find_package()
# ==============================================================================

install(EXPORT ExteriorMirrorCoreTargets
    FILE ExteriorMirrorCoreTargets.cmake
    NAMESPACE ExteriorMirror::
    DESTINATION lib/cmake/ExteriorMirrorCore
)
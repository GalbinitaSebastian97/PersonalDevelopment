# ==============================================================================
# MirrorServiceProvider Application
# ==============================================================================
# AUTOSAR Adaptive service provider executable.
# Hosts the MirrorFoldService skeleton and registers it via shared memory.
# ==============================================================================

cmake_minimum_required(VERSION 3.16)

project(MirrorServiceProvider
    VERSION 1.0.0
    DESCRIPTION "Mirror Fold Service Provider"
    LANGUAGES CXX
)

# ==============================================================================
# Executable Definition
# ==============================================================================

add_executable(MirrorServiceProvider
    src/main_service_provider.cpp
)

# ==============================================================================
# Include Directories
# ==============================================================================

target_include_directories(MirrorServiceProvider
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ==============================================================================
# Dependencies
# ==============================================================================

target_link_libraries(MirrorServiceProvider
    PRIVATE
        ExteriorMirror::Core            # Core business logic
        ExteriorMirror::AraComServices  # Proxy/Skeleton with shared memory
        ExteriorMirror::AraPersistency  # Persistency for storing positions
)

# ==============================================================================
# Installation
# ==============================================================================

install(TARGETS MirrorServiceProvider
    RUNTIME DESTINATION bin
)

# Install manifests alongside the executable
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/manifests/
    DESTINATION manifests
    FILES_MATCHING 
        PATTERN "*.json"
        PATTERN "*.xml"
)

# ==============================================================================
# Post-Build: Copy manifests to build output
# ==============================================================================

add_custom_command(TARGET MirrorServiceProvider POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/manifests
        $<TARGET_FILE_DIR:MirrorServiceProvider>/../manifests
    COMMENT "Copying manifests to output directory"
)



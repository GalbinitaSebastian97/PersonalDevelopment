
# ==============================================================================
# MirrorServiceConsumer Application
# ==============================================================================
# AUTOSAR Adaptive service consumer executable.
# Discovers MirrorFoldService via shared memory and invokes operations.
# ==============================================================================

cmake_minimum_required(VERSION 3.16)

project(MirrorServiceConsumer
    VERSION 1.0.0
    DESCRIPTION "Mirror Fold Service Consumer"
    LANGUAGES CXX
)

# ==============================================================================
# Executable Definition
# ==============================================================================

add_executable(MirrorServiceConsumer
    src/main_service_consumer.cpp
)

# ==============================================================================
# Dependencies
# ==============================================================================

target_link_libraries(MirrorServiceConsumer
    PRIVATE
        ExteriorMirror::AraComServices  # Proxy with shared memory discovery
)

# ==============================================================================
# Installation
# ==============================================================================

install(TARGETS MirrorServiceConsumer
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/manifests/
    DESTINATION manifests
    FILES_MATCHING 
        PATTERN "*.json"
        PATTERN "*.xml"
)
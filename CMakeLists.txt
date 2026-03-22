# ==============================================================================
# AUTOSAR Adaptive Platform - Exterior Mirror System
# Root CMake Configuration
# ==============================================================================

cmake_minimum_required(VERSION 3.16)

project(ExteriorMirrorSystem
    VERSION 1.0.0
    DESCRIPTION "AUTOSAR Adaptive Exterior Mirror Service"
    LANGUAGES CXX
)

# ==============================================================================
# Global Settings
# ==============================================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDE integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ==============================================================================
# Build Options
# ==============================================================================

option(CI_BUILD "Enable CI build settings" OFF)
option(ENABLE_COVERAGE "Enable code coverage" OFF)
option(WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(ENABLE_SHARED_MEMORY_IPC "Enable shared memory IPC for cross-process communication" ON)
option(BUILD_TESTING "Build tests" ON)

# Target application to build (set by run.py)
set(TARGET_APP "" CACHE STRING "Target application to build")

# ==============================================================================
# Compiler Flags
# ==============================================================================

# Common warning flags
add_compile_options(
    -Wall
)

if(WARNINGS_AS_ERRORS)
    add_compile_options(-Werror)
endif()

if(ENABLE_COVERAGE)
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

# Shared memory IPC requires POSIX realtime library
if(ENABLE_SHARED_MEMORY_IPC)
    add_compile_definitions(ENABLE_SHARED_MEMORY_IPC)
    link_libraries(rt pthread)
endif()

# ==============================================================================
# Directory Structure
# ==============================================================================

set(LIBS_DIR ${CMAKE_SOURCE_DIR}/package)
set(APPS_DIR ${CMAKE_SOURCE_DIR}/apps/variant)
set(CONFIG_DIR ${CMAKE_SOURCE_DIR}/config)
set(IDL_DIR ${CMAKE_SOURCE_DIR}/idl)

# ==============================================================================
# Include Paths
# ==============================================================================

# Global include directories
include_directories(
    ${CONFIG_DIR}
    ${IDL_DIR}/generated
)

# ==============================================================================
# Libraries (always built)
# ==============================================================================
# ARA::PER persistency layer
add_subdirectory(${LIBS_DIR}/AraPersistency)

# Core business logic library
add_subdirectory(${LIBS_DIR}/EMBL)

# ARA::COM service layer
add_subdirectory(${LIBS_DIR}/AraComServices)



# ==============================================================================
# Applications (built based on TARGET_APP)
# ==============================================================================

# Function to add an application subdirectory
function(add_app APP_NAME)
    if(TARGET_APP STREQUAL "" OR TARGET_APP STREQUAL APP_NAME OR TARGET_APP STREQUAL "all")
        if(EXISTS ${APPS_DIR}/${APP_NAME}/CMakeLists.txt)
            message(STATUS "Adding application: ${APP_NAME}")
            add_subdirectory(${APPS_DIR}/${APP_NAME})
        else()
            message(WARNING "Application not found: ${APP_NAME}")
        endif()
    endif()
endfunction()

# Add all applications
add_app(MirrorServiceConsumer)
add_app(MirrorAppIntegrated)
add_app(MirrorServiceProvider)
# ==============================================================================
# Testing
# ==============================================================================

#if(BUILD_TESTING)
#    enable_testing()
#    add_subdirectory(test)
#endif()

# ==============================================================================
# Installation
# ==============================================================================

# Install configuration files
install(DIRECTORY ${CONFIG_DIR}/
    DESTINATION config
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.json"
)

# ==============================================================================
# Summary
# ==============================================================================

message(STATUS "")
message(STATUS "=== Exterior Mirror System Configuration ===")
message(STATUS "  Build Type:      ${CMAKE_BUILD_TYPE}")
message(STATUS "  Target App:      ${TARGET_APP}")
message(STATUS "  CI Build:        ${CI_BUILD}")
message(STATUS "  Shared Memory:   ${ENABLE_SHARED_MEMORY_IPC}")
message(STATUS "  Testing:         ${BUILD_TESTING}")
message(STATUS "")
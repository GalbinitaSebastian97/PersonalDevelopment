cmake_minimum_required(VERSION 3.14)
project(MirrorFoldAdaptiveService CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    package/ExteriorMirrorLibrary
    package/ExteriorMirrorLibrary/src
    package/ExteriorMirrorLibrary/adaptive_services
    package/ExteriorMirrorLibrary/service_provider
    package/ExteriorMirrorLibrary/service_consumer
    package/ExteriorMirrorLibrary/dataTypes
    package/ExteriorMirrorLibrary/config
)

# ===== Library: Your existing controller =====
add_library(FramelessFoldControl
    package/ExteriorMirrorLibrary/src/FramelessFoldControl.cpp
    package/ExteriorMirrorLibrary/src/PersistencyManager.cpp
    package/ExteriorMirrorLibrary/src/Mocks.cpp
)

# ===== Library: Adaptive services (skeleton/proxy) =====
add_library(MirrorFoldServices
    package/ExteriorMirrorLibrary/adaptive_services/MirrorFoldServicesSkeleton.cpp
    package/ExteriorMirrorLibrary/adaptive_services/MirrorFoldServiceProxy.cpp
)
target_link_libraries(MirrorFoldServices
    FramelessFoldControl
)

# ===== Executable: Service Provider =====
add_executable(MirrorFoldServiceProvider
    package/ExteriorMirrorLibrary/service_provider/main_service_provider.cpp
)
target_link_libraries(MirrorFoldServiceProvider
    MirrorFoldServices
    FramelessFoldControl
    pthread
)

# ===== Executable: Service Consumer (Test Client) =====
add_executable(MirrorFoldServiceConsumer
    package/ExteriorMirrorLibrary/service_consumer/main_service_consumer.cpp
)
target_link_libraries(MirrorFoldServiceConsumer
    MirrorFoldServices
    pthread
)

# Installation
install(TARGETS 
    MirrorFoldServiceProvider 
    MirrorFoldServiceConsumer
    DESTINATION bin
)

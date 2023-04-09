# CMakeList.txt : CMake project for MetricsCollecor, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(
    SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/DeviceServer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/MetricsCollector.cpp"
)

# Add source to this project's executable.
add_executable (MetricsCollecor ${SOURCES})

target_include_directories(
    MetricsCollecor
    PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
endif()

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET MetricsCollecor PROPERTY CXX_STANDARD 20)
endif()

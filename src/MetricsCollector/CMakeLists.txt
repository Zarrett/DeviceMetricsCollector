# CMakeList.txt : CMake project for MetricsCollector, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

set(
    SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/DeviceServer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/MetricsCollector.cpp"
)

# Add source to this project's executable.
add_executable (MetricsCollector ${SOURCES})

set_property(TARGET MetricsCollector PROPERTY CXX_STANDARD 20)

target_include_directories(
    MetricsCollector
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

    


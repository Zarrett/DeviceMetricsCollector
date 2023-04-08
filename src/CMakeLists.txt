# CMakeList.txt : CMake project for DeviceMetricsCollecor, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("DeviceMetricsCollecor")

set(
    SOURCES 
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
)

# Add source to this project's executable.
add_executable (DeviceMetricsCollecor ${SOURCES})

if(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
endif()

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET DeviceMetricsCollecor PROPERTY CXX_STANDARD 20)
endif()

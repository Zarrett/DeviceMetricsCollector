# CMakeList.txt : CMake project for DeviceMetricsCollector, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("DeviceMetricsCollecor")

if(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(DeviceMetricsCollecor PRIVATE Threads::Threads)
else()
    set(Boost_INCLUDE_DIR "C:/Boost/boost_1_81_0")
endif()

set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

include_directories(${BOOST_INCLUDEDIR}) 
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

add_subdirectory(${CMAKE_SOURCE_DIR}/src/MetricsCollector)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/DeviceMockClient)

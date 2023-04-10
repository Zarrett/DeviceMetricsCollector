# CMakeList.txt : CMake project for DeviceMetricsCollector, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)
set (CMAKE_CXX_STANDARD 20)

project ("DeviceMetricsCollecor")

if(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    link_libraries(Threads::Threads)
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

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_subdirectory(${CMAKE_SOURCE_DIR}/src/MetricsCollector)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/DeviceMockClient)

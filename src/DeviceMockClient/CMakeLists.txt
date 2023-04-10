# CMakeList.txt : CMake project for DeviceMockClientinclude source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

set(
    SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/DeviceClient.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/DeviceInstance.cpp"
)

# Add source to this project's executable.
add_executable (DeviceMockClient ${SOURCES})
target_link_libraries(DeviceMockClient ${Boost_LIBRARIES})

set_property(TARGET MetricsCollector PROPERTY CXX_STANDARD 20)

target_include_directories(
    DeviceMockClient
    PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if(UNIX)
    target_link_libraries(DeviceMockClient PRIVATE Threads::Threads)
endif()


# CMakeList.txt : CMake project for DeviceMockClientinclude source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(
    SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)

# Add source to this project's executable.
add_executable (DeviceMockClient ${SOURCES} "src/DeviceClient.cpp" "src/DeviceInstance.cpp")
target_link_libraries(DeviceMockClient ${Boost_LIBRARIES})

target_include_directories(
    DeviceMockClient
    PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if(UNIX)
    target_link_libraries(DeviceMockClient PRIVATE Threads::Threads)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET DeviceMockClient PROPERTY CXX_STANDARD 20)
endif()

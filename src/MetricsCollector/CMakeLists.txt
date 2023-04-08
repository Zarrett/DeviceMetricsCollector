# CMakeList.txt : CMake project for DeviceMetricsCollecor, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(
    SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)

# Add source to this project's executable.
add_executable (DeviceMetricsCollecor ${SOURCES})
target_link_libraries(DeviceMetricsCollecor ${Boost_LIBRARIES})

target_include_directories(
    DeviceMetricsCollecor 
    PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET DeviceMetricsCollecor PROPERTY CXX_STANDARD 20)
endif()

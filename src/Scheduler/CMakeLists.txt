# CMakeList.txt : CMake project for DeviceMetricsCollecor, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(
    SOURCES 
    "src/Scheduler.cpp"
)

# Add source to this project's executable.
add_library (Scheduler ${SOURCES})
target_link_libraries(Scheduler ${Boost_LIBRARIES})

target_include_directories(
    Scheduler 
    PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if(UNIX)
    target_link_libraries(Scheduler PRIVATE Threads::Threads)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET Scheduler PROPERTY CXX_STANDARD 20)
endif()

# CMakeList.txt : CMake project for TestMetricsCollector, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

set(
    SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TestPacketParser.cpp"
)

enable_testing()

# Add source to this project's executable.
add_executable (TestMetricsCollector ${SOURCES})

target_link_libraries(
  TestMetricsCollector
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(TestMetricsCollector)

set_property(TARGET TestMetricsCollector PROPERTY CXX_STANDARD 20)

target_include_directories(
    TestMetricsCollector
    PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)

if(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
endif()

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

    


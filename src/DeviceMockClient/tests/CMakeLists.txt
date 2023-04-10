# CMakeList.txt : CMake project for TestDeviceMockClient, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

set(
    SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TestMessage.cpp"
)

enable_testing()

# Add source to this project's executable.
add_executable (TestDeviceMockClient ${SOURCES})

target_link_libraries(
  TestDeviceMockClient
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(TestDeviceMockClient)

set_property(TARGET TestDeviceMockClient PROPERTY CXX_STANDARD 20)

target_include_directories(
    TestDeviceMockClient
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

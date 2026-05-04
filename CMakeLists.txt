cmake_minimum_required(VERSION 3.20)
project(polycpp_forwarded VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

set(POLYCPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# Local validation against an in-tree polycpp checkout uses CMake's built-in
# FETCHCONTENT_SOURCE_DIR_POLYCPP override at configure time.
FetchContent_Declare(
    polycpp
    GIT_REPOSITORY git@github.com:enricohuang/polycpp.git
    GIT_TAG        master
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(polycpp)

add_library(polycpp_forwarded
    src/forwarded.cpp
)
target_link_libraries(polycpp_forwarded PUBLIC polycpp)
target_include_directories(polycpp_forwarded PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_library(polycpp::forwarded ALIAS polycpp_forwarded)

set(POLYCPP_FORWARDED_BUILD_TESTS_DEFAULT OFF)
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(POLYCPP_FORWARDED_BUILD_TESTS_DEFAULT ON)
endif()
option(POLYCPP_FORWARDED_BUILD_TESTS "Build tests" ${POLYCPP_FORWARDED_BUILD_TESTS_DEFAULT})
if(POLYCPP_FORWARDED_BUILD_TESTS)
    enable_testing()
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.15.2
    )
    FetchContent_MakeAvailable(googletest)

    add_executable(polycpp_forwarded_test_smoke tests/test_smoke.cpp)
    target_link_libraries(polycpp_forwarded_test_smoke PRIVATE polycpp::forwarded GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(polycpp_forwarded_test_smoke)
endif()

option(POLYCPP_FORWARDED_BUILD_EXAMPLES "Build examples" OFF)
if(POLYCPP_FORWARDED_BUILD_EXAMPLES)
    file(GLOB POLYCPP_FORWARDED_EXAMPLES CONFIGURE_DEPENDS examples/*.cpp)
    foreach(example_source IN LISTS POLYCPP_FORWARDED_EXAMPLES)
        get_filename_component(example_name "${example_source}" NAME_WE)
        set(example_target "polycpp_forwarded_example_${example_name}")
        add_executable("${example_target}" "${example_source}")
        target_link_libraries("${example_target}" PRIVATE polycpp::forwarded)
        set_target_properties("${example_target}" PROPERTIES
            OUTPUT_NAME "${example_name}"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
        )
    endforeach()
endif()

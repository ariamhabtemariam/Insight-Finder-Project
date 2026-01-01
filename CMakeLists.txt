cmake_minimum_required(VERSION 3.16)
project(decoderscpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GoogleTest via FetchContent
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Project sources / headers
file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Remove main.cpp from library sources if it's in src/
list(REMOVE_ITEM PROJECT_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

# Core library with all your logic
add_library(decoderscpp_lib ${PROJECT_SOURCES} ${PROJECT_HEADERS})
target_include_directories(decoderscpp_lib PUBLIC src)
target_link_libraries(decoderscpp_lib PRIVATE curl)

# CLI executable (uses main.cpp)
add_executable(cli src/main.cpp)
target_link_libraries(cli PRIVATE decoderscpp_lib)

# Tests
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/tests/*.cpp"
)

add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests
    PRIVATE
        decoderscpp_lib
        GTest::gtest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)
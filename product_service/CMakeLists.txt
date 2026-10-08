cmake_minimum_required(VERSION 3.16)
project(product_service)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED IMPORTED_TARGET libpqxx)
include(FetchContent)
enable_testing()

FetchContent_Declare(
   googletest
   GIT_REPOSITORY https://github.com/google/googletest.git
   GIT_TAG v1.14.0
   GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(googletest)
FetchContent_Declare(
   Crow
   GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
   GIT_TAG v1.2.1.2
   GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Crow)
add_library(product_lib
    config.cpp
    db.cpp
    product_repository.cpp
    product_routes.cpp
    product_validation.cpp
)
target_include_directories(product_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(product_service
   main.cpp)
target_link_libraries(product_lib
    PUBLIC Crow::Crow PkgConfig::PQXX)
    target_link_libraries(product_service
    PRIVATE product_lib
)
target_precompile_headers(product_lib PRIVATE <crow.h>)
add_executable(product_tests tests/validation_test.cpp)
target_link_libraries(product_tests PRIVATE product_lib GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(product_tests)

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
FetchContent_Declare(
   Crow
   GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
   GIT_TAG v1.2.1.2
   GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Crow)
add_executable(product_service
   main.cpp)
target_link_libraries(product_service PRIVATE Crow::Crow)
target_link_libraries(product_service PRIVATE  PkgConfig::PQXX)
target_precompile_headers(product_service PRIVATE <crow.h>)


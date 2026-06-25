cmake_minimum_required(VERSION 3.16)
project(cpp_app_client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Crow REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(
    CPP_HTTPLIB
    REQUIRED
    IMPORTED_TARGET
    cpp-httplib
)

add_executable(cpp_app_client src/main.cpp)

target_link_libraries(
    cpp_app_client
    PRIVATE
    Crow::Crow
    PkgConfig::CPP_HTTPLIB
)
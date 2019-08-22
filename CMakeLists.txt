# Copyright 2019 Istituto Nazionale di Fisica Nucleare
# Licensed under the EUPL

cmake_minimum_required(VERSION 3.15)

project(life CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        ON)

list(APPEND CMAKE_MODULE_PATH /usr/share/SFML/cmake/Modules)
find_package(SFML COMPONENTS graphics window system REQUIRED)

add_library(warnings INTERFACE)
target_compile_options(warnings INTERFACE -Wall -Wextra)

#set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)
add_executable(life main.cpp)
target_include_directories(life PRIVATE ${SFML_INCLUDE_DIR})
target_link_libraries(life PRIVATE warnings ${SFML_LIBRARIES})

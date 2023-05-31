cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(KBC)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Change working directory to ./build.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})
set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

find_package(ProjectPaths REQUIRED MODULE)
find_package(PublishPaths REQUIRED MODULE)
find_package(CompilerFlags REQUIRED MODULE)
find_package(ConditionalCxxFlag REQUIRED MODULE)

include(Project.cmake)
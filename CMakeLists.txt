cmake_minimum_required(VERSION 3.20)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME ProjectName)
project(${ProjectName} LANGUAGES CXX)

if(NOT WIN32)
message(FATAL_ERROR "SUPPORT ONLY WINDOWS!")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE sources LIST_DIRECTORIES FALSE RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS src/*.c* src/*.h*)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

add_executable(${ProjectName} ${sources})

target_link_libraries(${ProjectName} PRIVATE OpenGL::GL)
target_include_directories(${ProjectName} PRIVATE src)
target_compile_options(${ProjectName} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/MP>)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${ProjectName})

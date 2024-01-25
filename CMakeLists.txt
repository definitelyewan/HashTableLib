cmake_minimum_required(VERSION 3.22.1)
project(HashTableLib VERSION 1.0.0.0)

option(BUILD_SHARED_LIBS "building a shared lib" OFF)
option(BUILD_HTL_DOCS "build docs" OFF)
option(BUILD_HTL_EXAMPLES "build tests" OFF)
option(DEDUB_HTL_SYMBOLS "enables debugging" ON)

set(HTL_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/hashTable.h"
)

set(HTL_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hashTable.c"
)

if(DEBUG_HTL_SYMBOLS)
    add_compile_options("-Wall")
    add_compile_options("-g")
endif()

add_library(HashTableLib ${HTL_HEADERS} ${HTL_SRC})
target_include_directories(HashTableLib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/include/")
set_target_properties(HashTableLib PROPERTIES C_STANDARD 99)

if(BUILD_HTL_EXAMPLES)
    add_subdirectory("examples")
endif()
  
if(BUILD_HTL_DOCS)
    add_subdirectory("docs")
endif()
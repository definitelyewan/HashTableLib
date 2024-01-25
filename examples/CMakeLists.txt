cmake_minimum_required(VERSION 3.22.1)

set(TEST_HASHTABLE_CHAR_EXAMPLE "${CMAKE_CURRENT_SOURCE_DIR}/HashTableCharExample.c")
set(TEST_HASHTABLE_STRUCT_EXAMPLE "${CMAKE_CURRENT_SOURCE_DIR}/HashTableStructExample.c")

add_executable(HashTable_charExample ${TEST_HASHTABLE_CHAR_EXAMPLE})
add_executable(HashTable_structExample ${TEST_HASHTABLE_STRUCT_EXAMPLE})

set_target_properties(HashTable_charExample PROPERTIES C_STANDARD 99)
set_target_properties(HashTable_structExample PROPERTIES C_STANDARD 99)

target_link_libraries(HashTable_charExample PRIVATE HashTableLib)
target_link_libraries(HashTable_structExample PRIVATE HashTableLib)
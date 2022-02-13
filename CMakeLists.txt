# Stolen CMake from you know where) (c) sh0d4n - not author of this perfect CMake and clang-format


cmake_minimum_required (VERSION 3.16)

project ("CParaGL" VERSION 1.0)

find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/sources/*.[ch]pp
        ${PROJECT_SOURCE_DIR}/sources/*.[ch]
        ${PROJECT_SOURCE_DIR}/include/*.[h]pp
        ${PROJECT_SOURCE_DIR}/include/*.[h]
    )

    add_custom_target(
        format
        COMMAND clang-format
        -i
        -style=file
        ${ALL_CXX_SOURCE_FILES}
    )
endif()

set (EXEC_FILE "render")
set (SET_TEST_EXEC "test_VectorN")

set (SOURCES
    sources/main.cpp
)

set (SET_SOURCES
    sources/test_VectorN.cpp
)

set (HEADERS
    include/VectorN.hpp
)

add_executable(${EXEC_FILE} ${SOURCES})
add_executable(${SET_TEST_EXEC} ${SET_SOURCES})

target_include_directories(${EXEC_FILE} PRIVATE include)

set (COMPILER_SETTINGS
    -std=c++2a
    -Wall
    -Wextra
    -Wpedantic

    -Wsign-promo
    -Wsign-conversion
    -Wdouble-promotion

    -Wfloat-equal
    -Wbool-compare

    -Wdiv-by-zero

    -O2
)

target_compile_options(${EXEC_FILE} PRIVATE ${COMPILER_SETTINGS})
target_compile_options(${SET_TEST_EXEC}  PRIVATE ${COMPILER_SETTINGS})
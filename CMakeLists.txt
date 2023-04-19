# ---Choose CMake version---
cmake_minimum_required(VERSION 3.20)

# ---Choose project name---
project(Simple-p6-Setup)

# ---Declare source files---
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} src/boids.cpp include/boids.h include/world.h)
target_include_directories(${PROJECT_NAME} PRIVATE src include)
# Copie le fichier names.txt dans le même dossier que l'exécutable
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/src/names.txt ${CMAKE_BINARY_DIR}/names.txt)

# ---Choose C++ version---
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# ---Choose warning level---
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors)
endif()

# ---Maybe enable warnings as errors---
set(WARNINGS_AS_ERRORS OFF CACHE BOOL "ON iff you want to treat warnings as errors")

if(WARNINGS_AS_ERRORS)
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /WX)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
    endif()
endif()

# ---Disable treating warnings as errors for external folder---
if(NOT MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE "$<$<NOT:$<STREQUAL:$<TARGET_PROPERTY:SOURCE_DIR>,${CMAKE_CURRENT_SOURCE_DIR}/external>>:-Wno-error>")
endif()

# ---Setup Testing---
include(FetchContent)
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest
    GIT_TAG b7c21ec5ceeadb4951b00396fc1e4642dd347e5f
)
FetchContent_MakeAvailable(doctest)
target_link_libraries(${PROJECT_NAME} PRIVATE doctest::doctest)

# ---Add p6 library---
FetchContent_Declare(
    p6
    GIT_REPOSITORY https://github.com/julesfouchy/p6
    GIT_TAG b9e6366e1198abdd7bc835e6125f9ff5b89a75b8
)
FetchContent_MakeAvailable(p6)
target_link_libraries(${PROJECT_NAME} PRIVATE p6::p6)
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 11)
project(minecraft
        VERSION 1.0
        DESCRIPTION "CS 126 Final"
        LANGUAGES CXX)

# This tells the compiler to not aggressively optimize and
# to include debugging information so that the debugger
# can properly read what's going on.
# set(CMAKE_BUILD_TYPE Debug)

# Let's ensure -std=c++xx instead of -std=g++xx
set(CMAKE_CXX_EXTENSIONS OFF)

# Let's nicely support folders in IDE's
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Warning flags
if (MSVC)
    # warning level 3 and all warnings as errors
    add_compile_options(/W3 /WX)
else ()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wpedantic -Werror)
endif ()

# FetchContent added in CMake 3.11, downloads during the configure step
include(FetchContent)

# FetchContent_MakeAvailable was not added until CMake 3.14
if (${CMAKE_VERSION} VERSION_LESS 3.14)
    include(cmake/add_FetchContent_MakeAvailable.cmake)
endif ()

FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v2.5.0
)

# Adds Catch2 testing library
FetchContent_GetProperties(catch2)
if (NOT catch2_POPULATED)
    FetchContent_Populate(catch2)
    add_library(catch2 INTERFACE)
    target_include_directories(catch2 INTERFACE ${catch2_SOURCE_DIR}/single_include)
endif ()

FetchContent_Declare(
        fastnoise
        GIT_REPOSITORY https://github.com/Auburn/FastNoise.git
        GIT_TAG master
)

# Adds fastnoise library
FetchContent_GetProperties(fastnoise)
if (NOT fastnoise_POPULATED)
    FetchContent_Populate(fastnoise)
    add_library(fastnoise INTERFACE)
    target_include_directories(fastnoise INTERFACE ${fastnoise_SOURCE_DIR}/Cpp)
endif ()

get_filename_component(CINDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../" ABSOLUTE)
get_filename_component(APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/" ABSOLUTE)

include("${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake")

# App-specific files
list(APPEND SOURCE_FILES src/core/camera.cc)
list(APPEND SOURCE_FILES src/core/world.cc)
list(APPEND SOURCE_FILES src/core/block.cc)
list(APPEND SOURCE_FILES src/core/texture.cc)
list(APPEND SOURCE_FILES src/game_engine.cc)

# Testing files
list(APPEND TEST_FILES tests/core/world_test.cc)

ci_make_app(
        APP_NAME minecraft
        CINDER_PATH ${CINDER_PATH}
        SOURCES apps/cinder_app_main.cc ${SOURCE_FILES}
        INCLUDES include
        LIBRARIES catch2 fastnoise
)

ci_make_app(
        APP_NAME        minecraft-test
        CINDER_PATH     ${CINDER_PATH}
        SOURCES         tests/test_main.cc ${SOURCE_FILES} ${TEST_FILES}
        INCLUDES        include
        LIBRARIES       catch2 fastnoise
)


if (MSVC)
    set_property(TARGET ideal-gas-test APPEND_STRING PROPERTY LINK_FLAGS " /SUBSYSTEM:CONSOLE")
endif ()
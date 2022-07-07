# stop if cmake version below 3.12
cmake_minimum_required(VERSION 3.12)

macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

project(GBSM DESCRIPTION "Glowbuzzer Stepper Control Linux" VERSION "0.0.1" LANGUAGES C)
message(STATUS "GB: We are building [${PROJECT_NAME}]")


file(GLOB SOURCES src/*.c src/tmc4361a_conf/*.c src/tmc5160_conf/*.c libs/TMC-API/TMC4361A/*.c libs/TMC-API/TMC5160/*.c)

configure_file(version/version.h.in version/version.h)

#if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
#    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
#endif()

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "GB: No build type was provided, we will default to [Release]")
    set(CMAKE_BUILD_TYPE "Release")
else ()
    message(STATUS "GB: CMAKE_BUILD_TYPE is [${CMAKE_BUILD_TYPE}]")
endif ()


# Find the path to the pigpio includes.
find_path(pigpio_INCLUDE_DIR
        NAMES pigpio.h pigpiod_if.h pigpiod_if2.h
        HINTS /usr/local/include)

# Find the pigpio libraries.
find_library(pigpio_LIBRARY
        NAMES libpigpio.so
        HINTS /usr/local/lib)
find_library(pigpiod_if_LIBRARY
        NAMES libpigpiod_if.so
        HINTS /usr/local/lib)
find_library(pigpiod_if2_LIBRARY
        NAMES libpigpiod_if2.so
        HINTS /usr/local/lib)

# Set the pigpio variables to plural form to make them accessible for
# the paramount cmake modules.
set(pigpio_INCLUDE_DIRS ${pigpio_INCLUDE_DIR})
set(pigpio_INCLUDES     ${pigpio_INCLUDE_DIR})

# Handle REQUIRED, QUIET, and version arguments
# and set the <packagename>_FOUND variable.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(pigpio
        DEFAULT_MSG
        pigpio_INCLUDE_DIR pigpio_LIBRARY pigpiod_if_LIBRARY pigpiod_if2_LIBRARY)


#print_all_variables()

message(STATUS "GB: PROJECT_BINARY_DIR [${PROJECT_BINARY_DIR} ]")
message(STATUS "GB: CURRENT_SOURCE_DIR [${CMAKE_CURRENT_SOURCE_DIR} ]")

#find_package(Curses REQUIRED)

add_executable(GBSM ${SOURCES})

target_link_libraries(GBSM Logging common-misc cia state_machine dpm  minini linux-shm pid sleep gbsm_config m pigpio pthread rt)
target_include_directories(GBSM PUBLIC "inc" inc/tmc4361a_conf inc/tmc5160_conf libs/TMC-API/helpers libs/TMC-API/TMC5160 libs/TMC-API/TMC4361A ${PROJECT_BINARY_DIR}/version ${gbsm_config_BINARY_DIR})



add_compile_definitions(GB_APP_LINUX)
MESSAGE(STATUS "GB: [TARGET_GBSM_LINUX] is being used")
if (FLAVOUR STREQUAL "PI")
    MESSAGE(STATUS "GB: Loading [gbsm_defs_pi.cmake]")
    include(gbsm_defs_pi.cmake)
    if (EXISTS gbsm_custom_defs_pi.cmake)
        MESSAGE(STATUS "GB: Loading [gbsm_custom_defs_pi.cmake]")
        include(gbsm_custom_defs_pi.cmake)
    else ()
        MESSAGE(STATUS "GB: [gbsm_custom_def_pi.cmake] does NOT exist (no overrides for standard definitions will be used)")
    endif ()
else ()
    MESSAGE(STATUS "GB: Loading [gbsm_defs_linux.cmake]")
    include(gbsm_defs_linux.cmake)
    if (EXISTS gbsm_custom_defs_linux.cmake)
        MESSAGE(STATUS "GB: Loading [gbsm_custom_defs_linuxc.make]")
        include(gbsm_custom_defs_linux.cmake)
    else ()
        MESSAGE(STATUS "GB: [gbsm_custom_defs_linux.cmake] does NOT exist (no overrides for standard definitions will be used)")
    endif ()

endif ()
configure_file(gbsm_config_autogen.h.in libs/gbsm_config/gbsm_config_autogen.h)

add_subdirectory("libs/gbsm_config")


add_subdirectory("gclibs/common-misc")
add_subdirectory("gclibs/bcm2835")
add_subdirectory("gclibs/logging")
add_subdirectory("gclibs/pid")
add_subdirectory("gclibs/cia")
add_subdirectory("gclibs/dpm")
add_subdirectory("gclibs/linux-shm")
add_subdirectory("gclibs/sleep")
add_subdirectory("gclibs/gberror")
add_subdirectory("gclibs/minini")
add_subdirectory("gclibs/state_machine")


if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR “GB: In-source builds not allowed! Create a build directory and run CMake from there.”)
endif ()



install(TARGETS GBSM DESTINATION bin CONFIGURATIONS Release)
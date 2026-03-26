cmake_minimum_required(VERSION 3.25)
project(PODT VERSION 0.1.0 LANGUAGES C CXX)

option(PODT_ENABLE_WARNINGS "Enable compiler warnings for PODT targets." ON)
option(PODT_ENABLE_TESTING "Enable CTest smoke tests." ON)

list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(NTL REQUIRED)
find_package(GMP REQUIRED)

if (NOT TARGET NTL::NTL)
    add_library(NTL::NTL UNKNOWN IMPORTED)
    set_target_properties(NTL::NTL PROPERTIES
        IMPORTED_LOCATION "${NTL_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${NTL_INCLUDE_PATHS}"
    )
endif ()

if (NOT TARGET GMP::GMP)
    add_library(GMP::GMP UNKNOWN IMPORTED)
    set_target_properties(GMP::GMP PROPERTIES
        IMPORTED_LOCATION "${GMP_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${GMP_INCLUDE_PATHS}"
    )
endif ()

add_library(podt_project_options INTERFACE)
target_compile_features(podt_project_options INTERFACE cxx_std_11)

add_library(podt_warnings INTERFACE)
if (PODT_ENABLE_WARNINGS)
    if (MSVC)
        target_compile_options(podt_warnings INTERFACE /W4 /permissive-)
    else ()
        target_compile_options(podt_warnings INTERFACE -Wall -Wextra -Wpedantic)
    endif ()
endif ()

function(podt_configure_target target_name)
    target_link_libraries("${target_name}" PRIVATE podt_warnings)
    target_link_libraries("${target_name}" PUBLIC podt_project_options)
endfunction()

if (PODT_ENABLE_TESTING)
    include(CTest)
endif ()

add_subdirectory(common)
add_subdirectory(ODTE_SH)
add_subdirectory(PVODTE_MS)
add_subdirectory(PVODTE_MH)

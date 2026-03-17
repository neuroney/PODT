cmake_minimum_required(VERSION 3.25)
project(PVODTE VERSION 0.1.0)

# Global settings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(NTL REQUIRED)
find_package(GMP REQUIRED)

include_directories(${NTL_INCLUDE_PATHS} ${GMP_INCLUDE_PATHS})


# Add common library first
add_subdirectory(common)

# Add all subprojects
add_subdirectory(ODTE_SH)
add_subdirectory(PVODTE_MS)
add_subdirectory(PVODTE_MH)
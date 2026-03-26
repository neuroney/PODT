add_library(common STATIC
    src/helper.cpp
    src/elgamal.cpp
)

target_include_directories(common PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(common
    PUBLIC
        NTL::NTL
        GMP::GMP
)

podt_configure_target(common)

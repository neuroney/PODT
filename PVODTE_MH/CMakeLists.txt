add_executable(PVODTE_MH
    src/VHSS.cpp
    src/PVODTE_MH.cpp
    src/PVODTE_MH_tester.cpp
    test/main.cpp
)

target_include_directories(PVODTE_MH PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(PVODTE_MH
    PRIVATE
        common
        m
)

podt_configure_target(PVODTE_MH)

if (BUILD_TESTING)
    add_test(
        NAME PVODTE_MH_smoke
        COMMAND PVODTE_MH --quick
    )
endif ()

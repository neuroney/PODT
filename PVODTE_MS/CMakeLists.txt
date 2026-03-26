add_executable(PVODTE_MS
    src/HSS.cpp
    src/PVODTE_MS.cpp
    src/PVODTE_MS_tester.cpp
    test/main.cpp
)

target_include_directories(PVODTE_MS PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(PVODTE_MS
    PRIVATE
        common
        m
)

podt_configure_target(PVODTE_MS)

if (BUILD_TESTING)
    add_test(
        NAME PVODTE_MS_smoke
        COMMAND PVODTE_MS --quick
    )
endif ()

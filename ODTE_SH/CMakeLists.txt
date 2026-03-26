add_executable(ODTE_SH
    src/HSS.cpp
    src/ODTE_SH.cpp
    src/ODTE_SH_tester.cpp
    test/main.cpp
)

target_include_directories(ODTE_SH PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(ODTE_SH
    PRIVATE
        common
        m
)

podt_configure_target(ODTE_SH)

if (BUILD_TESTING)
    add_test(
        NAME ODTE_SH_smoke
        COMMAND ODTE_SH --quick
    )
endif ()

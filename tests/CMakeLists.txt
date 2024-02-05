enable_testing()

set(INSTALL_GTEST OFF)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  # cmake-lint: disable=C0103

set(TEST_SOURCES
        ${CMAKE_SOURCE_DIR}/tests/main.cpp
        ${CMAKE_SOURCE_DIR}/tests/test_main.cpp
)

add_executable(sunshine_test ${TEST_SOURCES})
target_link_libraries(sunshine_test
        ${SUNSHINE_EXTERNAL_LIBRARIES}
        ${EXTRA_LIBS}
        ${SUNSHINE_TARGET_FILES}
        gtest
)
target_compile_definitions(sunshine_test PUBLIC ${SUNSHINE_DEFINITIONS})
set_target_properties(sunshine_test PROPERTIES CXX_STANDARD 17
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR})
target_compile_options(sunshine_test PRIVATE ${SUNSHINE_COMPILE_OPTIONS})

add_test(NAME sunshine_test COMMAND sunshine_test)

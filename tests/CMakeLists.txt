cmake_minimum_required(VERSION 3.13)
# https://github.com/google/oss-policies-info/blob/main/foundational-cxx-support-matrix.md#foundational-c-support

project(sunshine_tests)
include_directories("${CMAKE_SOURCE_DIR}")

enable_testing()

# Add GoogleTest directory to the project
set(GTEST_SOURCE_DIR "${CMAKE_SOURCE_DIR}/third-party/googletest")
add_subdirectory("${GTEST_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/googletest")
include_directories("${GTEST_SOURCE_DIR}/googletest/include" "${GTEST_SOURCE_DIR}")

set(CPACK_COMPONENT_GTEST_DISABLED ON)
set(CPACK_COMPONENT_GMOCK_DISABLED ON)

#list(REMOVE_ITEM CPACK_COMPONENTS_ALL
#        "gmock"
#        "gtest")

#set_target_properties(gtest PROPERTIES EXCLUDE_FROM_ALL TRUE)
#set_target_properties(gmock PROPERTIES EXCLUDE_FROM_ALL TRUE)
#
#set(INSTALL_GTEST OFF)
#set(INSTALL_GMOCK OFF)

# if windows
if (WIN32)
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  # cmake-lint: disable=C0103
endif ()

set(TEST_SOURCES
        ${CMAKE_SOURCE_DIR}/tests/main.cpp
        ${CMAKE_SOURCE_DIR}/tests/test_main.cpp
)

set(SUNSHINE_SOURCES
        ${SUNSHINE_TARGET_FILES})

# remove main.cpp from the list of sources
list(REMOVE_ITEM SUNSHINE_SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)

add_executable(${PROJECT_NAME}
        ${TEST_SOURCES}
        ${SUNSHINE_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME}
        ${SUNSHINE_EXTERNAL_LIBRARIES}
        gtest
        gtest_main
        ${PLATFORM_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${SUNSHINE_DEFINITIONS})
target_compile_options(${PROJECT_NAME} PRIVATE ${SUNSHINE_COMPILE_OPTIONS})
target_link_options(${PROJECT_NAME} PRIVATE)

add_test(NAME ${PROJECT_NAME} COMMAND sunshine_test)

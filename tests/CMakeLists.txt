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

# coverage
set(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS "-fprofile-arcs -ftest-coverage")

# if windows
if (WIN32)
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  # cmake-lint: disable=C0103
endif ()

file(GLOB_RECURSE TEST_SOURCES
        ${CMAKE_SOURCE_DIR}/tests/main.cpp
        ${CMAKE_SOURCE_DIR}/tests/conftest.cpp
        ${CMAKE_SOURCE_DIR}/tests/test_*.cpp)

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

# copy repo tree to build target directory
# this is used for building docs in the tests
# todo: this is somewhat slow, is there a better way for the tests to access the repo tree?
file(GLOB_RECURSE ALL_REPO_FILES "${CMAKE_SOURCE_DIR}/*")  # Collect all files in the source directory
foreach(FILE ${ALL_REPO_FILES})  # Filter out unwanted directories and files
    string(REGEX MATCH
            "^${CMAKE_SOURCE_DIR}/(.git.+|.idea|build|build-.+|cmake-.+|docker|gh-pages-template|node_modules|packaging|scripts|tests|third-party|tools|venv)"
            EXCLUDE_FILE ${FILE})
    string(REGEX MATCH
            "^${CMAKE_SOURCE_DIR}/third-party/build-deps"  # todo: match only ffmpeg build for this OS and architecture
            INCLUDE_FILE ${FILE})
    if((NOT EXCLUDE_FILE) OR INCLUDE_FILE)
        list(APPEND INCLUDED_REPO_FILES ${FILE})
    endif()
endforeach()
message("Included repo files: ${INCLUDED_REPO_FILES}")
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD  # Remove the repo directory from the build directory
        COMMAND ${CMAKE_COMMAND} -E remove_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/repo")
foreach(FILE ${INCLUDED_REPO_FILES})  # Copy the filtered files
    file(RELATIVE_PATH RELATIVE_FILE ${CMAKE_SOURCE_DIR} ${FILE})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${FILE} "$<TARGET_FILE_DIR:${PROJECT_NAME}>/repo/${RELATIVE_FILE}")
endforeach()
message("Destination directory: $<TARGET_FILE_DIR:${PROJECT_NAME}>/repo")

add_test(NAME ${PROJECT_NAME} COMMAND sunshine_test)

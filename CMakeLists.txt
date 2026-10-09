cmake_minimum_required(VERSION 3.24)
project(embedded_multiprocessor_template VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(PROJECT_BUILD_TESTS "Build host tests" ON)

add_library(project_core
    src/core/status.c
    src/ipc/freshness.c
    platforms/host/platform_time.c
    platforms/host/platform_memory.c
)
target_include_directories(project_core PUBLIC include)
target_compile_options(project_core PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Werror)

add_executable(project_host_smoke platforms/host/main.c)
target_link_libraries(project_host_smoke PRIVATE project_core)

if(PROJECT_BUILD_TESTS)
    enable_testing()
    add_library(unity STATIC tests/unity/unity.c)
    target_include_directories(unity PUBLIC tests/unity)
    target_compile_options(unity PRIVATE -Wall -Wextra -Wpedantic -Werror)

    add_executable(project_unit_tests
        tests/unity/test_runner.c
        tests/unit/status_test.c
        tests/unit/freshness_unity_test.c
    )
    target_link_libraries(project_unit_tests PRIVATE project_core unity)
    target_include_directories(project_unit_tests PRIVATE include tests/unity)
    target_compile_options(project_unit_tests PRIVATE -Wall -Wextra -Wpedantic -Werror)
    add_test(NAME project_unit_tests COMMAND project_unit_tests)
endif()

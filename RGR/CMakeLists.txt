cmake_minimum_required(VERSION 3.10)
project(typing_trainer C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JANSSON REQUIRED jansson)

include_directories(
    ${JANSSON_INCLUDE_DIRS}
    include
)

add_executable(typing_trainer
    src/main.c
    src/stats.c
    src/training.c
)

target_link_libraries(typing_trainer
    ${JANSSON_LIBRARIES}
    m
)

install(TARGETS typing_trainer DESTINATION bin)
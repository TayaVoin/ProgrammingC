cmake_minimum_required(VERSION 3.10)
project(SortMatrix)

enable_testing()
find_package(cmocka REQUIRED)

add_executable(matrix_tests tests.c matrix.c)
target_link_libraries(matrix_tests cmocka)

add_test(NAME Matrix COMMAND matrix_tests)

set(CTEST_OUTPUT_ON_FAILURE ON)
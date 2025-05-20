#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "matrix.h"

static void test_determinant_1x1(void **state) {
    (void)state;
    double matrix[1][1] = {{5}};
    assert_true(determinant(matrix, 1) == 1);
}

static void test_printMatrix(void **state) {
    (void)state;
    double matrix[2][2] = {{1.5, 2.5}, {3.5, 4.5}};
    printMatrix(matrix, 2);
    assert_true(1);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_determinant_1x1),
        cmocka_unit_test(test_printMatrix),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
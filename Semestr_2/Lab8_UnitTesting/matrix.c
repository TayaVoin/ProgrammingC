#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

double determinant(double matrix[MAX_SIZE][MAX_SIZE], int n) {
    if (n <= 0 || n > MAX_SIZE) {
        fprintf(stderr, "Invalid matrix size: %d\n", n);
        exit(EXIT_FAILURE);
    }
    
    if (n == 1) {
        return matrix[0][0];
    }

    double det = 0;
    for (int col = 0; col < n; col++) {
        double minor[MAX_SIZE][MAX_SIZE];

        for (int i = 1; i < n; i++) {
            int minor_col = 0;
            for (int j = 0; j < n; j++) {
                if (j == col) continue;
                minor[i - 1][minor_col] = matrix[i][j];
                minor_col++;
            }
        }
        double sign = (col % 2 == 0) ? 1 : -1;
        det += sign * matrix[0][col] * determinant(minor, n - 1);
    }
    return det;
}

void printMatrix(double matrix[MAX_SIZE][MAX_SIZE], int N) {
    if (N <= 0 || N > MAX_SIZE) {
        fprintf(stderr, "Invalid matrix dimension: %d\n", N);
        return;
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%8.4lf ", matrix[i][j]);
        }
        printf("\n");
    }
}
#include "student.h"

void insertionSort(struct Student students[], int n) {
    for (int i = 1; i < n; i++) {
        struct Student key = students[i];
        int j = i - 1;
        while (j >= 0 && students[j].total < key.total) {
            students[j + 1] = students[j];
            j--;
        }
        students[j + 1] = key;
    }
}

void countingSort(struct Student students[], int n) {
    struct Student output[n];
    int count[301] = {0};

    for (int i = 0; i < n; i++) {
        count[students[i].total]++;
    }

    int index = 0;
    for (int i = 300; i >= 0; i--) {
        while (count[i] > 0) {
            output[index].total = i;
            index++;
            count[i]--;
        }
    }

    for (int i = 0; i < n; i++) {
        students[i] = output[i];
    }
}

void quickSort(struct Student students[], int low, int high) {
    if (low < high) {
        int pivot = students[high].total;
        int i = (low - 1);

        for (int j = low; j < high; j++) {
            if (students[j].total > pivot) {
                i++;
                struct Student temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }

        struct Student temp = students[i + 1];
        students[i + 1] = students[high];
        students[high] = temp;

        int pi = i + 1;

        quickSort(students, low, pi - 1);
        quickSort(students, pi + 1, high);
    }
}
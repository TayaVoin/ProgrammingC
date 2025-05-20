#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "student.h"

int main() {
    struct Student students[N];
    const char* names[] = {
        "Андрей", "Маша", "Медведь", "Ель", "Виктория", "Кузя", "Иван", "Мурзя", "Татьяна"
    };
    int numNames = sizeof(names) / sizeof(names[0]);

    srand(time(NULL));

    for (int i = 0; i < N; i++) {
        const char* name = names[rand() % numNames];
        int math = rand() % 101;
        int phy = rand() % 101;
        int inf = rand() % 101;
        students[i] = addStudent(name, math, phy, inf);
    }

    printf("Список студентов до сортировки:\n");
    for (int i = 0; i < N; i++) {
        printStudentInfo(students[i]);
    }

    clock_t start = clock();
    insertionSort(students, N);
    clock_t end = clock();
    double time_spent_insert = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\nСписок студентов после сортировки вставками:\n");
    for (int i = 0; i < N; i++) {
        printStudentInfo(students[i]);
    }
    printf("\nВремя работы сортировки вставками для N=%d: %f секунд\n", N, time_spent_insert);

    for (int i = 0; i < N; i++) {
        const char* name = names[rand() % numNames];
        int math = rand() % 101;
        int phy = rand() % 101;
        int inf = rand() % 101;
        students[i] = addStudent(name, math, phy, inf);
    }

    start = clock();
    countingSort(students, N);
    end = clock();
    double time_spent_counting = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\nСписок студентов после сортировки подсчетом:\n");
    for (int i = 0; i < N; i++) {
        printStudentInfo(students[i]);
    }
    printf("\nВремя работы сортировки подсчетом для N=%d: %f секунд\n", N, time_spent_counting);

    for (int i = 0; i < N; i++) {
        const char* name = names[rand() % numNames];
        int math = rand() % 101;
        int phy = rand() % 101;
        int inf = rand() % 101;
        students[i] = addStudent(name, math, phy, inf);
    }

    start = clock();
    quickSort(students, 0, N - 1);
    end = clock();
    double time_spent_quick = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\nСписок студентов после быстрой сортировки:\n");
    for (int i = 0; i < N; i++) {
        printStudentInfo(students[i]);
    }
    printf("\nВремя работы быстрой сортировки для N=%d: %f секунд\n", N, time_spent_quick);

    printCPUInfo();

    return 0;
}
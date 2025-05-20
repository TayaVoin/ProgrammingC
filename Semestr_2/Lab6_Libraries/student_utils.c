#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

struct Student addStudent(const char* name, int math, int phy, int inf) {
    struct Student newStudent;
    strncpy(newStudent.name, name, NAME_LENGTH - 1);
    newStudent.name[NAME_LENGTH - 1] = '\0';
    newStudent.math = math;
    newStudent.phy = phy;
    newStudent.inf = inf;
    newStudent.total = math + phy + inf;
    return newStudent;
}

void printStudentInfo(struct Student student) {
    printf("Имя: %s, Математика: %d, Физика: %d, Информатика: %d, Общий балл: %d\n",
           student.name, student.math, student.phy, student.inf, student.total);
}

void printCPUInfo() {
    printf("Информация о процессоре:\n");
    system("lscpu | grep 'Model name'");
    system("lscpu | grep 'CPU MHz'");
}
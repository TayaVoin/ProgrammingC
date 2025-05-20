#ifndef STUDENT_H
#define STUDENT_H

#define N 100
#define NAME_LENGTH 64

struct Student {
    char name[NAME_LENGTH];
    int math;
    int phy;
    int inf;
    int total;
};

struct Student addStudent(const char* name, int math, int phy, int inf);
void printStudentInfo(struct Student student);
void insertionSort(struct Student students[], int n);
void countingSort(struct Student students[], int n);
void quickSort(struct Student students[], int low, int high);
void printCPUInfo();

#endif
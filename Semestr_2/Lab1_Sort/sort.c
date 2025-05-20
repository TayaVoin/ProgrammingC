#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 100 // Количество студентов
#define NAME_LENGTH 64 // Максимальная длина имени студента

//Изменение 2 для гита
//Изменение 3 для гита

// Определяем структуру для студента
struct Student {
    char name[NAME_LENGTH];
    int math;
    int phy;
    int inf;
    int total;
};

// Функция для добавления студента
struct Student addStudent(const char* name, int math, int phy, int inf) {
    struct Student newStudent;
    strncpy(newStudent.name, name, NAME_LENGTH - 1);
    newStudent.name[NAME_LENGTH - 1] = '\0'; // Обеспечиваем нуль-терминатор
    newStudent.math = math;
    newStudent.phy = phy;
    newStudent.inf = inf;
    newStudent.total = math + phy + inf; // Общий балл
    return newStudent;
}

// Функция для вывода информации о студенте
void printStudentInfo(struct Student student) {
    printf("Имя: %s, Математика: %d, Физика: %d, Информатика: %d, Общий балл: %d\n",
           student.name, student.math, student.phy, student.inf, student.total);
}

// Функция для сортировки студентов по общему баллу (Insertion Sort)
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

// Функция для сортировки студентов по общему баллу (Counting Sort)
void countingSort(struct Student students[], int n) {
    struct Student output[n];
    int count[301] = {0}; // Максимальный возможный общий балл = 300

    // Подсчёт количества студентов для каждого общего балла
    for (int i = 0; i < n; i++) {
        count[students[i].total]++;
    }

    // Построение выходного массива
    int index = 0;
    for (int i = 300; i >= 0; i--) {
        while (count[i] > 0) {
            output[index].total = i;
            index++;
            count[i]--;
        }
    }

    // Копирование отсортированного массива в исходный массив
    for (int i = 0; i < n; i++) {
        students[i] = output[i];
    }
}

// Функция для быстрой сортировки (QuickSort)
void quickSort(struct Student students[], int low, int high) {
    if (low < high) {
        int pivot = students[high].total; // Опорный элемент
        int i = (low - 1); // Индекс меньшего элемента

        for (int j = low; j < high; j++) {
            if (students[j].total > pivot) { // Убираем по убыванию
                i++;
                struct Student temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }

        struct Student temp = students[i + 1];
        students[i + 1] = students[high];
        students[high] = temp;

        int pi = i + 1; // Индекс опорного элемента

        quickSort(students, low, pi - 1); // Сортируем элементы до и после опорного
        quickSort(students, pi + 1, high);
    }
}

// Функция для вывода информации о процессоре
void printCPUInfo() {
    printf("Информация о процессоре:\n");
    system("lscpu | grep 'Model name'");
    system("lscpu | grep 'CPU MHz'");
}

int main() {
    struct Student students[N];
    const char* names[] = {
        "Андрей", "Маша", "Медведь", "Ель", "Виктория", "Кузя", "Иван", "Мурзя", "Татьяна"
    };
    int numNames = sizeof(names) / sizeof(names[0]);

    // Инициализируем генератор случайных чисел
    srand(time(NULL));

    // Заполняем массив студентов случайными данными
    for (int i = 0; i < N; i++) {
        const char* name = names[rand() % numNames];
        int math = rand() % 101; // Оценка по математике
        int phy = rand() % 101; // Оценка по физике
        int inf = rand() % 101; // Оценка по информатике
        students[i] = addStudent(name, math, phy, inf);
    }

    // Выводим информацию о студентах до сортировки
    printf("Список студентов до сортировки:\n");
    for (int i = 0; i < N; i++) {
        printStudentInfo(students[i]);
    }

    // Замер времени сортировки вставками
    clock_t start = clock();
    insertionSort(students, N);
    clock_t end = clock();
    double time_spent_insert = (double)(end - start) / CLOCKS_PER_SEC;

    // Выводим информацию о студентах после сортировки вставками
    printf("\nСписок студентов после сортировки вставками:\n");
    for (int i = 0; i < N; i++) {
        printStudentInfo(students[i]);
    }
    printf("\nВремя работы сортировки вставками для N=%d: %f секунд\n", N, time_spent_insert);

    // Перезаполняем студентов для сортировки подсчетом
    for (int i = 0; i < N; i++) {
        const char* name = names[rand() % numNames];
        int math = rand() % 101;
        int phy = rand() % 101;
        int inf = rand() % 101;
        students[i] = addStudent(name, math, phy, inf);
    }

    // Замер времени сортировки подсчетом
    start = clock();
    countingSort(students, N);
    end = clock();
    double time_spent_counting = (double)(end - start) / CLOCKS_PER_SEC;

    // Выводим информацию о студентах после сортировки подсчетом
    printf("\nСписок студентов после сортировки подсчетом:\n");
    for (int i = 0; i < N; i++) {
        printStudentInfo(students[i]);
    }
    printf("\nВремя работы сортировки подсчетом для N=%d: %f секунд\n", N, time_spent_counting);

    // Перезаполняем студентов для быстрой сортировки
    for (int i = 0; i < N; i++) {
        const char* name = names[rand() % numNames];
        int math = rand() % 101;
        int phy = rand() % 101;
        int inf = rand() % 101;
        students[i] = addStudent(name, math, phy, inf);
    }

    // Замер времени быстрой сортировки
    start = clock();
    quickSort(students, 0, N - 1);
    end = clock();
    double time_spent_quick = (double)(end - start) / CLOCKS_PER_SEC;

    // Выводим информацию о студентах после быстрой сортировки
    printf("\nСписок студентов после быстрой сортировки:\n");
    for (int i = 0; i < N; i++) {
        printStudentInfo(students[i]);
    }
    printf("\nВремя работы быстрой сортировки для N=%d: %f секунд\n", N, time_spent_quick);

    // Вывод информации о процессоре
    printCPUInfo();

    return 0;
}
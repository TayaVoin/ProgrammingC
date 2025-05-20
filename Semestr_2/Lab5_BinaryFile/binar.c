#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char start_point[50];
    char end_point[50];
    int route_number;
} Route;

void input_route(Route *route) {
    printf("Введите название начального пункта: ");
    scanf("%s", route->start_point);
    printf("Введите название конечного пункта: ");
    scanf("%s", route->end_point);
    printf("Введите номер маршрута: ");
    scanf("%d", &route->route_number);
}

void print_route(const Route *route) {
    printf("Начальный пункт: %s\n", route->start_point);
    printf("Конечный пункт: %s\n", route->end_point);
    printf("Номер маршрута: %d\n", route->route_number);
    printf("----------------------------\n");
}

int main() {
    int N;
    printf("Введите количество маршрутов: ");
    scanf("%d", &N);

    FILE *file = fopen("Spravka.dat", "wb");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        Route route;
        printf("\nВведите данные для маршрута %d:\n", i + 1);
        input_route(&route);
        fwrite(&route, sizeof(Route), 1, file);
    }
    fclose(file);

    int choice;
    printf("\nВыберите поле для поиска:\n");
    printf("1. Название начального пункта\n");
    printf("2. Название конечного пункта\n");
    printf("3. Номер маршрута\n");
    printf("Ваш выбор: ");
    scanf("%d", &choice);

    char search_string[50];
    int search_number;
    int found = 0;

    // Запрашиваем поисковое значение ДО цикла чтения файла
    switch (choice) {
        case 1:
            printf("Введите название начального пункта для поиска: ");
            scanf("%s", search_string);
            break;
        case 2:
            printf("Введите название конечного пункта для поиска: ");
            scanf("%s", search_string);
            break;
        case 3:
            printf("Введите номер маршрута для поиска: ");
            scanf("%d", &search_number);
            break;
        default:
            printf("Неверный выбор.\n");
            return 1;
    }

    file = fopen("Spravka.dat", "rb");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return 1;
    }

    Route route;
    while (fread(&route, sizeof(Route), 1, file) == 1) {
        switch (choice) {
            case 1:
                if (strcmp(route.start_point, search_string) == 0) {
                    print_route(&route);
                    found = 1;
                }
                break;
            case 2:
                if (strcmp(route.end_point, search_string) == 0) {
                    print_route(&route);
                    found = 1;
                }// string compare
                break;
            case 3:
                if (route.route_number == search_number) {
                    print_route(&route);
                    found = 1;
                }
                break;
        }
    }

    fclose(file);

    if (!found) {
        printf("Данные не найдены.\n");
    }

    return 0;
}
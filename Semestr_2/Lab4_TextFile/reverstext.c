#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Функция для реверсирования строки
void reverse_string(char *str) {
    int length = strlen(str);
    for (int i = 0; i < length / 2; i++) {
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

int main(int argc, char *argv[]) {
    // Проверка аргументов командной строки
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <файл>\n", argv[0]);
        return 1;
    }

    // Открываем файл для чтения
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return 1;
    }

    // Переменные для хранения самой длинной строки
    char longest_line[1024] = "";
    char current_line[1024];

    // Чтение файла построчно
    while (fgets(current_line, sizeof(current_line), file) != NULL) {
        // Удаляем символ новой строки, если он есть
        current_line[strcspn(current_line, "\n")] = '\0';

        // Если текущая строка длиннее самой длинной, обновляем самую длинную
        if (strlen(current_line) > strlen(longest_line)) {
            strcpy(longest_line, current_line);
        }
    }

    // Закрываем файл
    fclose(file);

    // Если файл был пуст, завершаем программу
    if (strlen(longest_line) == 0) {
        printf("Файл пуст.\n");
        return 0;
    }

    // Реверсируем самую длинную строку
    reverse_string(longest_line);

    // Открываем файл для записи (в режиме добавления)
    file = fopen(argv[1], "a");
    if (file == NULL) {
        perror("Ошибка открытия файла для записи");
        return 1;
    }

    // Записываем реверсированную строку в файл
    fprintf(file, "\n%s\n", longest_line);

    // Закрываем файл
    fclose(file);

    printf("Самая длинная строка записана в обратном порядке в файл.\n");
    return 0;
}
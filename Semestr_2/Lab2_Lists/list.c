#include <stdio.h>
#include <stdlib.h>

//Изменение для тестирования гита
//Изменение 5

// Структура узла
struct node {
    int data;
    struct node* next;
    struct node* prev;
};
typedef struct node node_t;

// Функция для создания нового узла
node_t* newNode(int data) {
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

// Функция для добавления узла в конец списка
void append(node_t** head, int data) {
    node_t* new = newNode(data);
    if (*head == NULL) {
        *head = new;
        return;
    }
    node_t* cur = *head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = new;
    new->prev = cur;
}

// Функция для вывода списка на экран
void printList(node_t* head) {
    node_t* cur = head;
    while (cur != NULL) {
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("\n");
}

// Функция для навигации по списку
void navigateList(node_t* head) {
    node_t* current = head;
    char input;

    while (1) {
        printf("Текущее значение: %d\n", current->data);
        printf("Введите команду (W/A/S/D или 2/4/6/8): ");
        scanf(" %c", &input);

        switch (input) {
            case 'D':
            case '6':
                if (current->next != NULL) {
                    current = current->next;
                } else {
                    printf("Достигнут конец списка. Вернуться в начало? (Y/N): ");
                    scanf(" %c", &input);
                    if (input == 'Y' || input == 'y') {
                        current = head;
                    }
                }
                break;
            case 'A':
            case '4':
                if (current->prev != NULL) {
                    current = current->prev;
                } else {
                    printf("Достигнуто начало списка.\n");
                }
                break;
            case 'S':
            case '2':
                // Перемещение вниз (если есть такая возможность)
                // Здесь можно добавить логику для перемещения вниз, если это необходимо
                break;
            case 'W':
            case '8':
                // Перемещение вверх (если есть такая возможность)
                // Здесь можно добавить логику для перемещения вверх, если это необходимо
                break;
            default:
                printf("Неверная команда.\n");
        }
    }
}

// Функция для объединения двух списков по схеме
node_t* mergeLists(node_t* list1, node_t* list2) {
    node_t* merged = NULL; // Голова объединённого списка
    node_t* tail = NULL;   // Хвост объединённого списка

    node_t* cur1 = list1;
    node_t* cur2 = list2;

    while (cur1 != NULL || cur2 != NULL) {
        // Добавляем элемент из первого списка
        if (cur1 != NULL) {
            if (merged == NULL) {
                merged = cur1;
                tail = cur1;
            } else {
                tail->next = cur1;
                cur1->prev = tail;
                tail = cur1;
            }
            cur1 = cur1->next;
        }

        // Добавляем элемент из второго списка
        if (cur2 != NULL) {
            if (merged == NULL) {
                merged = cur2;
                tail = cur2;
            } else {
                tail->next = cur2;
                cur2->prev = tail;
                tail = cur2;
            }
            cur2 = cur2->next;
        }
    }

    return merged;
}

int main() {
    node_t* list1 = NULL;
    node_t* list2 = NULL;
    int data;

    // Ввод первого списка
    printf("Введите элементы первого списка (0 для завершения):\n");
    while (1) {
        scanf("%d", &data);
        if (data == 0) {
            break;
        }
        append(&list1, data);
    }

    // Ввод второго списка
    printf("Введите элементы второго списка (0 для завершения):\n");
    while (1) {
        scanf("%d", &data);
        if (data == 0) {
            break;
        }
        append(&list2, data);
    }
// Вывод списков
    printf("Первый список:\n");
    printList(list1);
printf("Второй список:\n");
    printList(list2);

    // Объединение списков по схеме
    node_t* mergedList = mergeLists(list1, list2);

    // Вывод объединенного списка
    printf("Объединенный список:\n");
    printList(mergedList);

    // Навигация по списку
    navigateList(mergedList);

    // Освобождение памяти
    node_t* cur = mergedList;
    while (cur != NULL) {
        node_t* next = cur->next;
        free(cur);
        cur = next;
    }

    return 0;
}
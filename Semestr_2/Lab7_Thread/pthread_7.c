#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// 1 Создание потока
void* thread_func_basic(void* arg) {
    for (int i = 0; i < 5; i++) {
        printf("Дочерний поток: строка %d\n", i);
    }
    return NULL;
}

void task1() {
    printf("\n=== 1 Создание потока ===\n");
    pthread_t thread;
    pthread_create(&thread, NULL, thread_func_basic, NULL);
    
    for (int i = 0; i < 5; i++) {
        printf("Родительский поток: строка %d\n", i);
    }
    
    pthread_join(thread, NULL);
}

// 2 Ожидание потока
void* thread_func_wait(void* arg) {
    for (int i = 0; i < 5; i++) {
        printf("Дочерний поток: строка %d\n", i);
        sleep(1);
    }
    return NULL;
}

void task2() {
    printf("\n=== 2 Ожидание потока ===\n");
    pthread_t thread;
    pthread_create(&thread, NULL, thread_func_wait, NULL);
    
    pthread_join(thread, NULL);
    
    for (int i = 0; i < 5; i++) {
        printf("Родительский поток: строка %d\n", i);
    }
}

// 3 Параметры потока
void* thread_func_params(void* arg) {
    char** messages = (char**)arg;
    for (int i = 0; messages[i] != NULL; i++) {
        printf("%s\n", messages[i]);
        sleep(1);
    }
    return NULL;
}

void task3() {
    printf("\n=== 3 Параметры потока ===\n");
    pthread_t threads[4];
    char* messages[4][5] = {
        {"Поток 1: строка 1", "Поток 1: строка 2", NULL},
        {"Поток 2: строка 1", NULL},
        {"Поток 3: строка 1", "Поток 3: строка 2", "Поток 3: строка 3", NULL},
        {"Поток 4: строка 1", "Поток 4: строка 2", "Поток 4: строка 3", "Поток 4: строка 4", NULL}
    };
    
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, thread_func_params, messages[i]);
    }
    
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
}

// 4 Завершение потока без ожидания
void* thread_func_cancel(void* arg) {
    for (int i = 0; i < 5; i++) {
        sleep(1);
        printf("Дочерний поток: строка %d\n", i);
    }
    return NULL;
}

void task4() {
    printf("\n=== 4 Завершение потока без ожидания ===\n");
    pthread_t thread;
    pthread_create(&thread, NULL, thread_func_cancel, NULL);
    
    sleep(2);
    pthread_cancel(thread);
    
    printf("Родительский поток завершил дочерний\n");
}

// 5 Обработка завершения потока
void cleanup(void* arg) {
    printf("Очистка: поток завершается\n");
}

void* thread_func_cleanup(void* arg) {
    pthread_cleanup_push(cleanup, NULL);
    
    for (int i = 0; i < 5; i++) {
        sleep(1);
        printf("Дочерний поток: строка %d\n", i);
    }
    
    pthread_cleanup_pop(0);
    return NULL;
}

void task5() {
    printf("\n=== 5 Обработка завершения потока ===\n");
    pthread_t thread;
    pthread_create(&thread, NULL, thread_func_cleanup, NULL);
    
    sleep(2);
    pthread_cancel(thread);
    pthread_join(thread, NULL);
}

// 6 Sleepsort
void* sleep_sort(void* arg) {
    int value = *(int*)arg;
    sleep(value);
    printf("%d ", value);
    return NULL;
}

void task6() {
    printf("\n=== 6 Sleepsort ===\n");
    int arr[] = {3, 1, 4, 2, 5};
    int n = sizeof(arr)/sizeof(arr[0]);
    pthread_t threads[n];
    
    printf("Исходный массив: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\nОтсортированный массив: ");
    
    for (int i = 0; i < n; i++) {
        pthread_create(&threads[i], NULL, sleep_sort, &arr[i]);
    }
    
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("\n");
}

// 7 Синхронизированный вывод
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int turn = 0;

void* thread_func_sync(void* arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);
        if (turn == 1) {
            printf("Дочерний поток: строка %d\n", i);
            turn = 0;
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void task7() {
    printf("\n=== 7 Синхронизированный вывод ===\n");
    pthread_t thread;
    pthread_create(&thread, NULL, thread_func_sync, NULL);
    
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);
        if (turn == 0) {
            printf("Родительский поток: строка %d\n", i);
            turn = 1;
        }
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_join(thread, NULL);
}

// 8 Перемножение матриц
typedef struct {
    int** a;
    int** b;
    int** c;
    int size;
    int start_row;
    int end_row;
} matrix_args;

void* multiply(void* arg) {
    matrix_args* args = (matrix_args*)arg;
    for (int i = args->start_row; i < args->end_row; i++) {
        for (int j = 0; j < args->size; j++) {
            args->c[i][j] = 0;
            for (int k = 0; k < args->size; k++) {
                args->c[i][j] += args->a[i][k] * args->b[k][j];
            }
        }
    }
    return NULL;
}

int** create_matrix(int size, int init_value) {
    int** matrix = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        matrix[i] = (int*)malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            matrix[i][j] = init_value;
        }
    }
    return matrix;
}

void free_matrix(int** matrix, int size) {
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void print_matrix(int** matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void task8(int matrix_size, int num_threads) {
    printf("\n=== 8 Перемножение матриц %dx%d (%d потоков) ===\n", 
           matrix_size, matrix_size, num_threads);
    
    // Инициализация матриц
    int** a = create_matrix(matrix_size, 1);
    int** b = create_matrix(matrix_size, 1);
    int** c = create_matrix(matrix_size, 0);
    
    // Создание потоков
    pthread_t threads[num_threads];
    matrix_args args[num_threads];
    int rows_per_thread = matrix_size / num_threads;
    
    clock_t start = clock();
    
    for (int i = 0; i < num_threads; i++) {
        args[i].a = a;
        args[i].b = b;
        args[i].c = c;
        args[i].size = matrix_size;
        args[i].start_row = i * rows_per_thread;
        args[i].end_row = (i == num_threads-1) ? matrix_size : (i+1)*rows_per_thread;
        
        pthread_create(&threads[i], NULL, multiply, &args[i]);
    }
    
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    clock_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    printf("Время выполнения: %.2f мс\n", time);
    
    // Вывод для маленьких матриц
    if (matrix_size < 5) {
        printf("Матрица A:\n");
        print_matrix(a, matrix_size);
        printf("Матрица B:\n");
        print_matrix(b, matrix_size);
        printf("Результат (A x B):\n");
        print_matrix(c, matrix_size);
    }
    
    // Освобождение памяти
    free_matrix(a, matrix_size);
    free_matrix(b, matrix_size);
    free_matrix(c, matrix_size);
}

// Структура очереди сообщений
typedef struct {
    char* messages[10];
    int count;
    int head;
    int tail;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
    bool shutdown;
} message_queue;

// Инициализация очереди
void init_queue(message_queue* q) {
    q->count = 0;
    q->head = 0;
    q->tail = 0;
    q->shutdown = false;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);
}

// Уничтожение очереди
void destroy_queue(message_queue* q) {
    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->not_empty);
    pthread_cond_destroy(&q->not_full);
}

// Функция производителя (клиента)
int msgSend(message_queue* q, const char* msg) {
    pthread_mutex_lock(&q->mutex);
    
    while (q->count == 10 && !q->shutdown) {
        pthread_cond_wait(&q->not_full, &q->mutex);
    }
    
    if (q->shutdown) {
        pthread_mutex_unlock(&q->mutex);
        return -1;
    }
    
    // Копируем сообщение (обрезаем до 128 символов)
    char* copy = strndup(msg, 128);
    q->messages[q->tail] = copy;
    q->tail = (q->tail + 1) % 10;
    q->count++;
    
    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->mutex);
    return strlen(copy);
}

// Функция потребителя (сервера)
int msgRecv(message_queue* q, char* buf, size_t bufsize) {
    pthread_mutex_lock(&q->mutex);
    
    while (q->count == 0 && !q->shutdown) {
        pthread_cond_wait(&q->not_empty, &q->mutex);
    }
    
    if (q->shutdown && q->count == 0) {
        pthread_mutex_unlock(&q->mutex);
        return -1;
    }
    
    // Извлекаем сообщение
    char* msg = q->messages[q->head];
    size_t len = strlen(msg);
    size_t copy_size = len < bufsize-1 ? len : bufsize-1;
    strncpy(buf, msg, copy_size);
    buf[copy_size] = '\0';
    
    free(msg);
    q->head = (q->head + 1) % 10;
    q->count--;
    
    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->mutex);
    return copy_size;
}

// Функция для разблокировки всех ожидающих потоков
void msgDrop(message_queue* q) {
    pthread_mutex_lock(&q->mutex);
    q->shutdown = true;
    pthread_cond_broadcast(&q->not_empty);
    pthread_cond_broadcast(&q->not_full);
    pthread_mutex_unlock(&q->mutex);
}

// Функция сервера (потребителя)
void* server_func(void* arg) {
    message_queue* q = (message_queue*)arg;
    char buf[128];
    while (1) {
        int len = msgRecv(q, buf, sizeof(buf));
        if (len == -1) break;
        printf("[Сервер] Получено: %s\n", buf);
        sleep(rand() % 2 + 1);
    }
    printf("[Сервер] Завершение работы\n");
    return NULL;
}

// Функция клиента (производителя)
void* client_func(void* arg) {
    message_queue* q = (message_queue*)arg;
    const char* messages[] = {"Привет", "Как дела?", "Что нового?", "Пока"};
    int num_messages = sizeof(messages)/sizeof(messages[0]);
    
    for (int i = 0; i < 5; i++) {
        const char* msg = messages[rand() % num_messages];
        printf("[Клиент] Отправка: %s\n", msg);
        msgSend(q, msg);
        sleep(rand() % 2 + 1);
    }
    
    printf("[Клиент] Завершение работы\n");
    return NULL;
}

void task9() {
    printf("\n=== 9 Производитель-потребитель ===\n");
    message_queue q;
    init_queue(&q);
    
    pthread_t server_thread, client_thread1, client_thread2;
    pthread_create(&server_thread, NULL, server_func, &q);
    pthread_create(&client_thread1, NULL, client_func, &q);
    pthread_create(&client_thread2, NULL, client_func, &q);
    
    pthread_join(client_thread1, NULL);
    pthread_join(client_thread2, NULL);
    
    // Даем серверу время обработать оставшиеся сообщения
    sleep(3);
    msgDrop(&q);
    pthread_join(server_thread, NULL);
    
    destroy_queue(&q);
}

//Главная функция

int main() {
    srand(time(NULL));
    task1();
    task2();
    task3();
    task4();
    task5();
    task6();
    task7();
    task8(4, 2);  // Матрица 4x4, 2 потока
    task8(100, 4); // Матрица 100x100, 4 потока
    task9();
    return 0;
}
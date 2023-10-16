#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define N 512
#define M 512

int matrix[N][M];

void *find_max(void *arg) {
    int *col = (int *) arg;
    int max = matrix[0][*col];
    for (int i = 1; i < N; i++) {
        if (matrix[i][*col] > max) {
            max = matrix[i][*col];
        }
    }
    return (void *) max;
}

int main() {
    // Заполнение матрицы случайными числами
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            matrix[i][j] = rand() % 100;
        }
    }

    // Создание массива потоков
    pthread_t threads[M];

    // Запуск потоков
    for (int i = 0; i < M; i++) {
        pthread_create(&threads[i], NULL, find_max, (void *) &i);
    }

    // Получение результатов
    int max_values[M];
    for (int i = 0; i < M; i++) {
        void *result;
        pthread_join(threads[i], &result);
        max_values[i] = (int) result;
    }

    // Вывод результатов
    for (int i = 0; i < M; i++) {
        printf("%d ", max_values[i]);
    }
    printf("\n");

    return 0;
}

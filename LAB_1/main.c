#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 512
#define M 512

int A[N][M];

struct Args {
    int col_begin;
    int col_end;
    int* max;
};

void *find_max(void *arg) {
    struct Args args = *(struct Args*) arg;
    //printf("Start %d\n", args.col_begin);
    for (int j = args.col_begin; j < args.col_end; ++j) {
        args.max[j] = A[0][j];
        for (int i = 0; i < N; ++i) {
            if (A[i][j] > args.max[j]) {
                args.max[j] = A[i][j];
            }
        }
    }
    //printf("End %d\n", args.col_end);
    return NULL;
}

int main(int argc, char *argv[]) {
    // Заполнение матрицы случайными числами
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            A[i][j] = rand();
        }
    }

    // Поиск максимальных значений в столбцах элементов матрицы 𝐴
    int max[M];
    int num_threads = atoi(argv[1]);
    printf("Threads: %d\n", num_threads);

    pthread_t threads[num_threads];
    struct Args args[num_threads];
    for (int thread_id = 0; thread_id < num_threads; ++thread_id) {
        args[thread_id].col_begin = M / num_threads * thread_id;
        args[thread_id].col_end = M / num_threads * (thread_id + 1);
        //printf("%d: %d - %d\n", thread_id, args[thread_id].col_begin, args[thread_id].col_end);
        args[thread_id].max = max;
    }
    args[num_threads - 1].col_end = M;
    clock_t start_time = time(NULL);
    for (int thread_id = 0; thread_id < num_threads; ++thread_id) {
        pthread_create(&threads[thread_id], NULL, find_max, &args[thread_id]);
    }
    for (int thread_id = 0; thread_id < num_threads; ++thread_id) {
        pthread_join(threads[thread_id], NULL);
    }
    clock_t end_time = time(NULL);

    // Вывод результатов
    printf("Максимальные значения в столбцах элементов матрицы 𝐴:\n");
    for (int j = 0; j < M; j++) {
        //printf("%d\n", max[j]);
    }
    printf("\n");
    //double time = (end_time - start_time);
    printf("Время выполнения программы: %ld секунд\n", (end_time - start_time));

    return 0;
}

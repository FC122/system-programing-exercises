#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#define ROWS 100
#define COLS 100
#define NUM_THREADS 8

float matrix[ROWS][COLS];
float paralelSum = 0;
float normal_matrix[ROWS][COLS];
float normalSum = 0;
clock_t t, t1;
double time_taken_normal = 0, time_taken_paralel = 0;

void *fill_matrix(void *arg) {
    int *thread_id = (int *)arg;
    int start_row = *thread_id * ROWS / NUM_THREADS;
    int end_row = (*thread_id + 1) * ROWS / NUM_THREADS;

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < COLS; j++) {
            float sum = 0;
            for (int k = 0; k <= i; k++) {
                sum += k * sin(j) - j * cos(k);
            }
            matrix[i][j] = sum;
            paralelSum += matrix[i][j];
        }
    }
    return NULL;
}

void fill_matrix_normal() {
    t = clock();
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            float sum = 0;
            for (int k = 0; k <= i; k++) {
                sum += k * sin(j) - j * cos(k);
            }
            normal_matrix[i][j] = sum;
            normalSum += normal_matrix[i][j];
        }
    }
    time_taken_normal = (double)(clock() - t) / CLOCKS_PER_SEC;
    printf(" No threads: %f - time %f\n", normalSum, time_taken_normal);
}

void fill_matrix_paralel() {
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];

    t1 = clock();
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, fill_matrix, &thread_args[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    time_taken_paralel = (double)(clock() - t1) / CLOCKS_PER_SEC;
    printf("Threads: %f - time: %f\n", paralelSum, time_taken_paralel);
}

int main() {
    fill_matrix_paralel();
    //fill_matrix_normal();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define CACHE_LINE_SIZE 64
#define CACHE_SIZE 32768

#define NUM_COLS (CACHE_LINE_SIZE /sizeof(int32_t))   
#define NUM_ROWS (CACHE_SIZE / NUM_COLS)   

#define PREFETCH_FOR_READ 0
#define PREFETCH_FOR_WRITE 1

#define LOCALITY_HIGH 3
#define LOCALITY_MODERATE 2
#define LOCALITY_LOW 1
#define LOCALITY_NONE 0

// timer variables
static struct timespec start, stop;

// Function to time access to an array in row major order
double time_access_row_major(int **array, int rows, int cols) {
    volatile int temp = 0; // Volatile to prevent optimization by the compiler

    clock_gettime(CLOCK_REALTIME, &start);

    // Access the array in a row-major fashion
    for (int i = 0; i < rows; i++) {
        __builtin_prefetch (&array[i + 1][0], PREFETCH_FOR_READ, LOCALITY_HIGH);
        for (int j = 0; j < cols; j++) {
            temp = array[i][j]; // Access element
        }
    }

    clock_gettime(CLOCK_REALTIME, &stop);
    
    return ( (stop.tv_sec - start.tv_sec) + ( (stop.tv_nsec - start.tv_nsec)  / 1e9 ) );
}

// Function to time access to an array in column major order
double time_access_col_major(int **array, int rows, int cols) {
    volatile int temp = 0; // Volatile to prevent optimization by the compiler

    clock_gettime(CLOCK_REALTIME, &start);

    // Access the array in a row-major fashion
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            __builtin_prefetch (&array[j + 1][0], PREFETCH_FOR_READ, LOCALITY_HIGH);
            temp = array[j][i]; // Access element
        }
    }

    clock_gettime(CLOCK_REALTIME, &stop);
    
    return ( (stop.tv_sec - start.tv_sec) + ( (stop.tv_nsec - start.tv_nsec)  / 1e9 ) );
}

int main() {
    int32_t **array;
    int i, j;

    // Allocate a 2D array (NUM_ROWS x NUM_COLS)
    array = (int32_t **)malloc(NUM_ROWS * sizeof(int32_t *));

    for (i = 0; i < NUM_ROWS; i++) {
        array[i] = (int32_t *)malloc(NUM_COLS * sizeof(int32_t));
    }

    // Initialize the array with values
    for (i = 0; i < NUM_ROWS; i++) {
        for (j = 0; j < NUM_COLS; j++) {
            array[i][j] = i * NUM_COLS + j;
        }
    }

    // Time the access to the array (Row-major access pattern)
    double access_time = time_access_row_major(array, NUM_ROWS, NUM_COLS);
    printf("Access time for row-major order: %f seconds\n", access_time);

    // Time the access to the array (Column-major access pattern)
    access_time = time_access_col_major(array, NUM_ROWS, NUM_COLS);
    printf("Access time for column-major order: %f seconds\n", access_time);

    // Free the allocated memory
    for (i = 0; i < NUM_ROWS; i++) {
        free(array[i]);
    }
    free(array);

    return 0;
}
//
// Created by Malogulko, Alexey on 01/03/2020.
// This is just shared code for other matrix multipliers
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const int SQUARE = 2;
const double NUM_MAX = 10.0;

struct matrixInfo {
    double *mxPtr;
    int size;
};

void check_partition(int matrix_size, int partition_size) {
    if (matrix_size%partition_size != 0) {
        fprintf(stderr, "matrix size must leave no remainder when divided by partition size");
        exit(1);
    }
}

// Parses args
void parse_args(int argc, char *argv[], int *size, int *num_partitions) {
    if (argc >= 3) {
        if (sscanf(argv[1], "%i", size) != 1) {
            fprintf(stderr, "error - not an integer");
            exit(1);
        }
        if (sscanf(argv[2], "%i", num_partitions) != 1) {
            fprintf(stderr, "error - not an integer");
            exit(1);
        }
    } else {
        printf("Please use <matrix size> <number of partitions> args");
        exit(1);
    }
    check_partition(*size, *num_partitions);
}

void parse_matrix_size(int argc, char *argv[], int *size) {
    if (argc >= 2) {
        if (sscanf(argv[1], "%i", size) != 1) {
            fprintf(stderr, "error - not an integer");
            exit(1);
        }
    } else {
        printf("Please use <matrix size>");
        exit(1);
    }
}

// Allocates matrix memory
double *matrix_malloc(int size) {
    return (double *) malloc((int) pow(size, SQUARE) * sizeof(double));
}

void print_matrix_row_wise(double *matrix, int size) {
    for (int i = 0; i < size; i++) {
        printf("[ ");
        for (int j = 0; j < size; j++) {
            printf("%f ", *(matrix + i * size  + j));
        }
        printf("]\n");
    }
}

void print_matrix_col_wise(double *matrix, int size) {
    for (int i = 0; i < size; i++) {
        printf("[ ");
        for (int j = 0; j < size; j++) {
            printf("%f ", *(matrix + j * size  + i));
        }
        printf("]\n");
    }
}

double *vector_malloc(int size) {
    return (double *) malloc((int) size * sizeof(double));
}

void print_vector(double *vector, int size) {
    for (int i = 0; i < size; i++) {
        printf("[ %f ]\n", *(vector+i));
    }
}

// Generates SQUARE matrix of the size x size
void *random_matrix(void *input) {
    double *mx;
    int size;
    struct matrixInfo *info = (struct matrixInfo *) input;
    mx = info->mxPtr;
    size = info->size;
    for (int i = 0; i < size * size; i++) {
        *(mx + i) = (double) rand() / RAND_MAX * (NUM_MAX * 2) - NUM_MAX;
    }
    return NULL;
}
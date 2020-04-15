//
// Created by Alex Malogulko on 13/01/2020.
//
//
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include "utils.c"

struct rowSumPartitionReq {
    double *matrix_a;
    double *matrix_b;
    double *inf_norm;
    double *row_sum_vector;
    int size;
    int partition_num; // partition number (0..1,2,n)
    int partition_rows; // number of rows in the partition
};

void row_sum(double *matrix_c, double *row_sum_vector, int size) {
    for (int i = 0; i < size; i++) {
        double *vec_sum = row_sum_vector + i;
        for (int j = 0; j < size; j++) {
            *(vec_sum) += *(matrix_c + i * size + j);
        }
    }
}

void ijk_row_sum_partition(void *input) {
    struct rowSumPartitionReq *info = (struct rowSumPartitionReq *) input;
    int partition_start = info->partition_num * info->partition_rows;
    int next_partition_start = (info->partition_num + 1) * info->partition_rows;

    for (int i = partition_start; i < next_partition_start; i++) {
        double row_sum = 0;
        for (int j = 0; j < info->size; j++) {
            for (int k = 0; k < info->size; k++) {
                row_sum += *(info->matrix_a + i * info->size + k) * *(info->matrix_b + j * info->size + k);
            }
        }
        *(info->row_sum_vector + i) = row_sum;
        if (*info->inf_norm < row_sum) {
            *info->inf_norm = row_sum;
        }
    }
}

void ijk_row_sum_partitioned(double *matrix_a,
                             double *matrix_b,
                             double *row_sum_vector,
                             double *inf_norm,
                             int matrix_size,
                             int num_partitions) {
    check_partition(matrix_size, num_partitions);
    *inf_norm = 0;
    int partition_size = matrix_size / num_partitions;
    for (int p_num = 0; p_num <= num_partitions; p_num++) {
        struct rowSumPartitionReq req = {
                .matrix_a = matrix_a,
                .matrix_b = matrix_b,
                .inf_norm = inf_norm,
                .row_sum_vector = row_sum_vector,
                .size = matrix_size,
                .partition_num = p_num,
                .partition_rows = partition_size
        };
        ijk_row_sum_partition(&req);
    }
}


void ijk(double *matrix_a, double *matrix_b, double *matrix_c, int matrix_size) {
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            double *c_sum = matrix_c + i * matrix_size + j;
            for (int k = 0; k < matrix_size; k++) {
                *(c_sum) += *(matrix_a + i * matrix_size + k) * *(matrix_b + j * matrix_size + k);
            }
        }
    }
}

/**
 * 4x4 matrix represented in memory as:
 *
 * 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16
 *
 * Matrices A and C are stored in row-wise format:
 *
 * 01 02 03 04
 * 05 06 07 08
 * 09 10 11 12
 * 13 14 15 16
 *
 * At the same time, matrix B blocks stored in column-wise format:
 *
 * 01 05 09 13
 * 02 06 10 14
 * 03 07 11 15
 * 04 08 12 16
 *
 */
int main(int argc, char *argv[]) {
    int size, num_partitions;
    pthread_t thread_ids[2];
    struct timespec start, end;
    // Uncomment this if you want the matrices to be actually random
    //srand(time(0));
    parse_args(argc, argv, &size, &num_partitions);
    double *matrix_a = matrix_malloc(size);
    double *matrix_b = matrix_malloc(size);
    double *inf_norm = malloc(sizeof(double));
    // Initialize info objs
    struct matrixInfo matrix_a_info = {.size = size, .mxPtr = matrix_a};
    struct matrixInfo matrix_b_info = {.size = size, .mxPtr = matrix_b};
    // Fill matrices with random values (parallelism is just for fun here)
    pthread_create(&thread_ids[0], NULL, random_matrix, &matrix_a_info);
    pthread_create(&thread_ids[1], NULL, random_matrix, &matrix_b_info);

    double *matrix_c = matrix_malloc(size); // result matrix
    double *row_sum_vector = vector_malloc(size); // result vector with max
    double *row_sum_vector_backup = vector_malloc(size); // result vector with max

    // Blocking until join is finished
    for (int tn = 0; tn < 2; tn++) {
        (void) pthread_join(thread_ids[tn], 0);
    }

    //*** Commenting out the prints ***/

    // printf("Matrix A stripe:\n");
    // for (int s = 0; s < size * size; s++) {
    //     printf("%f ", *(matrix_a + s));
    // }
    // printf("\nMatrix A:\n");
    // print_matrix_row_wise(matrix_a, size);

    // printf("Matrix B stripe:\n");
    // for (int s = 0; s < size * size; s++) {
    //     printf("%f ", *(matrix_b + s));
    // }
    // printf("\nMatrix B:\n");
    // print_matrix_col_wise(matrix_b, size);

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    ijk_row_sum_partitioned(matrix_a, matrix_b, row_sum_vector, inf_norm, size, num_partitions);
    //ijk(matrix_a, matrix_b, matrix_c, size);
    //row_sum(matrix_c, row_sum_vector_backup, size);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    //*** Commenting out the prints ***/

    //printf("Matrix C stripe:\n");
    //for (int s = 0; s < size * size; s++) {
    //    printf("%f ", *(matrix_c + s));
    //}
    //printf("\nMatrix C:\n");
    //print_matrix_row_wise(matrix_c, size);

    //printf("Matrix C row SUM:\n");
    //print_vector(row_sum_vector, size);
    //printf("Infinity norm is %f\n", *inf_norm);

    //printf("Matrix C row SUM BACKUP:\n");
    //print_vector(row_sum_vector_backup, size);

    uint64_t delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000; // microseconds
    printf("%d;%d;%llu\n", size, num_partitions, delta_us);
    return 0;
}

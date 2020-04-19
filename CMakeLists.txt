cmake_minimum_required(VERSION 3.15)
project(max_norm_pthread C)
project(max_norm_seq C)

set(CMAKE_C_STANDARD 99)

add_executable(max_norm_pthread matrix_norm_pthread.c)
add_executable(max_norm_seq max_norm_seq.c)


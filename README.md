## What?

This repository has examples for calculating matrices infinity norms with and without pthread.

PLEASE NOTE: This code is only for educational purposes and can only operate on random generated matrices.

## How to use

This is a regular CMake project, just follow the steps below:

```shell script
.build$ cmake ..
-- The C compiler identification is AppleClang 10.0.1.10010046
-- Check for working C compiler: /Library/Developer/CommandLineTools/usr/bin/cc
-- Check for working C compiler: /Library/Developer/CommandLineTools/usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: .build
.build$ ls
CMakeCache.txt	CMakeFiles  Makefile  cmake_install.cmake
.build$ make
Scanning dependencies of target max_norm_seq
[ 25%] Building C object CMakeFiles/max_norm_seq.dir/max_norm_seq.c.o
[ 50%] Linking C executable max_norm_seq
[ 50%] Built target max_norm_seq
Scanning dependencies of target max_norm_pthread
[ 75%] Building C object CMakeFiles/max_norm_pthread.dir/matrix_norm_pthread.c.o
[100%] Linking C executable max_norm_pthread
[100%] Built target max_norm_pthread
```

## Running sequential version

There are 2 arguments this program takes: 

1. Size of the matrix
2. How many partitions are in the matrix(just keep 1 for sequential version)

```shell script
.build$ ./max_norm_seq 100 10
100;10;5141
```

How to read  output:

```
100;10;5141
 |   |   |
 |   |   └-- Time spent in calculation only(microseconds)
 |   └------ How many partitions are in the matrix
 └---------- Size of the matrix
```

## Running pthread version


There are 2 arguments this program takes: 

1. Size of the matrix
2. How many threads are running(matrices are split into this number of equal parts)

```shell script
.build$ ./max_norm_pthread 800 8
800;8;447961
```

How to read  output:

```
800;8;447961
 |   |   |
 |   |   └-- Time spent in calculation only(microseconds)
 |   └------ Number of threads
 └---------- Size of the matrix
```

# 03: Pthreads, openmp

## Task

0. Download the following code and compile it:
    ```
    $ <cc> <codefile.c> -o <executable> -fopenmp
    ```
    Gists: https://gist.github.com/ssstuvz/574c3d1eb663980a5487af2e773d2460
1. Demonstration files - correct code examples for understanding OpenMP. This tasks aren't graded.
    * omp_hello.c - simple program, printing 'Hello world'. Introduction into ```parallel``` construction.
      Tasks:
      * learn how to manually change the number of threads executed in the program by changing the environment variable:

        ```$ export OMP_NUM_THREADS=<number you want>.```
      * learn how to set the number of threads within the program
    * omp_outmes.cpp - comparison of parallel I / O C and C ++. An example on understanding the concept of “threadsafe”. An example of a critical construction. Compilation:

      ```$ g++ OutMes.cpp -o <executable> -fopenmp```

    * omp_privateshared.c - an example of public and private variables, 'for' construction. Task: run the program, figure out what all the commands with ```#pragma``` do.
    * omp_parsec.c - an example of using the section construction. Task: before running, try to guess the output of the program, then check.
    * omp_sumarray.c - an example of using reduction. Pay attention to the ```#pragma omp parallel for``` construction - what is the difference between this construction and ```#pragma omp parallel ...```,  ```#pragma omp for```

2. Programs with bugs - examples of programs that need to be fixed.
* omp_bugreduction.c - code for dot product of two vectors. Add dotprod function, check for bugs in ```#pragma omp```, check the result. (5 points)
* omp_bugparfor.c – find and fix bugs. (5 points)

3. Implement a parallel program that uses the Monte Carlo method to estimate Pi number. Randomly (be careful with the random number generator! An example in the lecture) you throw a point into a unit square. A circle is inscribed in the same square. If the point is in the circle, increase the counter. Then find the ratio of the points in the circle to the total number of points. Knowing the areas of the square and the circle, find approximately Pi. (20 points)

Note: clock() measures the sum of wall clocks across all threads. Use omp_get_wtime() instead (it returns value in seconds).

## Solution

Fixed files: [omp_fixedreduction.c](https://github.com/dmitriyklebanov/made_2020_hpc/tree/master/3/src/omp_fixedreduction.c) and [omp_fixedparfoc.c](https://github.com/dmitriyklebanov/made_2020_hpc/tree/master/3/src/omp_fixedparfoc.c).

Pi calculation: [omp_pi_calc.c](https://github.com/dmitriyklebanov/made_2020_hpc/tree/master/3/src/omp_pi_calc.c).

### Usage

* ```make``` - build *.c files

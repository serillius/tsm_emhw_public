# TSM_EmbHard Laboratories

----

## Marking
There are 4 laboratories with a possible maximum 20% of the final grade mark to be gained

----
## Laboratory 1
In this laboratory we examine testing according to the V-Model. 
The sobel-algorithm code is designed to run on the embedded platform so the framing code in C and any python code will not be in the final release.
This will have an effect on the content of the concepts of verification and validation - which you have to work out for your selves. 

We have prepared a python test environment which can be run using
```python
python -m unittest test_sobel.py 
```  
The file ```test_sobel.py``` contains code for interfacing C-libraries with python code using c-types and is constructed using the unittest facility from python.
The C library can be compiled to a shared object (Linux) using the call

```bash
gcc -fPIC -shared -o ../lib/sobel_algo.so sobel_algo.c 
```

### Exercises (5P)
#### Task 1a (1P): Test Identification
Identify the type of tests contained in the file ```test_sobel.py``` 
Design at least one test missing from the suite to conform to the V-Model.
#### Task 1b (1P)
The unit test is passed but the test is pointless - correct it and the underlying C-code.
#### Task 2 (2P): Unit Test Implementation
Write all necessary tests for the sobel_mac function, be able to explain the reasoning behind the number and form of the tests.
#### Task 3 (1P): Interface Test Implementation
Write an interface test for the path sobel_x->sobel_mac->sobel_x->sobel_sum. Be able to explain your reasoning.

----

## Laboratory 2
In this laboratory we examine loop optimisations.

### Usage
There is a code framework `sobel_main.c` around the sobel algorithm code, `sobel_algo.c`, that is used to call the algorithm implementation in a rich operating system, in this case Linux, so we can use features of the OS that are not available in a bare metal target system.
This framework code is structured to enable multiple runs of the sobel algorithm and measure time taken for each execution run. This code generates a comma separated value file with teh execution times that can be imported into a program like Excel for graphical analysis.

We shall also be using the gnu profiler which we kick off using the command line 
```bash
make profile
```
This command line will enable the creation of a symbol table `gprof.out` during compilation that can be linked with the output of the profiler.
By running the resultant binary using the command
```bash
gprof ./bin/fancy-calc gmon.out > an.txt
```
we generate an output file `an.txt` which contains the results of a profiling run. 

The man-page for `clock_gettime()` may be of help
https://man7.org/linux/man-pages/man3/clock_gettime.3.html

### Exercises (4P)
#### Task 1a (1P): In-code Profiling
Run the code framework and view the execution times in excel and graph them in a histogram. Be prepared to answer the following questions
1. What is the WCET and the BCET?
2. Should there be a difference between the two?
3. Why is there a difference between the two?
4. What kind of accuracy in the execution times are you getting?

#### Task 1b (1P): Profiling using External Toolset
Now run the code using the profiler. Be prepared to answer the following questions
1. Explain the difference between the output of the code and the output of the profiler
2. Which do you think is more accurate and/or useful?
3. What function takes the most time?
4. Which function is worth optimising? 

#### Task 2 (1P): Manual Loop Unrolling
Unroll the sobel_mac loop in the fashion explained in the lecture
1. Does the distribution of WCET/BCET change?
2. Are all the calculations necessary? Eliminate any lines of code (LoC) that produce no result. 
3. Don't forget to regression test making appropriate assumptions (which are?)  

Continue the theme of point 2 above - this time by eliminating unnecessary calculations. Be prepared to answer the same questions as above

Inline the `sobel_mac` code as explained in the lectures - be prepared to answer the same questions as above

#### Task 3 (1P): Automated Loop Unrolling
Revert to the original sobel_algo code and compile using compiler switches for loop unrolling and inlining. 
Compare the results to those achieved above
-----

## Laboratory 3 (6P)
In this laboratory we examine the use of vectorised instructions. 
The ARM architecture has gone through a number of iterations of these using the co-processor NEON. 
We shall be using NEON intrinsics.
ARM documentation is variable, sometimes the assembler instructions are more illuminating than that of the intrinsics so both references are given here.

The SIMD capabilities vary by architecture.
The Raspberry Pi 1 B+ is an ARMv6 architecture - the documentation for the appropriate intrinsics is here:
https://developer.arm.com/documentation/dui0491/i/ARMv6-SIMD-Instruction-Intrinsics

### Exercises
#### Task 1 (6P): Single Instruction Multiple Data Code Implementation
Implement the sobel algorithm, starting from the naive code, using vector instructions. 
Follow the processes learned so far. Be prepared to defend your results.
Vector instruction programing becomes a data-flow problem very quickly so use data-flow analysis tools to plan your implementation. 
-----
## Laboratory 4 (5P)
In this laboratory we examine the use of cache and prefetching instructions

### Exercises
#### Task 1 (1P): Cache Line Size
You have been told that when a word, in the generic sense, is accessed, this access occurs a compulsory miss as this word and all other words that fill a cache-line are loaded into cache. 
In other words the access to the other words in the cache line should be cheaper. Write a program to test this theory on the board.
A good starting point would be to instantiate an array of columns LEVEL1_DCACHE_LINESIZE and some large number of rows (smaller than both cache and page size). 
Under Linux one normally would be able to find out cache details using something like `getconf -a | grep CACHE` but there is a reason the Pi is cheap. 

#### Task 2 (3P): Prefetching
The ARM processor supports prefetching and the gcc interface to this feature is via the
```C
__builtin_prefetch( const void *, int, int )
``` 
call. This call generates architecture specific instructions, i.e. the assembly language generated varies per ARM architecture (Pi1B+ ARMv6, Pi3B+ ARMv8).
Insert prefetch statements into the code you wrote for Task 1 and verify this. 
```bash
gcc -s main.c 
``` 
or
```bash
gcc -S main.c
```
will generate assembler code via the gcc toolset, with or without comments.

Measure the execution time, interpret the results and attempt to derive an optimal prefetch distance. You may find it useful to examine the execution time of the instructions required to sustain the prefetch operation.

#### Task 3 (1P): Tightly Coupled Memory (TCM)
The ARM core used in the Broadcom SoC for the RaspberryPi reputedly supports 4 kB instruction and 4 kB data tightly coupled memory.
Unfortunately it would appear however that Pi1B+ OS does not support/initalise these RAMs.
Linux provides an interface to support ARM TCM. The Linux-kernel documentation has more information.
```C
#include <asm/tcm.h> 
```
Re-write your solution-code for Task 1 using TCM. 

Be prepared to answer
1. questions on how you think your solution should work. 
2. Does the Pi3B+ support TCM (in theory)? If not, explain why you think this is reasonable. 
-----

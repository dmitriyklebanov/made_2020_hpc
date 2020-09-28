# 02: Terminal, environment variables, linking, Makefile, virtual environments

## Task

* Implement matrix multiplication on C/C++ (3 points)
* Split the solution on several modules and build them using static linking (2 points)
* Prepare two build with ``-g`` and ``-O3`` flags respectively and measure execution time of matrix multiplication with matrix sizes ``N = 512, 1024..., 4096`` (3 points)
* Measure execution time of matrix multiplication with matrix on a virtual machine, real OS and (optionally) in the docker (3 points + 0.5 point for the docker)
* Implement the following basic bash scripts (1.5 балла):
  1. Organize FOR loop printing the even numbers only from 100 to 1000
  2. Initialize the array of 10-20 elements and organize FOR loop printing the elements of array
  3. Compute 100 + 0.5 in bash
  4. Check if file ”Linux” exists in present directory. If yes, print message ”course”. If no, print message ”very easy” and create the ”Linux” file with text ”course is easy”.

Bonus points:
* Additional points for using BLAS/cBLAS functions (1.5 points)
* Additional points for calling LINPACK test (1.5 points)
* Superbonus: implement strassen method of matrix multiplication (3 points)

To Pass: 11 points

## Solution

Bash scripts [source code](https://github.com/dmitriyklebanov/made_2020_hpc/tree/master/2/scripts).

Matrix multiplication and shtrassen method [source code](https://github.com/dmitriyklebanov/made_2020_hpc/tree/master/2/src/matrix).

Not implemented:
* Test matrix multiplication in docker
* Use of BLAS/cBLAS functions
* Use of LINPACK test


### Usage

* ```make``` - build Matrix tests
* ```make test_perf``` - launch performance tests for Matrix multiplication
* ```make test_matmul``` - launch tests for Matrix multiplication

### Performance tests results

Measured on:
* Windows 10 Home with Windows Subsystem for Linux 2 (WSL 2): Ubuntu 18.04
* KDE neon 5.19.2 (Ubuntu 20.04 based system)

---
#### *Windows 10 Home with Windows Subsystem for Linux 2 (WSL 2): Ubuntu 18.04*
---
**Flag: '-g'**
|N   |norm, ms |shtrassen, ms |
|----|---------|--------------|
|128 |225      |3857          |
|256 |1667     |27672         |
|512 |15281    |215771        |
|1024|126348   |-             |
|2048|1081416  |-             |
|4096|-        |-             |

**Flag: '-O3'**
|N   |norm, ms |shtrassen, ms |
|----|---------|--------------|
|128 |6        |363           |
|256 |51       |2502          |
|512 |429      |17275         |
|1024|7177     |-             |
|2048|97698    |-             |
|4096|803958   |-             |


---
#### *KDE neon 5.19.2 (Ubuntu 20.04 based system)*
---
**Flag: '-g'**
|N   |norm, ms |shtrassen, ms |
|----|---------|--------------|
|128 |217      |3822          |
|256 |1741     |26879         |
|512 |15594    |217886        |
|1024|135099   |-             |
|2048|1112214  |-             |
|4096|-        |-             |

**Flag: '-O3'**
|N   |norm, ms |shtrassen, ms |
|----|---------|--------------|
|128 |8        |348           |
|256 |57       |2582          |
|512 |471      |19287         |
|1024|7384     |-             |
|2048|99808    |-             |
|4096|782517   |-             |



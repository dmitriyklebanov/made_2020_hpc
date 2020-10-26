# 05: MPI usage

## Task

1. Assume that processes is a company of strangers who get to know each other by playing the followint game:
    * Starting with the first process he randomly chooses another process ```i``` and sends him a message with its name (you could randomly set it).
    * Process ```i``` sends message to another random process ```j``` which hasn't receieved a message yet. This message also contains all names and ranks of previous processes in the right order. Process number ```j``` is only known by process ```i``` so all process should be prepared to receive a message.
    * The game stops after ```N```(number of processes) steps. Use synchronous forwarding MPI_SSend.

    Implement this game using MPI library.
2. Use MPI to parallelize the 1D cellular automaton [Rule 110](https://en.wikipedia.org/wiki/Rule_110):
   * Initialize randomly 1d array with 0 or 1.
   * Depending on itself and its neighbors change cell state (for example, use [Rule 110](https://en.wikipedia.org/wiki/Rule_110#Definition)).
   * Implement periodic and non-periodic boundary conditions (3 points).
   * Parallel code runs on multiple processes (5 points).
   * Implement ghost cells (2 points).
   * Support changing the rule of game (any of 256) (5 points).
   * The graph of the dependence of the program runtime on the number of processes (2 points).
   * Evolution image for any rule (3 points).

   Summary: 20 points
## Solution

1. [Source code](https://github.com/dmitriyklebanov/made_2020_hpc/tree/master/5/1.c).

    Process sleeps random number of seconds $\le 3$ before generating message. Message contains current timestamp.

2. [Source code](https://github.com/dmitriyklebanov/made_2020_hpc/tree/master/5/2/2.c).
    * ```BOUNDARY_STATEMENT``` variable setups periodic/non-periodic boundary conditions:
      *  ```BOUNDARY_STATEMENT = 2``` - periodic boundary condition
      *  ```BOUNDARY_STATEMENT = 0 or 1 ``` - the value of the left neighbor of the first cell and the value of the right neidhbor of the last cell.
   * Supports multiprocessing using MPI library.
   * ```RULE``` variable setups the rule of the game.
   * The [graph](https://github.com/dmitriyklebanov/made_2020_hpc/tree/master/5/2/results/size5040_iter5000.png) of the dependence of the program runtime on the number of processes.
   * Evolution [image](https://github.com/dmitriyklebanov/made_2020_hpc/tree/master/5/2/results/size80_rule30_periodic.png) and [file](https://github.com/dmitriyklebanov/made_2020_hpc/tree/master/5/2/results/size80_rule30_periodic.txt) for rule 30 and periodic boundary conditions.
   * Evolution [image](https://github.com/dmitriyklebanov/made_2020_hpc/tree/master/5/2/results/size80_rule30_non_periodic0.png) and [file](https://github.com/dmitriyklebanov/made_2020_hpc/tree/master/5/2/results/size80_rule30_non_periodic0.txt) for rule 30 and non-periodic boundary conditions (always 0).

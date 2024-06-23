# HW2 – Threads.
## Task Description :

The objective of this assignment is to demonstrate data structures and threading skills by processing an endless data stream efficiently.
The stream simulates security camera data, where the task is to count the number of prime numbers in the stream.
The process should be parallelized to utilize all CPU cores and use no more than 2MB of RAM.
Expected Performance:
At least 4 times better without improving the isPrime function.
At least 10 times better with improving the isPrime function.

### Solution Details :

1. **Parallel Processing**: The prime number counting is parallelized to fully utilize the CPU cores.
2. **Optimized `isPrime` Function**: The `isPrime` function is optimized to reduce computational overhead.
3. **Minimal RAM Usage**: The implementation ensures that the total RAM usage does not exceed 2MB.
4. **Lock-Free Queue**: A lock-free queue is used for efficient communication between threads.

Provided 3 photos for result comparisons between the single thread program you provided (named "tutor") ,a multi thread with your `isPrime` function (named "solutionIsPrime") and my over all solution (named "bestSolution").

## Usage Instructions

To compile the code, run: `make`

To delete all the compiled files (randomGenerator and primeCounter) run `make clean`

To run the solution, use: `./randomGenerator <seed> <num_of_numbers> | ./primeCounter`

Example: `./randomGenerator 10 10000000 | ./primeCounter`

### Measuring Performance

To measure the time taken, use the time command: `./randomGenerator 10 10000000 | /usr/bin/time -v ./primeCounter` (depends on your time function location).

Or just run the `time` command before the `./primeCounter` like so: `./randomGenerator 10 10000000 | time ./primeCounter`

Expected Output: `<total_primes> total primes.`


**Tutor code:**
![Example Image](https://i.imagesup.co/images2/7600a9f0f12563478f142c9669b49a6eaeb664ed.png)


**My code with tutor isPrime function:**
![Example Image](https://i.imagesup.co/images2/71f0d02e12a306ae381c589404c57a9f749c90f7.png)

**My over all best solution:**
![Example Image](https://i.imagesup.co/images2/6eff70e28d321f56c3cf5fc518494e200bc1bd8b.png)

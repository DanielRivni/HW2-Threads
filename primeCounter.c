#define _GNU_SOURCE  // Enable GNU extensions for pthread_setaffinity_np

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <unistd.h>
#include <sched.h>

#define BUFFER_SIZE 35000
#define NUM_THREADS 4

typedef struct {
    int data[BUFFER_SIZE];
    _Atomic int head;
    _Atomic int tail;
} LockFreeQueue;

LockFreeQueue queue;
_Atomic bool done = false;
_Atomic int prime_count = 0;

// Initialize the lock-free queue
void init_queue(LockFreeQueue *queue) {
    atomic_init(&queue->head, 0);
    atomic_init(&queue->tail, 0);
}

// Enqueue function for lock-free queue
bool enqueue(LockFreeQueue *queue, int value) {
    int head, next_head;
    do {
        head = atomic_load_explicit(&queue->head, memory_order_relaxed);
        next_head = (head + 1) % BUFFER_SIZE;
        // Check if the queue is full
        if (next_head == atomic_load_explicit(&queue->tail, memory_order_acquire)) {
            return false;  // Queue is full
        }
    } while (!atomic_compare_exchange_weak_explicit(&queue->head, &head, next_head, 
                                                    memory_order_release, memory_order_relaxed));
    
    queue->data[head] = value;
    return true;
}

// Dequeue function for lock-free queue
bool dequeue(LockFreeQueue *queue, int *value) {
    int tail, next_tail;
    do {
        tail = atomic_load_explicit(&queue->tail, memory_order_relaxed);
        // Check if the queue is empty
        if (tail == atomic_load_explicit(&queue->head, memory_order_acquire)) {
            return false;  // Queue is empty
        }
        *value = queue->data[tail];
        next_tail = (tail + 1) % BUFFER_SIZE;
    } while (!atomic_compare_exchange_weak_explicit(&queue->tail, &tail, next_tail, 
                                                    memory_order_release, memory_order_relaxed));
    
    return true;
}

// // Your isPrime Function
// bool isPrime(int n) {
//     if (n <= 1) {
//         return false;
//     }
//     for (int i = 2; i * i <= n; i++) {
//         if (n % i == 0) {
//             return false;
//         }
//     }
//     return true;
// }


// Function to check if a number is prime
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// Thread function to count prime numbers
void *prime_counter(void *arg) {
    int num;
    int core_id = *((int *)arg);

    // Set thread affinity to a specific core
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);
    if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) != 0) {
        perror("pthread_setaffinity_np");
        return NULL;
    }

    // Process numbers from the queue
    while (true) {
        if (dequeue(&queue, &num)) {
            if (isPrime(num)) {
                atomic_fetch_add_explicit(&prime_count, 1, memory_order_relaxed);
            }
        } else if (atomic_load_explicit(&done, memory_order_acquire) &&
                   atomic_load_explicit(&queue.head, memory_order_acquire) == 
                   atomic_load_explicit(&queue.tail, memory_order_acquire)) {
            break;  // Exit if done and queue is empty
        }
    }
    return NULL;
}

int main() {
    init_queue(&queue);
    pthread_t threads[NUM_THREADS];
    int core_ids[NUM_THREADS];
    
    // Create worker threads
    for (int i = 0; i < NUM_THREADS; i++) {
        core_ids[i] = i;
        if (pthread_create(&threads[i], NULL, prime_counter, &core_ids[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    int num;
    // Read numbers from input and enqueue them
    while (scanf("%d", &num) != EOF) {
        while (!enqueue(&queue, num)) {
            // Busy-waiting until space is available in the queue
        }
    }

    // Indicate that all input has been processed
    atomic_store_explicit(&done, true, memory_order_release);

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Output the total number of primes found
    printf("%d total primes.\n", atomic_load_explicit(&prime_count, memory_order_relaxed));
    
    return 0;
}

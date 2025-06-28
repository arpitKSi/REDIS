#pragma once

#include <stddef.h>
#include <pthread.h>
#include <vector>
#include <deque>

/*
 * Work - A single unit of work for the thread pool
 * ------------------------------------------------
 * Contains a function pointer and its corresponding argument.
 */
struct Work {
    void (*f)(void *) = nullptr;  // Function to execute
    void *arg = nullptr;          // Argument to pass to the function
};

/*
 * ThreadPool - A simple thread pool using POSIX threads
 * -----------------------------------------------------
 * Manages a fixed number of worker threads that wait for and process tasks from a shared queue.
 *
 * Fields:
 *   - threads     : List of pthread_t representing worker threads
 *   - queue       : Queue of tasks to be executed
 *   - mu          : Mutex to protect queue access
 *   - not_empty   : Condition variable used to signal when the queue has work
 */
struct TheadPool {
    std::vector<pthread_t> threads;
    std::deque<Work> queue;
    pthread_mutex_t mu;
    pthread_cond_t not_empty;
};

/*
 * thread_pool_init
 * ----------------
 * Initializes the thread pool and spawns `num_threads` worker threads.
 */
void thread_pool_init(TheadPool *tp, size_t num_threads);

/*
 * thread_pool_queue
 * -----------------
 * Adds a new task to the thread pool’s queue.
 * The task is specified by a function pointer and an argument.
 */
void thread_pool_queue(TheadPool *tp, void (*f)(void *), void *arg);

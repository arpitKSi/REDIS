// thread_pool.cpp
#include <assert.h>
#include "thread_pool.h"

/**
 * worker - Worker thread function
 * @arg: Pointer to TheadPool structure
 * 
 * This function runs in each worker thread. It continuously waits for tasks
 * in the thread pool's queue and executes them. The worker thread will:
 * 1. Lock the mutex to access the shared queue
 * 2. Wait on the condition variable if the queue is empty
 * 3. Dequeue a task when available
 * 4. Release the mutex before executing the task
 * 5. Execute the task function with its argument
 * 
 * This function runs in an infinite loop and never returns.
 */
static void *worker(void *arg) {
    TheadPool *tp = (TheadPool *)arg;
    
    while (true) {
        // Acquire lock to access shared queue
        pthread_mutex_lock(&tp->mu);
        
        // Wait for the condition: a non-empty queue
        // Use while loop to handle spurious wakeups
        while (tp->queue.empty()) {
            pthread_cond_wait(&tp->not_empty, &tp->mu);
        }

        // Dequeue the next task
        Work w = tp->queue.front();
        tp->queue.pop_front();
        
        // Release lock before executing task (allows other threads to queue work)
        pthread_mutex_unlock(&tp->mu);

        // Execute the task function with its argument
        w.f(w.arg);
    }
    return NULL;  // Never reached
}

/**
 * thread_pool_init - Initialize a thread pool
 * @tp: Pointer to TheadPool structure to initialize
 * @num_threads: Number of worker threads to create
 * 
 * Initializes the thread pool by:
 * 1. Setting up the mutex and condition variable for synchronization
 * 2. Creating the specified number of worker threads
 * 
 * Each worker thread will run the worker() function and wait for tasks
 * to be queued. The function will assert-fail if any pthread operations fail.
 * 
 * Preconditions: num_threads > 0, tp is a valid pointer
 */
void thread_pool_init(TheadPool *tp, size_t num_threads) {
    assert(num_threads > 0);

    // Initialize mutex for protecting shared queue access
    int rv = pthread_mutex_init(&tp->mu, NULL);
    assert(rv == 0);
    
    // Initialize condition variable for signaling non-empty queue
    rv = pthread_cond_init(&tp->not_empty, NULL);
    assert(rv == 0);

    // Allocate space for thread handles
    tp->threads.resize(num_threads);
    
    // Create worker threads
    for (size_t i = 0; i < num_threads; ++i) {
        int rv = pthread_create(&tp->threads[i], NULL, &worker, tp);
        assert(rv == 0);
    }
}

/**
 * thread_pool_queue - Add a task to the thread pool queue
 * @tp: Pointer to TheadPool structure
 * @f: Function pointer to the task to execute
 * @arg: Argument to pass to the task function
 * 
 * Adds a new task to the thread pool's work queue. The task consists of
 * a function pointer and its argument. This function is thread-safe and
 * will wake up a waiting worker thread to process the new task.
 * 
 * The function will:
 * 1. Acquire the mutex to protect queue access
 * 2. Add the task to the back of the queue
 * 3. Signal the condition variable to wake up a waiting worker
 * 4. Release the mutex
 * 
 * Preconditions: tp is initialized, f is a valid function pointer
 */
void thread_pool_queue(TheadPool *tp, void (*f)(void *), void *arg) {
    // Acquire lock to safely modify the shared queue
    pthread_mutex_lock(&tp->mu);
    
    // Add new task to the end of the queue
    tp->queue.push_back(Work {f, arg});
    
    // Signal that the queue is no longer empty (wake up a worker)
    pthread_cond_signal(&tp->not_empty);
    
    // Release the lock
    pthread_mutex_unlock(&tp->mu);
}

#ifndef SHARED_MEMORY_HEAP_H
#define SHARED_MEMORY_HEAP_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <chrono>
#include "task_heap.h"

class SharedMemoryHeap
{
private:
    key_t SHM_KEY;
    int shm_id;
    HeapData *shm_data; // Assume HeapData is defined elsewhere

    // Private constructor
    SharedMemoryHeap();

    // Delete copy constructor and assignment operator
    SharedMemoryHeap(const SharedMemoryHeap &) = delete;
    SharedMemoryHeap &operator=(const SharedMemoryHeap &) = delete;

public:
    static SharedMemoryHeap &getInstance()
    {
        static SharedMemoryHeap instance; // Guaranteed to be destroyed, instantiated on first use.
        return instance;
    }

    ~SharedMemoryHeap();
    void pushToSharedMemory(std::string taskID, int priority);
};

#endif // SHARED_MEMORY_HEAP_H

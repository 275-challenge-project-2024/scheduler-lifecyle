#include "task_heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

#define PATHNAME "/tmp" // Make sure this directory exists and is accessible
#define PROJ_ID 123     // An arbitrary number, used to generate a key

int main()
{
    key_t SHM_KEY;
    // Generate a unique key from a pathname and a project identifier
    SHM_KEY = ftok(PATHNAME, PROJ_ID);
    if (SHM_KEY == -1)
    {
        perror("ftok failed");
        exit(1);
    }
    std::cout << "HERE" << std::endl;
    int shm_id = shmget(SHM_KEY, sizeof(HeapData), IPC_CREAT | 0666);
    if (shm_id >= 0)
    {
        // If shared memory segment exists, delete it
        if (shmctl(shm_id, IPC_RMID, NULL) == -1)
        {
            perror("shmctl(IPC_RMID) failed");
            exit(1);
        }
    }

    shm_id = shmget(SHM_KEY, sizeof(HeapData), IPC_CREAT | 0666);
    if (shm_id < 0)
    {
        perror("shmget failed");
        exit(1);
    }

    void *shm_addr = shmat(shm_id, NULL, 0);
    if (shm_addr == (void *)-1)
    {
        perror("shmat failed");
        exit(1);
    }

    HeapData *heap = initialize_heap(shm_addr);

    heap_push(heap, "task_1", "2024-05-16T01:30:00Z", 5);
    heap_push(heap, "task_2", "2024-05-16T01:35:00Z", 3);
    heap_push(heap, "task_3", "2024-05-16T01:28:00Z", 10);

    remove_node_by_id(heap, "task_2");

    HeapElement maxItem = heap_pop(heap);
    printf("Popped: %s, Timestamp: %s, Current Priority: %d\n", maxItem.taskId, maxItem.timestamp, maxItem.priority);

    if (shmdt(shm_addr) < 0)
    {
        perror("shmdt failed");
        exit(1);
    }

    if (shmctl(shm_id, IPC_RMID, NULL) == -1)
    { // Clean up shared memory
        perror("shmctl(IPC_RMID) clean up failed");
        exit(1);
    }

    return 0;
}

// int main2()
// {
//     int shm_id = shmget(SHM_KEY, sizeof(HeapData), IPC_CREAT | 0666);
//     if (shm_id >= 0)
//     {

//         // If shared memory segment exists, delete it
//         if (shmctl(shm_id, IPC_RMID, NULL) == -1)
//         {
//             perror("shmctl(IPC_RMID) failed");
//             exit(1);
//         }
//     }

//     shm_id = shmget(SHM_KEY, sizeof(HeapData), IPC_CREAT | 0666);
//     if (shm_id < 0)
//     {
//         perror("shmget failed");
//         exit(1);
//     }

//     void *shm_addr = shmat(shm_id, NULL, 0);
//     if (shm_addr == (void *)-1)
//     {
//         perror("shmat failed");
//         exit(1);
//     }

//     HeapData *heap = initialize_heap(shm_addr);
//     // Push into heap (taskId, timestamp, curr_capacity, total_capacity)
//     heap_push(heap, "task_1", "2024-05-16T01:30:00Z", 5);
//     heap_push(heap, "task_2", "2024-05-16T01:35:00Z", 3);
//     heap_push(heap, "task_3", "2024-05-16T01:28:00Z", 10);

//     remove_node_by_id(heap, "task_2");

//     HeapElement maxItem = heap_pop(heap);
//     printf("Popped: %s, Timestamp: %s, Current Priority: %d\n", maxItem.taskId, maxItem.timestamp, maxItem.priority);

//     if (shmdt(shm_addr) < 0)
//     {
//         perror("shmdt failed");
//         exit(1);
//     }

//     return 0;
// }

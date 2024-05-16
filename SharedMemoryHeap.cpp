#include "SharedMemoryHeap.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "task_heap.h"

SharedMemoryHeap::SharedMemoryHeap()
{
    // Generate a unique key from a pathname and a project identifier
    const char *PATHNAME = "/tmp"; // Example pathname, you should use a valid path
    const int PROJ_ID = 'A';       // Example project identifier

    SHM_KEY = ftok(PATHNAME, PROJ_ID);
    std::cout << "SHM_KEY: " << SHM_KEY << std::endl;
    if (SHM_KEY == -1)
    {
        perror("ftok failed");
        exit(1);
    }

    shm_id = shmget(SHM_KEY, sizeof(HeapData), IPC_CREAT | 0666);
    if (shm_id == -1)
    {
        perror("shmget failed");
        exit(1);
    }

    shm_data = (HeapData *)shmat(shm_id, NULL, 0);
    if (shm_data == (HeapData *)-1)
    {
        perror("shmat failed");
        exit(1);
    }
}

SharedMemoryHeap::~SharedMemoryHeap()
{
    if (shmdt(shm_data) == -1)
    {
        perror("shmdt failed");
        exit(1);
    }
    if (shmctl(shm_id, IPC_RMID, NULL) == -1)
    {
        perror("shmctl(IPC_RMID) failed");
        exit(1);
    }
}

void SharedMemoryHeap::pushToSharedMemory(std::string taskID, int priority)
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    // Convert timestamp to string
    std::string timestamp_str = std::to_string(timestamp);

    // Assume heap_push is defined to take parameters for shm_data, taskID, timestamp, and priority
    heap_push(shm_data, taskID.c_str(), timestamp_str.c_str(), priority);
}

#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include "SharedMemoryHeap.h"

int main()
{
    SharedMemoryHeap &shmHeap = SharedMemoryHeap::getInstance();
    shmHeap.print();
}

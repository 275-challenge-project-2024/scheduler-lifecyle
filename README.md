
Start main

g++ -std=c++14 -o main main.cpp SharedMemoryHeap.cpp task_heap.cpp

./main



Run another process (that will read the shared memory)

g++ -std=c++14 -o program2 another.cpp SharedMemoryHeap.cpp task_heap.cpp
./program2
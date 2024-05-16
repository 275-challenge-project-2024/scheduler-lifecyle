#include "task_heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

// Initialize the heap structure
struct HeapData *initialize_heap(void *shm_addr)
{
    struct HeapData *heap = (struct HeapData *)shm_addr;
    heap->size = 0;
    return heap;
}

// Swap two heap elements
void swap(struct HeapElement *a, struct HeapElement *b)
{
    struct HeapElement temp = *a;
    *a = *b;
    *b = temp;
}

// Reorder the heap from the index upwards
void heapify_up(struct HeapData *heap, size_t index)
{
    while (index != 0 && heap->data[(index - 1) / 2].priority < heap->data[index].priority)
    {
        swap(&heap->data[index], &heap->data[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

// Reorder the heap from the index downwards
void heapify_down(struct HeapData *heap, size_t index)
{
    size_t largest = index;
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;

    if (left < heap->size && heap->data[left].priority > heap->data[largest].priority)
    {
        largest = left;
    }
    if (right < heap->size && heap->data[right].priority > heap->data[largest].priority)
    {
        largest = right;
    }
    if (largest != index)
    {
        swap(&heap->data[index], &heap->data[largest]);
        heapify_down(heap, largest);
    }
}

// Add an element to the heap
void heap_push(struct HeapData *heap, const char *taskId, const char *timestamp, int32_t priority)
{
    if (heap->size == MAX_TASKS)
    {
        fprintf(stderr, "Heap is full\n");
        return;
    }

    strncpy(heap->data[heap->size].taskId, taskId, sizeof(heap->data[heap->size].taskId) - 1);
    heap->data[heap->size].taskId[sizeof(heap->data[heap->size].taskId) - 1] = '\0'; // Ensure null termination

    strncpy(heap->data[heap->size].timestamp, timestamp, sizeof(heap->data[heap->size].timestamp) - 1);
    heap->data[heap->size].timestamp[sizeof(heap->data[heap->size].timestamp) - 1] = '\0'; // Ensure null termination

    heap->data[heap->size].priority = priority;

    heap->size++;
    heapify_up(heap, heap->size - 1);
}

// Remove the highest priority element from the heap
struct HeapElement heap_pop(struct HeapData *heap)
{
    if (heap->size == 0)
    {
        fprintf(stderr, "Heap is empty\n");
        return (struct HeapElement){}; // Default initialization
    }

    struct HeapElement root = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);

    return root;
}

// Find the index of an element by task ID
ssize_t find_index_by_id(struct HeapData *heap, const char *taskId)
{
    for (size_t i = 0; i < heap->size; ++i)
    {
        if (strncmp(heap->data[i].taskId, taskId, sizeof(heap->data[i].taskId)) == 0)
        {
            return i;
        }
    }
    return -1;
}

// Remove a specific element from the heap
bool remove_node_by_id(struct HeapData *heap, const char *taskId)
{
    ssize_t index = find_index_by_id(heap, taskId);
    if (index == -1)
    {
        fprintf(stderr, "Worker ID not found\n");
        return false;
    }

    heap->data[index] = heap->data[heap->size - 1];
    heap->size--;

    if (index < heap->size)
    {
        heapify_down(heap, index);
        heapify_up(heap, index);
    }

    return true;
}

#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdbool.h>

// Queue for BFS and Connected Components
typedef struct {
    int *data;
    int front, rear, size, capacity;
} Queue;

Queue* queue_create(int capacity);
void queue_destroy(Queue *q);
void queue_enqueue(Queue *q, int value);
int queue_dequeue(Queue *q);
bool queue_is_empty(Queue *q);

// Stack for DFS
typedef struct {
    int *data;
    int top, capacity;
} Stack;

Stack* stack_create(int capacity);
void stack_destroy(Stack *s);
void stack_push(Stack *s, int value);
int stack_pop(Stack *s);
bool stack_is_empty(Stack *s);

// Min-Heap for Dijkstra
typedef struct {
    int vertex;
    float dist;
} HeapNode;

typedef struct {
    HeapNode *data;
    int *pos; // Position of vertex v in data array (-1 if not in heap)
    int size, capacity;
} MinHeap;

MinHeap* minheap_create(int capacity);
void minheap_destroy(MinHeap *h);
void minheap_insert_or_update(MinHeap *h, int v, float dist);
HeapNode minheap_extract_min(MinHeap *h);
bool minheap_is_empty(MinHeap *h);

#endif

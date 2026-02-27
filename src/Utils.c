#include "Utils.h"
#include <string.h>

Queue* queue_create(int capacity) {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->capacity = capacity; q->front = 0; q->size = 0; q->rear = capacity - 1;
    q->data = (int*)malloc(capacity * sizeof(int));
    return q;
}
void queue_destroy(Queue *q) { free(q->data); free(q); }
bool queue_is_empty(Queue *q) { return (q->size == 0); }
void queue_enqueue(Queue *q, int value) {
    q->rear = (q->rear + 1) % q->capacity;
    q->data[q->rear] = value; q->size++;
}
int queue_dequeue(Queue *q) {
    int value = q->data[q->front];
    q->front = (q->front + 1) % q->capacity; q->size--;
    return value;
}

Stack* stack_create(int capacity) {
    Stack *s = (Stack*)malloc(sizeof(Stack));
    s->capacity = capacity; s->top = -1;
    s->data = (int*)malloc(capacity * sizeof(int));
    return s;
}
void stack_destroy(Stack *s) { free(s->data); free(s); }
bool stack_is_empty(Stack *s) { return (s->top == -1); }
void stack_push(Stack *s, int value) { s->data[++s->top] = value; }
int stack_pop(Stack *s) { return s->data[s->top--]; }

MinHeap* minheap_create(int capacity) {
    MinHeap *h = (MinHeap*)malloc(sizeof(MinHeap));
    h->capacity = capacity; h->size = 0;
    h->data = (HeapNode*)malloc(capacity * sizeof(HeapNode));
    h->pos = (int*)malloc(capacity * sizeof(int));
    for (int i = 0; i < capacity; i++) h->pos[i] = -1;
    return h;
}
void minheap_destroy(MinHeap *h) { free(h->data); free(h->pos); free(h); }
bool minheap_is_empty(MinHeap *h) { return (h->size == 0); }

void swap_heap_node(HeapNode *a, HeapNode *b) { HeapNode t = *a; *a = *b; *b = t; }

void minheapify(MinHeap *h, int idx) {
    int smallest = idx, left = 2 * idx + 1, right = 2 * idx + 2;
    if (left < h->size && h->data[left].dist < h->data[smallest].dist) smallest = left;
    if (right < h->size && h->data[right].dist < h->data[smallest].dist) smallest = right;
    if (smallest != idx) {
        h->pos[h->data[smallest].vertex] = idx;
        h->pos[h->data[idx].vertex] = smallest;
        swap_heap_node(&h->data[smallest], &h->data[idx]);
        minheapify(h, smallest);
    }
}

void minheap_insert_or_update(MinHeap *h, int v, float dist) {
    if (h->pos[v] != -1) {
        int i = h->pos[v];
        h->data[i].dist = dist;
        while (i && h->data[i].dist < h->data[(i - 1) / 2].dist) {
            h->pos[h->data[i].vertex] = (i - 1) / 2;
            h->pos[h->data[(i - 1) / 2].vertex] = i;
            swap_heap_node(&h->data[i], &h->data[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    } else {
        int i = h->size++;
        h->data[i].vertex = v; h->data[i].dist = dist; h->pos[v] = i;
        while (i && h->data[i].dist < h->data[(i - 1) / 2].dist) {
            h->pos[h->data[i].vertex] = (i - 1) / 2;
            h->pos[h->data[(i - 1) / 2].vertex] = i;
            swap_heap_node(&h->data[i], &h->data[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }
}

HeapNode minheap_extract_min(MinHeap *h) {
    HeapNode root = h->data[0];
    HeapNode lastNode = h->data[h->size - 1];
    h->data[0] = lastNode;
    h->pos[root.vertex] = -1;
    h->pos[lastNode.vertex] = 0;
    h->size--;
    minheapify(h, 0);
    return root;
}

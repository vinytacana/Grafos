#ifndef fila_bfs
#define fila_bfs
#include <stdio.h>

struct queue
{
    int tam;
    float *dados;
    int first;
    int last;
    int nItens;
};
typedef queue queue;

queue *createQueue(int c);
bool isFull(queue *f);
bool isEmpty(queue *f);
void enqueue(queue *f, int v);
int dequeue(queue *f);
void showQueue(queue *f);

#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Queue.h"

queue *createQueue(int c)
{
    queue *f = (queue *)malloc(sizeof(queue));
    f->tam = c;
    f->dados = (float *)malloc(f->tam * sizeof(float));
    f->first = 0;
    f->last = -1;
    f->nItens = 0;
    return f;
}
bool isFull(queue *f)
{
    return (f->nItens == f->tam);
}
bool isEmpty(queue *f)
{
    return (f->nItens == 0);
}
void enqueue(queue *f, int v)
{
    if (f->last == f->tam - 1)
        f->last = -1;
    f->last++;
    f->dados[f->last] = v;
    f->nItens++;
}
int dequeue(queue *f)
{
    int temp = f->dados[f->first++];
    if (f->first == f->tam)
        f->first = 0;
    f->nItens--;
    return temp;
}
void showQueue(queue *f)
{
    int cont, i;
    for (cont = 0, i = f->first; cont < f->nItens; cont++)
    {
        printf("\n%.2f\t", f->dados[i++]);
        if (i == f->tam)
            i = 0;
    }
    printf("\n\n");
}

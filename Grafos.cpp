#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Grafos.h"

No *newNo(int w)
{
    No *no = (No *)malloc(sizeof(No));
    if (no == NULL)
        return NULL;
    no->w = w;
    no->prox = NULL;
    return no;
}
graph *newGraph(int V)
{
    graph *grafo = (graph *)malloc(sizeof(graph));
    if (grafo)
    {
        grafo->V = V;
        grafo->U = 0;
        grafo->adj = (No **)malloc(V * sizeof(No *));
        for (int i = 0; i < V; i++)
        {
            grafo->adj[i] = NULL;
        }
    }
    return grafo;
}

void addAresta(graph *grafo, int v, int w)
{
    No *no = newNo(w);
    no->prox = grafo->adj[v];
    grafo->adj[v] = no;

    no = newNo(v);
    no->prox = grafo->adj[w];
    grafo->adj[w] = no;
    grafo->U++;
}
void exibeGraph(graph *grafo)
{
    for (int v = 0; v < grafo->V; v++)
    {
        No *p = grafo->adj[v];
        printf("\n Vertice %d: \n", v);
        while (p)
        {
            printf("\n->%d", p->w);
            p = p->prox;
        }
        printf("\n");
    }
}
void freeGraph(graph *grafo)
{
    for (int v = 0; v < grafo->V; v++)
    {
        No *p = grafo->adj[v];
        while (p)
        {
            No *aux = p;
            p = p->prox;
            free(aux);
        }
    }
    free(grafo->adj);
    free(grafo);
}
graph *initGraphArq(const char *cam)
{
    FILE *arq = fopen(cam, "r");
    if (arq == NULL)
        return NULL;
    int V;
    fscanf(arq, "%d", &V);
    graph *g = newGraph(V);
    int v, u;
    while (fscanf(arq, "%d %d", &v, &u) != EOF)
    {
        addAresta(g, v - 1, u - 1);
    }
    fclose(arq);
    return g;
}
int numArestasL(graph *grafo)
{
    int arestas = 0;
    for (int i = 0; i < grafo->V; i++)
    {
        No *aux = grafo->adj[i];
        while (aux)
        {
            arestas++;
            aux = aux->prox;
        }
    }
    return arestas / 2;
}
int numVertL(graph *grafo)
{
    return grafo->V;
}
int grauVertL(graph *grafo, int *graus)
{
    for (int i = 0; i < grafo->V; i++)
    {
        graus[i] = 0;
        No *aux = grafo->adj[i];
        while (aux)
        {
            graus[i]++;
            aux = aux->prox;
        }
    }
}
int grauMinL(graph *grafo)
{
    int graus[grafo->V];
    grauVertL(grafo, graus);
    int min = graus[0];
    for (int i = 1; i < grafo->V; i++)
    {
        if (graus[i] < min)
        {
            min = graus[i];
        }
    }
    return min;
}

int grauMaxL(graph *grafo){
    int graus[grafo->V];
    grauVertL(grafo, graus);
    int max=graus[0];
    for(int i=0;i<grafo->V;i++){
        if(graus[i]>max){
          max=graus[i];  
        }
        
    }
    return max;

}

int grauMedioL(graph *grafo){
    int graus[grafo->V];
    grauVertL(grafo, graus);
    int soma=0;
    for(int i=0;i<grafo->V;i++){
        soma=soma+graus[i];
    }
    return soma/grafo->V;
}
void outputFile(graph* grafo, const char *namefile){
    FILE *arq= fopen(namefile, "w");
    if(arq==NULL)
        perror("Erro ao abrir arquivo de saida");
    fprintf(arq,"\nO Grafo escolhido contem: \n");
    fprintf(arq,"\nNumero de vertices: %d\n",numVertL(grafo));
    fprintf(arq,"\nNumero de arestas: %d\n", numArestasL(grafo));
    fprintf(arq,"\nGrau minimo do grafo: %d\n", grauMinL(grafo));
    fprintf(arq,"\nGrau maximo de um grafo: %d\n", grauMaxL(grafo));
    fprintf(arq,"\nGrau Medio do grafo: %d\n", grauMedioL(grafo));

    fclose(arq);

}

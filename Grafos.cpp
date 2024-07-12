#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Grafos.h"
#include "Queue.h"

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
        grafo->visitado = (int *)malloc(V * sizeof(int));
        for (int i = 0; i < V; i++)
        {
            grafo->adj[i] = NULL;
            grafo->visitado[i] = 0;
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
    free(grafo->visitado);
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
    printf("\n%d", g->visitado[0]);
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

int grauMaxL(graph *grafo)
{
    int graus[grafo->V];
    grauVertL(grafo, graus);
    int max = graus[0];
    for (int i = 0; i < grafo->V; i++)
    {
        if (graus[i] > max)
        {
            max = graus[i];
        }
    }
    return max;
}

int grauMedioL(graph *grafo)
{
    int graus[grafo->V];
    grauVertL(grafo, graus);
    int soma = 0;
    for (int i = 0; i < grafo->V; i++)
    {
        soma = soma + graus[i];
    }
    return soma / grafo->V;
}
void outputFile(graph *grafo, const char *namefile)
{
    FILE *arq = fopen(namefile, "w");
    if (arq == NULL)
        perror("Erro ao abrir arquivo de saida");
    fprintf(arq, "\nO Grafo escolhido contem: \n");
    fprintf(arq, "\nNumero de vertices: %d\n", numVertL(grafo));
    fprintf(arq, "\nNumero de arestas: %d\n", numArestasL(grafo));
    fprintf(arq, "\nGrau minimo do grafo: %d\n", grauMinL(grafo));
    fprintf(arq, "\nGrau maximo de um grafo: %d\n", grauMaxL(grafo));
    fprintf(arq, "\nGrau Medio do grafo: %d\n", grauMedioL(grafo));

    fclose(arq);
}

void dfs(graph *grafo, int v, Info_Vertice vert[])
{
    printf("\nInicializou o dfs1");
    grafo->visitado[v] = 1;
    int w;
    printf("Vertex: %d", v);

    No *aux = grafo->adj[v];
    while (aux != NULL)
    {
        printf("\nO grafo nao é nulo, entrou no while do dfs");
        w = aux->w;
        if (w < 0 || w >= grafo->V)
        {
            printf("Erro");
            exit(EXIT_FAILURE);
        }
        if (!grafo->visitado[w])
        {
            printf("\nEntrou no if do dfs, !grafo->visitado[w]");
            vert[w].pai = v;
            vert[w].profundidade = vert[v].profundidade + 1;
            dfs(grafo, w, vert);
        }
        aux = aux->prox;
    }
    printf("\nSaiu do dfs");
} /*
 void DFS(graph *G, int v){
     struct No* adjs= G->adj[v];

     printf("\n%d", G->visitado[0]);
     struct No* aux= adjs;

     printf("\n%d", v);
     G->visitado[v]=1;
     while(aux!=NULL){
         int cv= aux->w;
         if(G->visitado[cv]==0){
             G->visitado[cv].pai = v;
             G->visitado[cv].profundidade= G->visitado[v].profundidade+1;
             DFS(G, cv);

         }
         aux= aux->prox;
     }

 }*/
int writeGeneTree(const char *namefile, int v, graph *G)
{
    v--;
    Info_Vertice *vert = (Info_Vertice *)malloc(sizeof(Info_Vertice) * G->V - 1);
    int w;
    if (vert == NULL)
        exit(EXIT_FAILURE);
    for (w = 0; w < G->V; w++)
    {
        printf("test2");
        G->visitado[w] = 0;
        vert[w].profundidade = 0;
        vert[w].pai = -1;
    }

    dfs(G, v, vert);
    // DFS(G, v);
    FILE *arq = fopen(namefile, "w");
    if (!arq)
    {
        free(vert);
        return 0;
        perror("Erro ao abrir o arquivo de saida");
    }
    for (int w = 0; w < G->V; w++)
    {
        if (G->visitado[w])
        {
            fprintf(arq, "Vertice=%u\tPai=%d\tProfundidade=%d\n", w + 1, vert[w].pai + 1, vert[w].profundidade);
        }
    }
    fclose(arq);
    free(vert);
    return 1;
}
void bfs(graph *grafo, int v, Info_Vertice vert[])
{
    queue *queue = createQueue(grafo->V);
    grafo->visitado[v] = 1;
    vert[v].profundidade = 0;
    enqueue(queue, v);
    while (!isEmpty(queue))
    {
        int u = dequeue(queue);
        No *aux = grafo->adj[u];
        while (aux != NULL)
        {
            int w = aux->w;
            if (!grafo->visitado[w])
            {
                grafo->visitado[w] = 1;
                vert[w].pai = u;
                vert[w].profundidade = vert[u].profundidade + 1;
                enqueue(queue, w);
            }
            aux = aux->prox;
        }
    }
    free(queue->dados);
    free(queue);
}
int writeGeneTreeBFS(const char *namefile, int v, graph *G)
{
    v--;
    Info_Vertice *vert = (Info_Vertice *)malloc(sizeof(Info_Vertice) * G->V);
    int w;
    if (vert == NULL)
        exit(EXIT_FAILURE);
    for (w = 0; w < G->V; w++)
    {
        G->visitado[w] = 0;
        vert[w].profundidade = 0;
        vert[w].pai = -1;
    }
    vert[v].profundidade = 0;
    bfs(G, v, vert);
    FILE *arq = fopen(namefile, "w");
    if (!arq)
    {
        free(vert);
        perror("Erro ao abrir o arquivo de saida");
        return 0;
    }
    for (int w = 0; w < G->V; w++)
    {
        if (G->visitado[w])
        {
            fprintf(arq, "Vertice=%u\tPai=%d\tProfundidade=%d\n", w + 1, vert[w].pai + 1, vert[w].profundidade);
        }
    }
    fclose(arq);
    free(vert);
    return 1;
}

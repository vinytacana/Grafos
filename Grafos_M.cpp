#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Grafos_M.h"
#include "Queue.h"

static int **InitMatrix(int m, int n, int firstval){
    int **M= (int **)malloc(m *sizeof(int *));
    if(M==NULL)
        exit(EXIT_FAILURE);
    for(int i=0; i<m;i++){
        M[i]= (int *)malloc(n*sizeof(int ));
        if(M[i]==NULL){

        }
    }
}
static void FreeM_M(int **M, int m){
    for(int i=0;i<m; i++){
        free(M[i]);
    }
    free(M);
}
graph_M *crateGraph_M(int v){
    graph_M *G = ( graph_M *)malloc(sizeof(graph_M));
    if(G==NULL)
        exit(EXIT_FAILURE);
    G->V= v;
    G->E=0;
    G->adj= InitMatrix(v, v, 0);
    return G;
}
void freeGraph_M(graph_M *G){
    FreeM_M(G->adj, G->V);
    free(G);
}
int insertAresta_M(graph_M *G, int u, int v){
    if(u<0||v<0||u>=G->V||v>=G->V)
        return 0;
    if(G->adj[u][v]==0){
        G->adj[u][v]=1;
        G->adj[v][u]=1;
        G->E++;
        return 1;
    }
    return 0;
}
int removeAresta_M(graph_M *G, int u, int v){
    if(u<0||v<0||u>=G->V||v>=G->V)
        return 0;
    if(G->adj[u][v]==1){
        G->adj[u][v]=0;
        G->adj[v][u]=0;
        G->E--;
        return 1;
    }
    return 0;
}

graph_M *initGraphArq_M(char *caminho){
    FILE *arq= fopen(caminho, "r");
    if(arq==NULL)
        return NULL;
    int V;
    int u, v;
    if(!fscanf(arq, "%d", &V)){
        fclose(arq);
        return NULL;
    }
    graph_M *G = crateGraph_M(V);
    if(G==NULL){
        fclose(arq);
        return NULL;
    }
    int u, v;
    while (fscanf(arq, "%d %d", &u, &v)!=EOF)
    {
        insertAresta_M(G, u, v);
    }
    fclose(arq);
    return G;
    
}

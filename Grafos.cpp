#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "Grafos.h"

No* newNo(int w){
    No* no= (No*)malloc(sizeof(No));
    if(no==NULL)
        return NULL;    
    no->w= w;
    no->prox= NULL;
    return no;
}
graph* newGraph(int V){
    graph* grafo= (graph*)malloc(sizeof(graph));
    if(grafo){
        grafo->V= V;
        grafo->U= 0;
        grafo->adj= (No**)malloc(V*sizeof(No*));
        for(int i=0;i<V;i++){
            grafo->adj[i]=NULL;
        }

    }
    return grafo;

}

void addAresta(graph* grafo, int v, int w){
    No* no= newNo(w);
    no->prox= grafo->adj[v];
    grafo->adj[v]=no;

    no= newNo(v);
    no->prox= grafo->adj[w];
    grafo->adj[w]=no;
    grafo->U++;
}
void exibeGraph(graph* grafo){
    for(int v=0; v<grafo->V;v++){
        No* p= grafo->adj[v];
        printf("\n Vertice %d: \n", v);
        while(p){
            printf("\n->%d", p->w);
            p=p->prox;
        }
        printf("\n");
    }
}
void freeGraph(graph* grafo){
    for(int v=0; v<grafo->V;v++){
        No*p= grafo->adj[v];
        while (p)
        {
        No* aux= p;
        p=p->prox;
        free(aux);            
        }
        
    }
    free(grafo->adj);
    free(grafo);
}
graph* initGraphArq(const char *cam){
    FILE *arq= fopen(cam, "r");
    if(arq==NULL)
        return NULL;
    int V;
    fscanf(arq, "%d", &V);
    graph* g= newGraph(V);
    int v, u;
    while(fscanf(arq, "%d %d", &v, &u)!=EOF){
        addAresta(g, v-1, u-1);
    }
    fclose(arq);
    return g;
}


//struct grafo{
  //  Vertice **vertices;
   // Aresta **arestas;
//}

/*
struct graph{
    int V;
    int A;
    int **adj;
    
};
typedef struct graph *Graph;
// REPRESENTAÇÃO POR MATRIZ DE ADJACÊNCIAS: A função constroiGrafo() constrói um grafo com vértices 0 1 .. V-1 e nenhum arco. 

Graph constroiGrafo(int V){
    Graph G = malloc(sizeof *G );
    G->V= V;
    G->A= 0;
    G->adj= 

}*/

/* 
struct node{
    
};
struct arc 
{
    int adj;
};
struct grafos
{
    struct node nodes[maxnodes];
  
};
struct grafos g; 

 */







#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Grafos_M.h"
#include "Queue.h"

static int **InitMatrix(int m, int n, int firstval){
    int **M= (int **)malloc(m *sizeof(int *));
    if(M==NULL)
        exit(EXIT_FAILURE);
    int i, j;
    for( i=0; i<m;i++){
        M[i]= (int *)malloc(n*sizeof(int ));
        if(M[i]==NULL){
            if(i!=0)
                for( j=0;j<i;j++){
                    free(M[j]);
            free(M);
            exit(EXIT_FAILURE);
            }
            for(int j=0;j<n;j++){
                M[i][j]=firstval;
            }
        }
    }
    return M;
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
    while (fscanf(arq, "%d %d", &u, &v)!=EOF)
    {
        insertAresta_M(G, u, v);
    }
    fclose(arq);
    return G;
    
}
void showGraph_M(graph_M *G){
    printf("\n---Matriz de adjacencia---\n");
    for(int i=0;i<G->V;i++){
        for(int j=0;j<G->V;j++){
            printf("%d", G->adj[i][j]);
        }
        printf("\n");
    }
}
void grauVert_M(graph_M *G, int *graus){
    for( int i=0;i<G->V;){
        graus[i]=0;
        for(int j=0;j<G->V;j++){
            if(G->adj[i][j]==1){
                graus[i]++;
            }
        }
    }
}
int numVert_M(graph_M *G){
    return G->V;
}
int grauMin_M(graph_M *G){
    int graus[G->V];
    grauVert_M(G, graus);
    int min= graus[0];
    for(int i=0;i<G->V;i++){
        if(graus[i]==min){
            min= graus[i];
        }
    }
    return min;
}
int grauMax_M(graph_M *G){
    int graus[G->V];
    grauVert_M(G, graus);
    int max=graus[0];
    for(int i=0;i<G->V;i++){
        if(graus[i]<max){
            max= graus[i];
        }
    }
    return max;
}
int grauMedio_M(graph_M *G){
    int graus[G->V];
    grauVert_M(G, graus);
    int soma=0;
    for(int i=0;i<G->V;i++){
        soma+=graus[i];
    }
    return soma/G->V;
}
void outputFile_M(graph_M *G, const char *namefile){
    printf("\nteste entrou na funcao");
    FILE *arq= fopen(namefile, "w");
    if(arq==NULL)
        perror("Erro ao abrir arquivo de saida.. ");
    printf("\ntestee");
    fprintf(arq, "\nO grafo escolhido contem: \n");
    fprintf(arq, "\nNumero de vertices: %d\n", numVert_M(G));
    fprintf(arq,"\nNumero de arestas: %d\n", G->E);
    fprintf(arq, "\nGrau minimo do grafo: %d\n", grauMin_M(G));
    fprintf(arq, "\nGrau maximo de um grafo: %d\n", grauMax_M(G));
    fprintf(arq, "\nGrau medio de um grafo: %d\n",grauMedio_M(G));
    fclose(arq);
}

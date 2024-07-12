#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Grafos.h"
#include "Grafos.cpp"
#include "Queue.h"
#include "Queue.cpp"

int main() {
 
    graph *G = initGraphArq("./output/grafo.txt");
    
    if(G != NULL){
       /* exibeGraph(G);
        numArestasL(G);
        numVertL(G);
        grauMinL(G);
        grauMaxL(G);
        grauMedioL(G);
       outputFile(G,"test.txt");*/
      writeGeneTree("dfs_saida.txt", 1, G);
      writeGeneTreeBFS("bfs_saida.txt", 1, G);
    }else
         printf("ERRO!\n");
}
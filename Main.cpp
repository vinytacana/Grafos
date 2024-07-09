#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Grafos.h"
#include "Grafos.cpp"

int main() {
 
    graph *G = initGraphArq("./output/grafo.txt");
    
    if(G != NULL){
        exibeGraph(G);
        numArestasL(G);
        numVertL(G);
        grauMinL(G);
        grauMaxL(G);
        grauMedioL(G);
       outputFile(G,"test.txt");
    }else
         printf("ERRO!\n");
}
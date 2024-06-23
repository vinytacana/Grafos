#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Grafos.h"
#include "Grafos.cpp"

int main() {
 
    graph *G = initGraphArq("./output/grafo.txt");
    
    if(G != NULL)
        exibeGraph(G);
    else printf("ERRO!\n");
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Grafos.h"
#include "Grafos.cpp"
#include "Queue.h"
#include "Queue.cpp"

int main()
{

  graph *G = initGraphArq("./output/grafo.txt");

  if (G != NULL)
  {
    /* exibeGraph(G);
     numArestasL(G);
     numVertL(G);
     grauMinL(G);
     grauMaxL(G);
     grauMedioL(G);
    outputFile(G,"test.txt");*/

    int diam = diameter(G);
    printf("\nDiametro do Grafo: %d", diam);
    // writeGeneTree("dfs_saida.txt", 1, G);
    // writeGeneTreeBFS("bfs_saida.txt", 1, G);
    freeGraph(G);
  }
  else
    printf("ERRO!\n");
  return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Grafos.h"
#include "Grafos.cpp"
#include "Queue.h"
#include "Queue.cpp"
#include "Grafos_M.h"
#include "Grafos_M.cpp"

int main()
{

  graph_M *G = initGraphArq_M("./output/grafo.txt");

  if (G != NULL)
  {
     /*exibeGraph(G);
     numArestasL(G);
     numVertL(G);
     grauMinL(G);
     grauMaxL(G);
     grauMedioL(G);
    outputFile(G,"test.txt");*/

    //int diam = Diameter(G);
    //printf("\nDiametro do Grafo: %d", diam);


   // writeGeneTree("dfs_saida.txt", 1, G);
    //writeGeneTreeBFS("bfs_saida.txt", 1, G);
    freeGraph_M(G);
  }
  else
    printf("ERRO!\n");
  return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Grafos.h"
#include "Grafos.cpp"
#include "Queue.h"
#include "Queue.cpp"
#include "Grafos_M.h"
#include "Grafos_M.cpp"
#include<time.h>
//usado para teste...

int main()
{
 
  //graph_M *G = initGraphArq_M("./output/grafo.txt");
  graph *GrafoL = initGraphArq("./grafo.txt");

  if (GrafoL != NULL)
  {
   // int totalExecut=100;
    //double total_t=0.0;
    //double media_t= TimeBfs(GrafoL, 0, totalExecut);
    //printf("\nTempo medio d0 Bfs: %f segundos", media_t);
    //showGraph_M(G);
    //outputFile_M(G, "saidaM.txt");
    //writeGeneTree_M("dfs_saida.txt", 1, G);
    //writeGeneTreeBfs_M("Bfs_saida.txt",1, G );
    //int dim =Diameter_M(G);
    //printf("\n Diametro do grafo: %d\n", dim);
    exibeGraph(GrafoL); 
     //numArestasL(G);
     //numVertL(G);
     //grauMinL(G);
     //grauMaxL(G);
     //grauMedioL(G);
    //outputFile(GrafoL,"saidaList.txt");*/
      //UGraphConnect(GrafoL, 10, 20);
    //int diam = Diameter(G);
    //printf("\nDiametro do Grafo: %d", diam);


   // writeGeneTree("dfs_saida.txt", 1, G);
    //writeGeneTreeBFS("bfs_saida.txt", 1, G);
 //freeGraph_M(G);
 freeGraph(GrafoL);
  }
  else
    printf("ERRO!\n");
  return 0;
}
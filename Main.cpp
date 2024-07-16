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

int main()
{
 
  //graph_M *G = initGraphArq_M("./output/grafo.txt");
 graph *GrafoL = initGraphArq("./output/grafo.txt");
 Info_Vertice *vert= (Info_Vertice *)malloc(sizeof(Info_Vertice)*GrafoL->V);

  if (GrafoL != NULL)
  {
    int totalExecut=100;
    double total_t=0.0;
    vert[GrafoL->V];
    for(int i=0;i<totalExecut;i++){
      memset(GrafoL->visitado, 0, sizeof(int)* GrafoL->visitado);
      total_t+=bfs(GrafoL, i, vert);
    }
    double media_t= total_t/totalExecut;
    printf("\nTempo medio de execucao bfs: %f segundos", media_t);
    //showGraph_M(G);
    //outputFile_M(G, "saidaM.txt");
    //writeGeneTree_M("dfs_saida.txt", 1, G);
    //writeGeneTreeBfs_M("Bfs_saida.txt",1, G );
    //int dim =Diameter_M(G);
    //printf("\n Diametro do grafo: %d\n", dim);
     /*exibeGraph(G); 
     numArestasL(G);
     numVertL(G);
     grauMinL(G);
     grauMaxL(G);
     grauMedioL(G);
    outputFile(GrafoL,"saidaList.txt");*/

    //int diam = Diameter(G);
    //printf("\nDiametro do Grafo: %d", diam);


   // writeGeneTree("dfs_saida.txt", 1, G);
    //writeGeneTreeBFS("bfs_saida.txt", 1, G);
 //freeGraph_M(G);
  }
  else
    printf("ERRO!\n");
  return 0;
}
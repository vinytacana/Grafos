#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
#include <time.h>
#include <float.h>

int main() 
{
	clock_t start, end;
    GrafoMatrizAdj *grafoMatriz = NULL;
    GrafoListaAdj *grafoLista = NULL;

    int op, vOrigem = -1,vDestino = -1;
    int vInicial;
    
    do {
        printf("\n-----------MENU------------\n\n");
        printf("1 - LER GRAFO - matriz de adj\n");
        printf("2 - MOSTRAR GRAFO - matriz de adj\n");
        printf("3 - BFS - BUSCA LARGURA - matriz de adj\n");
        printf("4 - DFS - BUSCA PROFUNDIDADE - matriz de adj\n");
        printf("5 - VER INFORMACOES DO GRAFO - matriz de adj\n");
        printf("6 - distancia entre o vertice X para o Y - matriz de adj\n");
        printf("7 - LER GRAFO - lista de adj\n");
        printf("8 - MOSTRAR GRAFO - lista de adj\n");
        printf("9- BFS - BUSCA LARGURA - lista de adj\n");
        printf("10 - DFS - BUSCA PROFUNDIDADE - lista de adj\n");
        printf("11 - VER INFORMACOES DO GRAFO - lista de adj\n");
        printf("12 - distancia entre o vertice X para o Y - lista de adj\n");
        printf("0 - SAIR\n");
        printf("Digite sua opcao: \n");
        scanf("%d", &op);

        switch (op) {
            case 1:
                lerArquivoDeGrafoMatriz(&grafoMatriz);
                break;
            case 2:
                if (grafoMatriz != NULL) {
                    mostrarGrafoMatriz(grafoMatriz);
                } else {
                    printf("Grafo nao foi carregado.\n");
                }
                break;
            case 3:
                if (grafoMatriz != NULL) {
                    printf("Digite o vertice inicial para BFS: ");
                    scanf("%d", &vInicial);
                    
                    start = clock();
                    
                    bfsMatriz(grafoMatriz, vInicial);
                    
                    end = clock();
                    
                    double tempo_gasto = (double)(end - start) / CLOCKS_PER_SEC;
                    printf("\nTEMPO GASTO: %.6f ", tempo_gasto);
                } else {
                    printf("Grafo nao foi carregado.\n");
                }
                break;
            case 4:
                if (grafoMatriz != NULL) {
                    printf("Digite o vertice inicial para DFS: ");
                    scanf("%d", &vInicial);
                    
                    start = clock();
                    
                    dfsMatriz(grafoMatriz,vInicial);
                    
                    end = clock();
                    
                    double tempo_gasto = (double)(end - start) / CLOCKS_PER_SEC;
                    printf("\nTEMPO GASTO: %.6f ", tempo_gasto);
                } else {
                    printf("Grafo nao foi carregado.\n");
                }
                break;
            case 5:
            	informacoesGrafoMatrizToArq(grafoMatriz);
            	listarComponentesConexas(grafoMatriz);
            	break;
            case 6:
            {	        	
	        	do{
	        		printf("\nQual o vertice de inicio? OBS: de 0 a %d\n ", grafoMatriz->qtdVertices);
	        		scanf("%d",&vOrigem);
				}while(vOrigem < 0 || vOrigem >= grafoMatriz->qtdVertices);
				
	        	do{
	        		printf("\nQual o vertice de destino? OBS: de 0 a %d\n", grafoMatriz->qtdVertices);
	        		scanf("%d",&vDestino);
				}while(vDestino < 0 || vDestino >= grafoMatriz->qtdVertices);
			
				float distancia = distanciaEntreVertices(grafoMatriz, vOrigem, vDestino);
				
				if(distancia >= 0 && distancia != FLT_MAX){
					printf("\nA distancia entre o vertice %d e o vertice %d eh: %f", vOrigem, vDestino, distancia);
				}else{
					printf("\nNao ha caminho algum entre esses vertices .");
				}
	        	break;	
			}
            case 7:
                lerArquivoDeGrafoLista(&grafoLista);
                break;
            case 8:
                if (grafoLista != NULL) {
                    mostrarGrafoLista(&grafoLista);
                } else {
                    printf("Grafo nao foi carregado.\n");
                }
                break;
            case 9:
                if (grafoLista != NULL) {
                    printf("Digite o vertice inicial para BFS: ");
                    scanf("%d", &vInicial);
                    start = clock();
                    
                    bfsLista(*grafoLista, vInicial);
                    
                    end = clock();
                    
                    double tempo_gasto = (double)(end - start) / CLOCKS_PER_SEC;
                    printf("\nTEMPO GASTO: %.6f ", tempo_gasto);
                } else {
                    printf("Grafo n�o foi carregado.\n");
                }
                break;
            case 10:
                if (grafoLista != NULL) {
                    printf("Digite o vertice inicial para DFS: ");
                    scanf("%d", &vInicial);
                    start = clock();
                    
                    dfsLista(grafoLista, vInicial);
                    
                    end = clock();
                    
                    double tempo_gasto = (double)(end - start) / CLOCKS_PER_SEC;
                    printf("\nTEMPO GASTO: %.6f ", tempo_gasto);
                } else {
                    printf("Grafo n�o foi carregado.\n");
                }
                break;
            case 11:
            	informacoesGrafoListaToArq(grafoLista);
            	listarComponentesConexasLista(grafoLista);
            	break;
            case 12:
            {
	        	do{
	        		printf("\nQual o vertice de inicio? OBS: de 0 a %d\n ", grafoLista->qtdVertices);
	        		scanf("%d",&vOrigem);
				}while( vOrigem < 0 || vOrigem >= grafoLista->qtdVertices);
				
	        	do{
	        		printf("\nQual o vertice de destino? OBS: de 0 a %d\n", grafoLista->qtdVertices);
	        		scanf("%d",&vDestino);
				}while( vDestino < 0 || vDestino >= grafoLista->qtdVertices);
				
				float distancia = distanciaEntreVerticesLista(grafoLista, vOrigem, vDestino);
			
				if(distancia >= 0 && distancia != FLT_MAX){
					printf("\nA distancia entre o vertice %d e o vertice %d eh: %f", vOrigem, vDestino, distancia);
				}else{
					printf("\nNao ha caminho algum entre esses vertices .");
				}
	        	break;	
			}
            case 0:
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    } while (op != 0);

    if (grafoMatriz != NULL) {
        freeGrafo(&grafoMatriz);
    }
    if (grafoLista != NULL) {
        freeGrafoLista(&grafoLista);
    }

    return 0;
}

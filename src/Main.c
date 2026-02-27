#include <stdio.h>
#include <time.h>
#include "Graph.h"
#include "Algorithms.h"
#include "Utils.h"
#include <string.h>

int get_vertex_from_user(const Graph *g) {
    char input[256];
    scanf(" %[^\n]", input);
    int id = atoi(input);
    if (id > 0 && id <= g->num_vertices) return id - 1;
    
    int found_id = graph_get_vertex_id_by_name(g, input);
    if (found_id == -1) printf("Vértice '%s' não encontrado.\n", input);
    return found_id;
}

int main() {
    Graph *g = NULL;
    int op;
    char filename[256];
    
    do {
        printf("\n----------- Menu de Grafos ------------\n");
        printf("1  - Carregar grafo (Matriz de Adjacência)\n");
        printf("2  - Carregar grafo (Lista de Adjacência)\n");
        printf("10 - Carregar nomes dos vértices (ID nome)\n");
        printf("3  - Busca em Largura (BFS)\n");
        printf("4  - Busca em Profundidade (DFS)\n");
        printf("5  - Calcular distância entre dois vértices\n");
        printf("6  - Informações gerais (Graus, Diâmetro, Componentes)\n");
        printf("7  - Caminho mínimo (Dijkstra com Heap)\n");
        printf("8  - Caminho mínimo (Dijkstra sem Heap)\n");
        printf("9  - Benchmark (Média de 100 buscas: BFS/DFS/Dijkstra)\n");
        printf("0  - Sair\n");
        printf("Digite sua opção: ");
        if (scanf("%d", &op) != 1) break;

        switch (op) {
            case 1:
            case 2:
                printf("Digite o nome do arquivo do grafo: ");
                scanf("%s", filename);
                if (g) graph_destroy(g);
                g = graph_load_from_file(filename, op == 1 ? GRAPH_MATRIX : GRAPH_LIST);
                if (g) {
                    printf("Grafo carregado com sucesso.\n");
                    printf("Memória utilizada: %.2f MB\n", graph_get_memory_usage(g));
                }
                else printf("Erro ao carregar o arquivo.\n");
                break;
            case 10:
                if (g) {
                    printf("Digite o nome do arquivo de nomes: ");
                    scanf("%s", filename);
                    graph_load_names(g, filename);
                    printf("Nomes carregados. Memória atual: %.2f MB\n", graph_get_memory_usage(g));
                } else printf("Carregue o grafo primeiro.\n");
                break;
            case 3:
                if (g) {
                    printf("Digite o vértice inicial (ID ou Nome): ");
                    int start = get_vertex_from_user(g);
                    if (start == -1) break;
                    clock_t begin = clock();
                    algorithms_bfs(g, start, "output/saida_bfs.txt");
                    clock_t end = clock();
                    printf("BFS concluído em %.4fs. Resultado salvo em output/saida_bfs.txt\n", (double)(end - begin) / CLOCKS_PER_SEC);
                } else printf("Carregue o grafo primeiro.\n");
                break;
            case 4:
                if (g) {
                    printf("Digite o vértice inicial (ID ou Nome): ");
                    int start = get_vertex_from_user(g);
                    if (start == -1) break;
                    clock_t begin = clock();
                    algorithms_dfs(g, start, "output/saida_dfs.txt");
                    clock_t end = clock();
                    printf("DFS concluído em %.4fs. Resultado salvo em output/saida_dfs.txt\n", (double)(end - begin) / CLOCKS_PER_SEC);
                } else printf("Carregue o grafo primeiro.\n");
                break;
            case 5:
                if (g) {
                    printf("Digite o vértice de origem (ID ou Nome): ");
                    int x = get_vertex_from_user(g);
                    if (x == -1) break;
                    printf("Digite o vértice de destino (ID ou Nome): ");
                    int y = get_vertex_from_user(g);
                    if (y == -1) break;
                    float d = algorithms_get_distance(g, x, y);
                    if (d == FLT_MAX) printf("Não existe caminho entre os vértices.\n");
                    else printf("Distância entre '%s' e '%s': %.2f\n", 
                                graph_get_vertex_name(g, x) ? graph_get_vertex_name(g, x) : "Origem",
                                graph_get_vertex_name(g, y) ? graph_get_vertex_name(g, y) : "Destino", d);
                } else printf("Carregue o grafo primeiro.\n");
                break;
            case 6:
                if (g) {
                    char outname[] = "output/informacoes_grafo.txt";
                    printf("Escolha o método de cálculo do diâmetro:\n1 - Exato (V passagens BFS - lento para redes grandes)\n2 - Aproximado (2-BFS - rápido para redes grandes)\nOpção: ");
                    int diamOp; scanf("%d", &diamOp);
                    
                    printf("Calculando diâmetro e informações...\n");
                    clock_t begin = clock();
                    float diam = (diamOp == 2) ? algorithms_get_approx_diameter(g) : algorithms_get_diameter(g);
                    algorithms_save_info(g, outname, diam);
                    
                    int num_comps;
                    Component *comps = algorithms_find_connected_components(g, &num_comps);
                    algorithms_save_components(comps, num_comps, outname);
                    clock_t end = clock();
                    
                    printf("Informações salvas com sucesso em %s\n", outname);
                    printf("Diâmetro calculado: %.2f\n", diam);
                    printf("Memória utilizada: %.2f MB\n", graph_get_memory_usage(g));
                    printf("Tempo de processamento: %.4fs.\n", (double)(end - begin) / CLOCKS_PER_SEC);
                    algorithms_destroy_components(comps, num_comps);
                } else printf("Carregue o grafo primeiro.\n");
                break;
            case 7:
            case 8:
                if (g) {
                    if (g->has_negative_weights) {
                        printf("A biblioteca não suporta pesos negativos para Dijkstra.\n");
                        break;
                    }
                    printf("Digite o vértice inicial (ID ou Nome): ");
                    int start = get_vertex_from_user(g);
                    if (start == -1) break;
                    
                    clock_t begin = clock();
                    DijkstraResult res = (op == 7) ? algorithms_dijkstra_with_heap(g, start) : algorithms_dijkstra_without_heap(g, start);
                    clock_t end = clock();
                    
                    printf("Dijkstra concluído em %.4fs.\n", (double)(end - begin) / CLOCKS_PER_SEC);
                    
                    printf("Informe o vértice de destino para ver a distância (ou Enter para pular): ");
                    int dest = get_vertex_from_user(g);
                    if (dest != -1) {
                        if (res.dist[dest] == FLT_MAX) printf("Não existe caminho até o destino.\n");
                        else printf("Distância mínima até '%s': %.2f\n", 
                               graph_get_vertex_name(g, dest) ? graph_get_vertex_name(g, dest) : "Destino", 
                               res.dist[dest]);
                    }

                    algorithms_destroy_dijkstra_result(res);
                } else printf("Carregue o grafo primeiro.\n");
                break;
            case 9:
                if (g) {
                    printf("Escolha o tipo de busca para o benchmark de 100 execuções:\n1 - BFS\n2 - DFS\n3 - Dijkstra (com Heap)\n4 - Dijkstra (sem Heap)\nOpção: ");
                    int benchOp; scanf("%d", &benchOp);
                    algorithms_benchmark_100_runs(g, benchOp);
                } else printf("Carregue o grafo primeiro.\n");
                break;
            case 0:
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (op != 0);

    if (g) graph_destroy(g);
    return 0;
}

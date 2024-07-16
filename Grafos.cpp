#include <stdio.h>
#include<time.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Grafos.h"
#include "Queue.h"

No *newNo(int w)
{
    No *no = (No *)malloc(sizeof(No));
    if (no == NULL)
        return NULL;
    no->w = w;
    no->prox = NULL;
    return no;
}
graph *newGraph(int V)
{
    graph *grafo = (graph *)malloc(sizeof(graph));
    if (grafo)
    {
        grafo->V = V;
        grafo->U = 0;
        grafo->adj = (No **)malloc(V * sizeof(No *));
        grafo->visitado = (int *)malloc(V * sizeof(int));
        for (int i = 0; i < V; i++)
        {
            grafo->adj[i] = NULL;
            grafo->visitado[i] = 0;
        }
    }
    return grafo;
}

void addAresta(graph *grafo, int v, int w)
{
    No *no = newNo(w);
    no->prox = grafo->adj[v];
    grafo->adj[v] = no;

    no = newNo(v);
    no->prox = grafo->adj[w];
    grafo->adj[w] = no;
    grafo->U++;
}
void exibeGraph(graph *grafo)
{
    for (int v = 0; v < grafo->V; v++)
    {
        No *p = grafo->adj[v];
        printf("\n Vertice %d: \n", v);
        while (p)
        {
            printf("\n->%d", p->w);
            p = p->prox;
        }
        printf("\n");
    }
}
void freeGraph(graph *grafo)
{
    for (int v = 0; v < grafo->V; v++)
    {
        No *p = grafo->adj[v];
        while (p)
        {
            No *aux = p;
            p = p->prox;
            free(aux);
        }
    }
    free(grafo->adj);
    free(grafo->visitado);
    free(grafo);
}
graph *initGraphArq(const char *cam)
{
    FILE *arq = fopen(cam, "r");
    if (arq == NULL)
        return NULL;
    int V;
    fscanf(arq, "%d", &V);
    graph *g = newGraph(V);
    printf("\n%d", g->visitado[0]);
    int v, u;
    while (fscanf(arq, "%d %d", &v, &u) != EOF)
    {
        addAresta(g, v - 1, u - 1);
    }
    fclose(arq);
    return g;
}
int numArestasL(graph *grafo)
{
    int arestas = 0;
    for (int i = 0; i < grafo->V; i++)
    {
        No *aux = grafo->adj[i];
        while (aux)
        {
            arestas++;
            aux = aux->prox;
        }
    }
    return arestas / 2;
}
int numVertL(graph *grafo)
{
    return grafo->V;
}
int grauVertL(graph *grafo, int *graus)
{
    for (int i = 0; i < grafo->V; i++)
    {
        graus[i] = 0;
        No *aux = grafo->adj[i];
        while (aux)
        {
            graus[i]++;
            aux = aux->prox;
        }
    }
    return 0;
}
int grauMinL(graph *grafo)
{
    int graus[grafo->V];
    grauVertL(grafo, graus);
    int min = graus[0];
    for (int i = 1; i < grafo->V; i++)
    {
        if (graus[i] < min)
        {
            min = graus[i];
        }
    }
    return min;
}

int grauMaxL(graph *grafo)
{
    int graus[grafo->V];
    grauVertL(grafo, graus);
    int max = graus[0];
    for (int i = 0; i < grafo->V; i++)
    {
        if (graus[i] > max)
        {
            max = graus[i];
        }
    }
    return max;
}

int grauMedioL(graph *grafo)
{
    int graus[grafo->V];
    grauVertL(grafo, graus);
    int soma = 0;
    for (int i = 0; i < grafo->V; i++)
    {
        soma = soma + graus[i];
    }
    return soma / grafo->V;
}
void outputFile(graph *grafo, const char *namefile)
{
    FILE *arq = fopen(namefile, "w");
    if (arq == NULL)
        perror("Erro ao abrir arquivo de saida");
    fprintf(arq, "\nO Grafo escolhido contem: \n");
    fprintf(arq, "\nNumero de vertices: %d\n", numVertL(grafo));
    fprintf(arq, "\nNumero de arestas: %d\n", numArestasL(grafo));
    fprintf(arq, "\nGrau minimo do grafo: %d\n", grauMinL(grafo));
    fprintf(arq, "\nGrau maximo de um grafo: %d\n", grauMaxL(grafo));
    fprintf(arq, "\nGrau Medio do grafo: %d\n", grauMedioL(grafo));

    fclose(arq);
}

void dfs(graph *grafo, int v, Info_Vertice vert[])
{
    grafo->visitado[v] = 1;
    int w;
    No *aux = grafo->adj[v];
    while (aux != NULL)
    {
        w = aux->w;
        if (w < 0 || w >= grafo->V)
        {
            printf("Erro");
            exit(EXIT_FAILURE);
        }
        if (!grafo->visitado[w])
        {
            vert[w].pai = v;
            vert[w].profundidade = vert[v].profundidade + 1;
            dfs(grafo, w, vert);
        }
        aux = aux->prox;
    }
} /*
 void DFS(graph *G, int v){
     struct No* adjs= G->adj[v];

     printf("\n%d", G->visitado[0]);
     struct No* aux= adjs;

     printf("\n%d", v);
     G->visitado[v]=1;
     while(aux!=NULL){
         int cv= aux->w;
         if(G->visitado[cv]==0){
             G->visitado[cv].pai = v;
             G->visitado[cv].profundidade= G->visitado[v].profundidade+1;
             DFS(G, cv);

         }
         aux= aux->prox;
     }

 }*/
int writeGeneTree(const char *namefile, int v, graph *G)
{
    v--;
    Info_Vertice *vert = (Info_Vertice *)malloc(sizeof(Info_Vertice) * G->V - 1);
    int w;
    if (vert == NULL)
        exit(EXIT_FAILURE);
    for (w = 0; w < G->V; w++)
    {
        G->visitado[w] = 0;
        vert[w].profundidade = 0;
        vert[w].pai = -1;
    }

    dfs(G, v, vert);
    // DFS(G, v);
    FILE *arq = fopen(namefile, "w");
    if (!arq)
    {
        free(vert);
        return 0;
        perror("Erro ao abrir o arquivo de saida");
    }
    for (int w = 0; w < G->V; w++)
    {
        if (G->visitado[w])
        {
            fprintf(arq, "Vertice=%u\tPai=%d\tProfundidade=%d\n", w + 1, vert[w].pai + 1, vert[w].profundidade);
        }
    }
    fclose(arq);
    free(vert);
    return 1;
}
int bfs(graph *grafo, int v, Info_Vertice vert[])
{
    clock_t start, end;
    double time_t;
    start= clock();
    queue *queue = createQueue(grafo->V);
    grafo->visitado[v] = 1;
    vert[v].profundidade = 0;
    int last_vertice = v;
    enqueue(queue, v);
    while (!isEmpty(queue))
    {
        int u = dequeue(queue);
        No *aux = grafo->adj[u];
        while (aux != NULL)
        {
            int w = aux->w;
            if (!grafo->visitado[w])
            {
                grafo->visitado[w] = 1;
                vert[w].pai = u;
                vert[w].profundidade = vert[u].profundidade + 1;
                enqueue(queue, w);
                last_vertice = w;
            }
            aux = aux->prox;
        }
    }
    free(queue->dados);
    free(queue);
    end= clock();
    time_t= ((double)(end-start))/CLOCKS_PER_SEC;
    printf("Tempo de execucao do BFS: %f segundos\n", time_t);
    return last_vertice;
}
int writeGeneTreeBFS(const char *namefile, int v, graph *G)
{
    v--;
    Info_Vertice *vert = (Info_Vertice *)malloc(sizeof(Info_Vertice) * G->V);
    int w;
    if (vert == NULL)
        exit(EXIT_FAILURE);
    for (w = 0; w < G->V; w++)
    {
        G->visitado[w] = 0;
        vert[w].profundidade = 0;
        vert[w].pai = -1;
    }
    vert[v].profundidade = 0;
    bfs(G, v, vert);
    FILE *arq = fopen(namefile, "w");
    if (!arq)
    {
        free(vert);
        perror("Erro ao abrir o arquivo de saida");
        return 0;
    }
    for (int w = 0; w < G->V; w++)
    {
        if (G->visitado[w])
        {
            fprintf(arq, "Vertice=%u\tPai=%d\tProfundidade=%d\n", w + 1, vert[w].pai + 1, vert[w].profundidade);
        }
    }
    fclose(arq);
    free(vert);
    return 1;
}

int Distance(graph *G, int v, int u)
{
    if (v == u)
        return 0;
    Info_Vertice *vert = (Info_Vertice *)malloc(sizeof(Info_Vertice) * G->V);
    if (vert == NULL)
        exit(EXIT_FAILURE);
    for (int i = 0; i < G->V; i++)
        G->visitado[i] = 0;
    bfs(G, v, vert);
    int distance = vert[u].profundidade;
    free(vert);
    return distance;
}
int eccentricity(graph *G, int v)
{
    Info_Vertice *vert = (Info_Vertice *)malloc(sizeof(Info_Vertice) * G->V);
    if (vert == NULL)
        exit(EXIT_FAILURE);
    for (int i = 0; i < G->V; i++)
        G->visitado[i] = 0;
    int vert_distante = bfs(G, v, vert);
    int eccen = vert[vert_distante].profundidade;
    free(vert);
    return eccen;
}
int diameter(graph *G)
{
    int diametro = 0;
    for (int i = 0; i < G->V; i++)
    {
        int dist = eccentricity(G, i);
        if (dist > diametro)
        {
            diametro = dist;
        }
    }
    return diametro;
}
int Diameter(graph *g)
{
    Info_Vertice *vert = (Info_Vertice *)malloc(sizeof(Info_Vertice) * g->V);
    if (vert == NULL)
        exit(EXIT_FAILURE);
    for (int i = 0; i < g->V; i++)
        g->visitado[i] = 0;
    int v = bfs(g, 0, vert);
    for (int i = 0; i < g->V; i++)
        g->visitado[i] = 0;
    int u = bfs(g, v, vert);
    int diametro_prox = vert[u].profundidade;
    free(vert);
    return diametro_prox;
}
void bfs_cc(graph *g, int v, int *visited, int *componet, int *size){
    queue *q= createQueue(g->V);
    enqueue(q, v);
    visited[v]=1;

    while(!isEmpty(q)){
        int u= dequeue(q);
        componet[*size]= u;
        (*size)++;
        No *aux= g->adj[u];
        while(aux!=NULL){
            int w= aux->w;
            if(!visited[w]){
                visited[w]=1;
                enqueue(q, w);
            }
            aux=aux->prox;
        }
    }
    free(q->dados);
    free(q);
}/*
static ConnectedComponets *initCc(){
    ConnectedComponets *cc= (ConnectedComponets *)malloc(sizeof(ConnectedComponets));
    if(cc == NULL)
        exit(EXIT_FAILURE);
    cc->components = (Componet *)malloc(sizeof(Componet)*10);
    if(cc->components==NULL)
        exit(EXIT_FAILURE);
    cc->numComponents = 0;
    cc->size= 10;
    return cc;
}

static void Lrealloc(ConnectedComponets *cc, int newSize){
    cc->components= (Componet *)realloc(cc->components, sizeof(Componet)* newSize);
    if(cc->components==NULL)
        exit(EXIT_FAILURE);
    cc->size= newSize;
}

void AddVtoCc(Componet *C, int vertex){
    C->vertices= (int*)realloc(C->vertices, (C->size+1));
    C->vertices[C->size]= vertex;
    C->size++;
}
ConnectedComponets *findCc(graph *G){
   ConnectedComponets *cc = initCc();
   int *visited= (int *)malloc(G->V* sizeof(int));
   for(int i=0; i< G->V;i++){
    visited[i]=0;
   }
   for(int v=0;v<G->V;v++){
    if(!visited[v]){
        if(cc->numComponents==cc->size){
            Lrealloc(cc, cc->size*2);
        }
        Componet *C= &cc->components[cc->numComponents];
        C->vertices= NULL;
        C->size=0;
        queue *q = createQueue(G->V);
        visited[v]=1;
        while(!isEmpty(q)){
            int u= dequeue(q);
            AddVtoCc(C,u);
            No *aux= G->adj[u];
            while(aux!=NULL){
                int w = aux->w;
                if(!visited[w]){
                    enqueue(q,w);
                    visited[w]=1;
                }
                aux= aux->prox;
            }
        }
        cc->numComponents++;
        free(q->dados);
        free(q);
    }
   }
   free(visited);
   return cc;

}*/
static ConnectedComponets allocCc(){
    ConnectedComponets *cc= (ConnectedComponets *)malloc(sizeof(ConnectedComponets));
    if(cc==NULL)
        exit(EXIT_FAILURE);
    cc->components= (Componet*)malloc(sizeof(Componet)*10);
    if(cc->components==NULL)
        exit(EXIT_FAILURE);
    cc->numComponents=0;
    cc->size=10;
    return *cc;
}
static void ReallocC(ConnectedComponets cc,int newSize){
    cc.components=(Componet*)realloc(cc.components, sizeof(Componet)*newSize);
    if(cc.components==NULL)
        exit(EXIT_FAILURE);
    cc.size= newSize;
}
void AddVtoCc(Componet *C, int vertex){
    C->vertices=(int *)realloc(C->vertices, (C->size + 1) * sizeof(int));
    C->vertices[C->size]=vertex;
    C->size++;
}
void dfsRCc(graph *g, int v, int id){
    cc[v]=id;
    No *aux= g->adj[v];
    while(aux!=NULL){
        int w = aux->w;
        if(cc[w]==0)
            dfsRCc(g,w, id);
        aux= aux->prox;
    }
}
int GraphCc(graph *G)
{
    int id=0;
    cc= (int *)malloc(G->V *sizeof(int));
    for(int v=0;v<G->V;v++)
        cc[v]=-1;
    for(int v=0;v<G->V;++v)
        if(cc[v]==-1)
            dfsRCc(G,v,id++);
    return id;
}
ConnectedComponets findCC(graph *G){
    int numC= GraphCc(G);
    ConnectedComponets ccstr= allocCc();
    if(numC>ccstr.size)
        ReallocC(ccstr, numC);
    for(int i=0; i<numC;i++){
        ccstr.components[i].vertices=NULL;
        ccstr.components[i].size=0;
    }
    for(int v=0;v<G->V;v++){
        AddVtoCc(&ccstr.components[cc[v]],v);
    }
    ccstr.numComponents= numC;
    free(cc);
    return ccstr;    
}
void freeCc(ConnectedComponets *cc){
    for(int i=0;i<cc->numComponents;i++){
        free(cc->components[i].vertices);
    }
    free(cc->components);
    cc->numComponents=0;
}
int compVertexes(const void *a, const void *b){
    return (*(int *)a - *(int *)b);
}
void sortCc(ConnectedComponets *cc){
    for(int i=0; i<cc->numComponents;i++){
        qsort(cc->components[i].vertices, cc->components[i].size, sizeof(int), compVertexes);
    }
}
void showConnectedComponents(graph *g){
    ConnectedComponets cc = findCC(g);
    sortCc(&cc);
    printf("\nNumero de componentes conexas: %d\n", cc.numComponents);
    for(int i=0;i<cc.numComponents;i++){
        printf("Componente %d: tamanho %d, vertices= ", i+1, cc.components[i].size);
        for(int j=0;j<cc.components[i].size;j++){
            printf("%d", cc.components[i].vertices[j]+1);
        }
        printf("\n");
    }
    freeCc(&cc);
}
int UGraphConnect(graph G, int s, int t){
    return cc[s]== cc[t];
}



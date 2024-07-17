#include<stdio.h>
#include<stdlib.h>
	
//para ambas
	typedef struct Nodo
	{
		int vertice,
			verticePai,
			nivel;
		float peso;
		struct Nodo *prox;
	}No;

	typedef struct 
	{
		int qtdVertices;
		No *cabeca;
	}ComponenteConexa;
	
	typedef struct 
	{
	    int vertice;
	    float distancia;
	}Heap;
	
	int minDistance(float dist[], int sptSet[], int nVertices);
	void heapifyDown(Heap heap[], int posicao[], int tamanhoHeap, int pai);	
	int removeDaHeap(Heap *heap, int posicao[], int *tamanhoDaHeap);
	void heapifyUp(Heap *heap,int posicao[], int filho);
	void adicionaNaHeap(Heap heap[], int posicao[], int *tamanhoDaHeap, Heap element);
    void mostrarComponentesConexas(ComponenteConexa listaConexos[], int qtdComponentes);
	void trocaGraus(int *a, int *b);
	int particaoGraus(int graus[], int inicio, int fim);
	void quicksortGraus(int graus[], int inicio, int fim);
	void trocaComponentes(ComponenteConexa *a, ComponenteConexa *b);
	int particaoComponentes(ComponenteConexa lista[], int inicio, int fim);
    void quicksortComponentes(ComponenteConexa lista[], int inicio, int fim);
	ComponenteConexa* criarListaConexos();
	void freeComponentesConexas(ComponenteConexa **visitados, int qtdComponentes);
	void addComponenteConexa(ComponenteConexa **listaConexos, ComponenteConexa novaComponente, int novoTamanho);
//------

//destinadas a MATRIZ DE ADJACENCIA:
    typedef struct
	{
	    int qtdComponentesConexas,
	    	qtdArestas,
			qtdVertices;
	    float **grafo;
	}GrafoMatrizAdj;
	
	void freeGrafo(GrafoMatrizAdj **grafo);
	void criarGrafo(GrafoMatrizAdj **grafo, int qtdVertices);
	void lerArquivoDeGrafoMatriz(GrafoMatrizAdj **grafo);
	void mostrarGrafoMatriz(GrafoMatrizAdj *grafo);
	
	void dijkstraMatrizComHeap(GrafoMatrizAdj *grafo, int verticeInicial);
	Heap *caminhoMinimoParaTodos(GrafoMatrizAdj *grafo, int verticeInicial);
	void dijkstraMatrizSemHeap(GrafoMatrizAdj *grafo, int verticeInicial);
	
	void bfsMatriz(GrafoMatrizAdj *grafo, int verticeInicial);
	ComponenteConexa* encontrarComponente(GrafoMatrizAdj *grafo, int visitados[], int verticeInicial);
	int grauDoVertice(GrafoMatrizAdj *grafo, int vertice);
	int grauMinimo(GrafoMatrizAdj *grafo);
	int grauMaximo(GrafoMatrizAdj *grafo);
	float grauMedio(GrafoMatrizAdj *grafo);
	float medianaDeGrau(GrafoMatrizAdj *grafo);
	float distanciaEntreVertices(GrafoMatrizAdj *grafo, int verticeOrigem, int verticeDestino);
	float diametroGrafoMatriz(GrafoMatrizAdj *grafo);
	int contarArestasMatriz(GrafoMatrizAdj *grafo);
	void informacoesGrafoMatrizToArq(GrafoMatrizAdj *grafoMatriz);
	void componentesConexasToArq(ComponenteConexa listaConexos[], int qtdComponentes);
	void listarComponentesConexas(GrafoMatrizAdj *grafo);
	void dfsMatriz(GrafoMatrizAdj *grafo, int verticeInicial);
	
//-----------

//destinadas a LISTA DE ADJACENCIA:
	typedef struct 
	{
		int qtdVertices, qtdArestas,
			qtdComponentesConexas;
		No** listaAdj;
	}GrafoListaAdj;
	
	void criarGrafoLista(GrafoListaAdj **grafo, int qtdVertices);
	void freeGrafoLista(GrafoListaAdj **grafo);
	int grauVerticeLista(GrafoListaAdj *grafo, int vertice);
	int grauMinimoLista(GrafoListaAdj *grafo);
	int grauMaximoLista(GrafoListaAdj *grafo);
	float grauMedioLista(GrafoListaAdj *grafo);
	float medianaDeGrauLista(GrafoListaAdj *grafo);
	float distanciaEntreVerticesLista(GrafoListaAdj *grafo, int verticeOrigem, int verticeDestino);
	float diametroGrafoLista(GrafoListaAdj *grafo);
	
	void dijkstraListaComHeap(GrafoListaAdj *grafo, int verticeInicial);
	Heap *caminhoMinimoParaTodoslista(GrafoListaAdj *grafo, int verticeInicial);
	void dijkstraListaSemHeap(GrafoListaAdj* grafo, int verticeInicial);
	
	int contarArestasLista(GrafoListaAdj *grafo);
	void lerArquivoDeGrafoLista(GrafoListaAdj **grafo);
	void mostrarGrafoLista(GrafoListaAdj **grafo);
	void bfsLista(GrafoListaAdj grafo, int verticeInicial);
	void dfsLista(GrafoListaAdj *grafo, int verticeInicial);
	ComponenteConexa* encontrarComponenteLista(GrafoListaAdj *grafo, int visitados[], int verticeInicial);
	void componentesConexasListaToArq(ComponenteConexa listaConexos[], int qtdComponentes);
	void listarComponentesConexasLista(GrafoListaAdj *grafo);
	void informacoesGrafoListaToArq(GrafoListaAdj *grafo);

//-----------


	

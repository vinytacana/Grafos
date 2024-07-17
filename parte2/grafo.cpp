#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "grafo.h"
#include <float.h>

int minDistance(float dist[], int sptSet[], int nVertices)
{
    float min = FLT_MAX; 
	int	min_index = -1, 
		v;

    for (v = 0; v < nVertices; v++)
    {
    	if (sptSet[v] == 0 && dist[v] <= min)
        {
        	min = dist[v];
			min_index = v;
		}
	}
        
    return min_index;
}

void heapifyDown(Heap heap[], int posicao[], int tamanhoHeap, int pai) //correta
{
    int minimo = pai,//considera que o pai possui a menor distancia
		esquerda = 2 * pai + 1,
		direita = 2 * pai + 2;
		
	//busca encontrar qual dos filhos tem a menor distancia, se encontrar, altera o indice da posi��o com a menor distancia.
    if ((esquerda < tamanhoHeap) && (heap[esquerda].distancia < heap[minimo].distancia))
        minimo = esquerda;

    if ((direita < tamanhoHeap) && (heap[direita].distancia < heap[minimo].distancia))
        minimo = direita;

    if (minimo != pai) //se o pai n�o era o menor valor, ent�o realiza a troca e chama a fun��o para continuar a rebaixar o novo valor pai.
	{
        Heap temp = heap[minimo];
		heap[minimo] =  heap[pai];
		heap[pai] = temp;
		
		posicao[heap[pai].vertice] = pai;
        posicao[heap[minimo].vertice] = minimo;
		
        heapifyDown(heap, posicao,tamanhoHeap, minimo);
    }
}

int removeDaHeap(Heap *heap, int posicao[], int *tamanhoDaHeap)//correta
{
	if(*tamanhoDaHeap == 0)
	{
		printf("\nHEAP VAZIA");
		return -1;
	}
		
	int elementoRemovido = heap[0].vertice;//salva o elemento a ser removido(apenas para retornar ele depois)
	 
	heap[0] = heap[(*tamanhoDaHeap) - 1];//para eliminar da heap, colocamos o ultimo elemento como o primeiro.
	posicao[ elementoRemovido ] = -1;
	(*tamanhoDaHeap)--;
	
	heapifyDown(heap,posicao,*tamanhoDaHeap,0);//verificacao para ver se a propriedade da heap nao foi quebrada.
	
	
	return elementoRemovido;
}

void heapifyUp(Heap *heap,int posicao[], int filho)//correta
{
    int pai = (filho-1)/ 2;//calcular o pai do filho
    
    while((filho > 0) && (heap[pai].distancia > heap[filho].distancia)) //se nao estiver na raiz e a distancia do pai for maior que a do filho
	{
		//troca o pai pelo filho
        Heap temp = heap[filho];
        heap[filho] = heap[pai];
        heap[pai] = temp;
        
        posicao[heap[filho].vertice] = filho;
        posicao[heap[pai].vertice] = pai;
        
        filho = pai;//atualiza o filho para o proximo pai
        pai = (filho-1)/ 2;//calcula o novo pai
    }
}

void adicionaNaHeap(Heap heap[], int posicao[], int *tamanhoDaHeap, Heap element)//correta
{
	//se ele esta na heap, troca a distancia e faz algo
	//se nao, adiciona normalmente
	
	if(posicao[element.vertice] != -1)
	{
		int i = posicao[element.vertice];

        if (heap[i].distancia > element.distancia) 
		{
            heap[i].distancia = element.distancia;
            heapifyUp(heap, posicao, i);
            heapifyDown(heap, posicao, *tamanhoDaHeap, i);
        }
	}
	else
	{
		heap[*tamanhoDaHeap] = element;
		posicao[element.vertice] = *tamanhoDaHeap;
	    (*tamanhoDaHeap)++;
	 
	    if(*tamanhoDaHeap > 1)//so come�a se haver pelo menos 2 elementos na heap
			heapifyUp(heap,posicao, *tamanhoDaHeap-1);
	}
}

void dijkstraMatrizComHeap(GrafoMatrizAdj *grafo, int verticeInicial)// kk...
{
    float distancia[grafo->qtdVertices]; // armazena a distancia entre o verticeInicial para i
	int posicaoNaHeap[grafo->qtdVertices],
	    determinado[grafo->qtdVertices],
		tamanhoDaHeap = 0,
		i; 
	Heap heap[grafo->qtdVertices],
		  novoElemento;
		  
    for (i = 0; i < grafo->qtdVertices; i++)
    {
    	distancia[i] = FLT_MAX;
		determinado[i] = 0;
		posicaoNaHeap[i] = -1;
	}
	
    distancia[verticeInicial] = 0;
    
    for (i = 0; i < grafo->qtdVertices; i++)
    {
    	heap[i].distancia = distancia[i];
    	heap[i].vertice = i;
	}
	
    novoElemento.distancia = distancia[verticeInicial];
    novoElemento.vertice = verticeInicial;
    adicionaNaHeap(heap,posicaoNaHeap,&tamanhoDaHeap, novoElemento);
    
	while(tamanhoDaHeap > 0)
	{
		int vizinho,
			min = removeDaHeap(heap,posicaoNaHeap, &tamanhoDaHeap);  //pega o vertice nao determinado com a menor distancia
			
		if(min == -1)
			break;
			
        determinado[min] = 1;//marca o vertice como determinado 

        for (vizinho = 0; vizinho < grafo->qtdVertices; vizinho++)
        {
            if ((determinado[vizinho] == 0 ) && (grafo->grafo[min][vizinho] > 0) && (distancia[min] != FLT_MAX) && (distancia[min] + grafo->grafo[min][vizinho] < distancia[vizinho]))
            {
            	distancia[vizinho] = distancia[min] + grafo->grafo[min][vizinho];
            	novoElemento.distancia = distancia[vizinho];
            	novoElemento.vertice = vizinho;
            	adicionaNaHeap(heap, posicaoNaHeap, &tamanhoDaHeap, novoElemento);
			}
		}
	}
}

Heap *caminhoMinimoParaTodos(GrafoMatrizAdj *grafo, int verticeInicial)
{
    float distancia[grafo->qtdVertices]; // armazena a distancia entre o verticeInicial para i
	int posicaoNaHeap[grafo->qtdVertices],
	    determinado[grafo->qtdVertices],
		tamanhoDaHeap = 0,
		count,
		i; 
	Heap *heap = (Heap*)(sizeof(Heap) * grafo->qtdVertices),
		  novoElemento;
	if(heap == NULL)
	{
		printf("\nFUNCAO: caminhoMinimoParaTodos() - nao foi possivel criar a heap para a funcao.");
		return NULL;
	}
    
    for (i = 0; i < grafo->qtdVertices; i++)//inicializa todas as distancias com INFINITO e os determinados como 0.
    {
    	distancia[i] = FLT_MAX;
		determinado[i] = 0;
		posicaoNaHeap[i] = -1;
	}
	
    distancia[verticeInicial] = 0;//inicializa a distancia do vertice inicial como 0
    
    for (i = 0; i < grafo->qtdVertices; i++)
    {
    	heap[i].distancia = distancia[i];
    	heap[i].vertice = i;
	}
	
	for(i = 0;i<grafo->qtdVertices -1;i++)
	{
		int vizinho,
			min = minDistance(distancia, determinado, grafo->qtdVertices);  //pega o vertice nao determinado com a menor distancia
			
		if(min == -1)
			break;
			
        determinado[min] = 1;//marca o vertice como determinado 

        for (vizinho = 0; vizinho < grafo->qtdVertices; vizinho++)
        {
            if ((determinado[vizinho] == 0 ) && (grafo->grafo[min][vizinho] > 0) && (distancia[min] != FLT_MAX) && (distancia[min] + grafo->grafo[min][vizinho] < distancia[vizinho]))
            {
            	distancia[vizinho] = distancia[min] + grafo->grafo[min][vizinho];
			}
		}
	}
	for(i=0;i<grafo->qtdVertices;i++)
    {
    	novoElemento.distancia = distancia[i];
    	novoElemento.vertice = i;
    	adicionaNaHeap(heap, posicaoNaHeap, &tamanhoDaHeap, novoElemento);
	}
	
	return heap;
}

void dijkstraMatrizSemHeap(GrafoMatrizAdj *grafo, int verticeInicial)
{
    float distancia[grafo->qtdVertices]; // armazena a distancia entre o verticeInicial para i
	int determinado[grafo->qtdVertices],
		count,
		v,
		i; 
    
    for (i = 0; i < grafo->qtdVertices; i++)//inicializa todas as distancias com INFINITO e os determinados como 0.
    {
    	distancia[i] = FLT_MAX;
		determinado[i] = 0;
	}
       
    distancia[verticeInicial] = 0;//inicializa a distancia do vertice inicial como 0

    for (count = 0; count < (grafo->qtdVertices - 1); count++)//garantir que se repita para todos os vertices.
	{
        int u = minDistance(distancia, determinado, grafo->qtdVertices);//pega o vertice nao determinado com a menor distancia
		
		if(u == -1)
			break;
			
        determinado[u] = 1;//marca o vertice como determinado 

        for (v = 0; v < grafo->qtdVertices; v++)
            // Atualiza as distancias se os vertices nao tiverem sido determinados, se ha uma aresta
			// e o peso total do caminho do verticeInicial para V atraves de U � o menor que o valor atual de distancia[V] 
            if ((determinado[v] == 0 ) && (grafo->grafo[u][v] > 0) && (distancia[u] != FLT_MAX) && (distancia[u] + grafo->grafo[u][v] < distancia[v]))
                distancia[v] = distancia[u] + grafo->grafo[u][v];
    }
}

void trocaGraus(int *a, int *b) 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int particaoGraus(int graus[], int inicio, int fim) 
{
    int pivo = graus[(inicio + fim) / 2];
    int i = inicio - 1;
    int j = fim + 1;

    while (1) 
	{
        do {
            i++;
        } while (graus[i] > pivo);

        do {
            j--;
        } while (graus[j] < pivo);

        if (i >= j) {
            return j;
        }

        trocaGraus(&graus[i], &graus[j]);
    }
}

void quicksortGraus(int graus[], int inicio, int fim) 
{
    if (inicio < fim) 
	{
        int pos = particaoGraus(graus, inicio, fim);
        quicksortGraus(graus, inicio, pos);
        quicksortGraus(graus, pos + 1, fim);
    }
}

void trocaComponentes(ComponenteConexa *a, ComponenteConexa *b)
{
    ComponenteConexa temp = *a;
    *a = *b;
    *b = temp;
}

int particaoComponentes(ComponenteConexa lista[], int inicio, int fim) 
{
    int pivo = lista[(inicio + fim) / 2].qtdVertices;
    int i = inicio - 1;
    int j = fim + 1;

    while (1) 
	{
        do {
            i++;
        } while (lista[i].qtdVertices > pivo);

        do {
            j--;
        } while (lista[j].qtdVertices < pivo);

        if (i >= j) {
            return j;
        }

        trocaComponentes(&lista[i], &lista[j]);
    }
}

void quicksortComponentes(ComponenteConexa lista[], int inicio, int fim) 
{
    if (inicio < fim) 
	{
        int pos = particaoComponentes(lista, inicio, fim);
        quicksortComponentes(lista, inicio, pos);
        quicksortComponentes(lista, pos + 1, fim);
    }
}

ComponenteConexa* criarListaConexos()
{
	//COMENTARIO PARA A EXPLICACAO DESSA FUNCAO E SOBRE A COMPONENTE CONEXA----
	//	Em um grafo, uma componente conexa eh um subgrafo de um grafo maior, em que nao existe caminho que conecte os vertices que estao dentro
	//	da componente conexa com os vertices que estao fora dela, mas existe pelo menos um caminho que conecte qualquer par de vertices dentro
	//	dela.
	//	Essa funcao cria uma lista para armazenar os vertices encontrados em uma componente conexa.
	//DATA: 14/06/2024 	HORARIO: 11:46	AUTOR: MIKAEL PEREIRA GOMES----
	
	ComponenteConexa *newConexa = (ComponenteConexa*) malloc(sizeof(ComponenteConexa));
	if(newConexa == NULL)
	{
		printf("\nFUNCAO: criarListaConexos() - nao foi possivel criar uma nova componente conexa");
		return NULL;
	}
	
	newConexa->qtdVertices = 0;
	newConexa->cabeca = NULL;
	
	return newConexa;
}

void addVisitado(ComponenteConexa *visitados, int vertice, int pai, int nivel)
{
	//COMENTARIO PARA A EXPLICACAO DESSA FUNCAO----
	//	Tem o objetivo de adicionar um vertice no final da lista de vertices de uma componente conexa.
	//DATA: 15/06/2024 DATA: 22:23	AUTOR: MIKAEL PEREIRA GOMES----
	
	if(visitados == NULL)
	{
		printf("\nFUNCAO: addVisitado() - nao existe uma componente conexa para adicionar um novo vertice visitado a ela.");
		return;
	}
	
	No *novoVisitado = (No*)malloc(sizeof(No));
	if(novoVisitado == NULL)
	{
		printf("\nFUNCAO: addVisitado() - nao foi possivel adicionar um novo vertice a componente conexa");
		return;
	}
	
	No *nodoAtual = visitados->cabeca;//o nodo atual recebe a cabeca para poder percorrer
		 
	novoVisitado->vertice = vertice;
	if(nodoAtual != NULL)//caso NAO seja o primeiro nodo a ser adicionado
	{
		while(nodoAtual->prox != NULL)//quando o prox do nodo Atual for NULL, indica que encontramos o ultimo vertice.
		{
			nodoAtual = nodoAtual->prox;
	    }
	    novoVisitado->verticePai = pai;
		novoVisitado->nivel = nivel;//o nivel de cada vertice eh a soma do nivel do seu pai + 1
		novoVisitado->prox = nodoAtual->prox;
		nodoAtual->prox = novoVisitado;
	}
	else//caso SEJA o primeiro nodo a ser adicionado
	{
		novoVisitado->verticePai = pai;//o primeiro no eh a raiz, ele nao possui um pai
		novoVisitado->nivel = nivel;//o nivel do primeiro vertice eh 0
		novoVisitado->prox = NULL;
		visitados->cabeca = novoVisitado;
	}
	visitados->qtdVertices++;
}

void freeComponentesConexas(ComponenteConexa **visitados, int qtdComponentes)
{	
	//COMENTARIO PARA A EXPLICACAO DESSA FUNCAO----
	//	Tem o objetivo de liberar a memoria alocada para cada nodo/vertice presente na lista de vertices da componente conexa.
	//	e do vetor de componentes conexas.
	//DATA: 14/06/2024 DATA: 12:04	AUTOR: MIKAEL PEREIRA GOMES----
	
	if((*visitados) == NULL)
	{
		printf("\nFUNCAO: freeComponentesConexas() - nao existe uma componente conexa armazenada a ser removida.");
		return;
	}
	
	int i;
	No *nodoAtual,
		 *nodoProx;
		 
	for(i = 0;i < qtdComponentes;i++)
	{
		nodoAtual = (*visitados)[i].cabeca;//recebe o nodo cabeca da componente conexa i
		
		while(nodoAtual != NULL)//eliminando cada no a partir do inicio
		{
			nodoProx = nodoAtual->prox;
			free(nodoAtual);
			nodoAtual = nodoProx;
		}
	}

	free(*visitados);//libera o vetor de componentes conexas
	(*visitados) = NULL;
}

void addComponenteConexa(ComponenteConexa **listaConexos, ComponenteConexa novaComponente, int novoTamanho)
{
	//COMENTARIO PARA A EXPLICACAO DESSA FUNCAO----
	//	Ha um vetor de componentes conexas, essa funcao realoca memoria para esse vetor e adiciona uma nova componente
	//  a ele.
	//DATA: 14/06/2024	HORARIO: 17:06	AUTOR: MIKAEL PEREIRA GOMES.----
	
	//realoca primeiro para uma lista temporaria, para nao haver problemas em perda de dados.
	ComponenteConexa *listaTemporaria = (ComponenteConexa*) realloc(*listaConexos, novoTamanho * sizeof(ComponenteConexa));
	if(listaTemporaria == NULL)
	{
		printf("\nFUNCAO: addComponenteConexa() - nao foi possivel adicionar uma nova componente a lista de componentes conexas");
		return;
	}
	
	listaTemporaria[novoTamanho - 1 ] = novaComponente;//adiciona a nova componente conexa na ultima posicao do vetor
	(*listaConexos) = listaTemporaria;//faz a lista original apontar para a lista temporaria.
}

void freeGrafo(GrafoMatrizAdj **grafo)
{
	//DESTINADO A MATRIZ DE ADJACENCIA 
	//COMENTARIO PARA A EXPLICACAO DESSA FUNCAO----
	//	Tem o objetivo de liberar a memoria alocada para cada nodo/vertice presente em um grafo e do struct do tipo Grafo.
	//DATA: 14/06/2024 DATA: 12:08	AUTOR: MIKAEL PEREIRA GOMES----
	
	if((*grafo) == NULL)
	{
		printf("\nFUNCAO: freeGrafo() - nao ha grafo definido. Retornando...");
		return;
	}
	
	int i;
		
	for(i = 0;i < (*grafo)->qtdVertices;i++)
        free((*grafo)->grafo[i]);//liberando memoria dos vetores de colunas de cada linha
    
    free((*grafo)->grafo);//liberando memoria do vetor de linhas
    free(*grafo);//liberando memoria do struct grafo
    *grafo = NULL;
}

void criarGrafo(GrafoMatrizAdj **grafo, int qtdVertices)
{
    //DESTINADO A MATRIZ DE ADJACENCIA
	//COMENTARIO PARA A EXPLICACAO DESSA FUNCAO----
	//	Tem o objetivo de alocar memoria para uma grafo em formato de MATRIZ DE ADJACENCIA e para um struct do tipo Grafo.
	//DATA: 14/06/2024 DATA: 12:10	AUTOR: MIKAEL PEREIRA GOMES----
	
	//MODIFICADO----
	//	o tipo GrafoMatrizAdj agora armazena a quantidade de componentes conexas e a quantidade de arestas
	//	foi modificado para inicializar ambos valores como 0.
	//DATA: 17/06/2024	HORARIO: 22:24	AUTOR: MIKAEL PEREIRA GOMES----
	
    GrafoMatrizAdj *tempGrafo = (GrafoMatrizAdj*) malloc(sizeof(GrafoMatrizAdj));//criacao de um grafo temporario, para prevenir problemas.
    
    if(tempGrafo == NULL)
    {
        printf("\nFUNCAO: criarGrafo() - Nao foi possivel criar o grafo temporario.");
        return;
    }

    int i, j;

    tempGrafo->qtdVertices = qtdVertices;
    tempGrafo->qtdComponentesConexas = 0;
    tempGrafo->qtdArestas = 0;
    tempGrafo->grafo = (float**) malloc(sizeof(float*) * qtdVertices);//alocacao para o vetor de linhas
    if(tempGrafo->grafo == NULL)
    {
        printf("\n FUNCAO: criarGrafo() - Nao foi possivel alocar vetor de linhas para a matriz de adjacencia");
        return;
    }

    for(i = 0;i < qtdVertices;i++)
    {
        tempGrafo->grafo[i] = (float *) malloc(sizeof(float) * qtdVertices);//alocacao para o vetor de colunas de cada linha
        if(tempGrafo->grafo[i] == NULL)//CASO DE ERRO NA ALOCACAO DE UM DOS VETORES DE COLUNAS:
        {
            printf("\nFUNCAO: criarGrafo() - Nao foi possivel alocar um vetor de colunas para a linha %d da matriz",i);
            for(j = 0;j < i;j++)//vai ate onde o i conseguiu alocar, que eh i-1.
                free( tempGrafo->grafo[j]);//liberando memoria dos vetores de colunas de cada linha
                
            free(tempGrafo->grafo);//liberando memoria do vetor de linhas.
            free(tempGrafo);//liberando memoria do vetor do struct grafo
            return;
        }
    }

    *grafo = tempGrafo;//o grafo recebe o grafo temporario
    tempGrafo = NULL;
    
    printf("\nGrafo criado\n");
}

void lerArquivoDeGrafoMatriz(GrafoMatrizAdj **grafo)
{
    //DESTINADO A MATRIZ DE ADJACENCIA
	//COMENTARIO PARA A EXPLICACAO DESSA FUNCAO----
	//	Tem o objetivo de ler um arquivo contendo as conexoes dos vertices de um grafo para salvar em uma matriz de adjacencia
	//DATA: 14/06/2024 DATA: 12:15	AUTOR: MIKAEL PEREIRA GOMES----
    
    //MODIFICADO----
    //	agora ele consegue ler um arquivo com pesos.
    //DATA: 05/07/2024. HORARIO: 21:12. AUTOR: MIKAEL PEREIRA GOMES----
    
    FILE *arquivo = fopen("grafo_W_1.txt","r");
    if(arquivo == NULL)
    {
        printf("\nFUNCAO: lerArquivoDeGrafoMatriz() - Nao foi possivel abrir o arquivo.");
        return;
    }

    int qtdVertices = 0,
		linha = 2,//na estrutura do arquivo, a primeira linha eh a qtd de vertices, e nao ha linha 0, entao inicia na linha 2 pra frente.
        i,
        j;
    float peso;
    
    fscanf( arquivo, "%d\n", &qtdVertices );//primeiro elemento a ser lido eh a quantidade de vertices;

    *grafo = NULL;//inicializa o grafo com NULL
    
    criarGrafo(grafo, qtdVertices );
    if( grafo == NULL)
    {
        printf("\nFUNCAO: lerArquivoDeGrafo() - Como nao foi possivel criar um grafo, o arquivo nao sera lido.");
        return;
    }

    for(i = 0;i < qtdVertices;i++)
    {
        for(j = 0;j < qtdVertices;j++)
        {
            (*grafo)->grafo[i][j] = 0;//constroi o grafo primeiro com uma matriz de apenas pesos 0
        }
    }
    
    while(fscanf( arquivo , "%d %d %f\n", &i, &j, &peso ) != EOF)
    {	
		if((i >= qtdVertices) || (j >= qtdVertices) || (i < 0) || (j < 0))
		{
			printf("\nFUNCAO: lerArquivoDeGrafo() - Um indice invalido e/ ou maior que o esperado para o grafo foi encontrado na LINHA %d do arquivo ", linha);
			freeGrafo(grafo);
			return;
		}
		//grafo nao direcionado
    	(*grafo)->grafo[i][j] = peso;
    	(*grafo)->grafo[j][i] = peso;
		linha++;
	}
       
    printf("\nArquivo Lido\n");
    fclose(arquivo);
}

void mostrarGrafoMatriz(GrafoMatrizAdj *grafo)
{
	//DESTINADO A MATRIZ DE ADJACENCIA 
	
    int i,
        j;
	
	if(grafo == NULL)
	{
		printf("\nFUNCAO: mostrarGrafoMatriz() - nao ha grafo definido. Retornando...");
		return;
	}
	
	printf("\n");
	
    for(i = 0;i < grafo->qtdVertices;i++ )
    {
        for(j = 0;j < grafo->qtdVertices;j++)
        {
        	printf("PESO[%d][%d]: %f ", i, j,grafo->grafo[i][j]);
        }
        printf("\n");
    }
}

void bfsMatriz(GrafoMatrizAdj *grafo, int verticeInicial)
{
    //DESTINADO A MATRIZ DE ADJACENCIA 
    //COMENTARIO PARA A EXPLICACAO DESSA FUNCAO-----
	//	O BFS eh um algoritmo que encontra todos os vertices alcancaveis a partir do verticeInicial dado. Inicia pelo verticeInicial, 
	//	visitando todos os vertices adjacentes e colocando os nao visitados em uma fila. Ap�s enfileirar todos os vertices adjacentes 
	//  ao vertice atual, ele desenfileira um vertice para ser o novo vertice atual. Esse processo garante que todos os vertices a uma
	//  distancia menor do verticeInicial sejam explorados antes de vertices a distancias maiores."
	//DATA: 13/06/2024 HORARIO: 22:45 AUTOR: MIKAEL PEREIRA GOMES.-----
	
	//MODIFICADO----
	//	Agora ele salva no arquivo o caminho percorrido, contendo a informacao do vertice, do pai do vertice e de seu nivel.
	//DATA: 18/06/2024 HORARIO: 00:42 AUTOR: MIKAEL PEREIRA GOMES----
	
	//MODIFICADO----
	//	Agora ele salva o pai e o nivel de cada vertice corretamentekkkk
	//DATA: 19/06/2024 HORARIO: 15:03 AUTOR: MIKAEL PEREIRA GOMES----
	
    if(grafo == NULL)
	{
		printf("\nFUNCAO: bfsMatriz() - nao ha grafo definido. Retornando...");
		return;
	}
	
	if(verticeInicial < 0 || verticeInicial >= grafo->qtdVertices)
	{
		printf("\nFUNCAO: bfsMatriz() - vertice inicial invalido.");
		return;
	}
	
	FILE *arq = fopen("caminhoPercorridoBFSMatriz.txt","w");
	if(arq == NULL)
	{
		printf("\nFUNCAO: bfsMatriz() - nao foi possivel abrir o arquivo para salvar o caminho.");
		return;
	}
	
    int nivelDoVertice[grafo->qtdVertices],//vetor que armazena o nivel de cada vertice i
	    paiDoVertice[grafo->qtdVertices],//vetor que armazena o pai de cada vertice i
	    visitados[grafo->qtdVertices],//vetor para identificar se um vertice i foi visitado ou nao
	    fila[grafo->qtdVertices],//Criar uma fila para o BFS
        frente = 0, //indice do primeiro elemento da fila.
		tras = 0, //indice onde o proximo elemento sera inserido.
        i;
		
    for(i = 0;i < grafo->qtdVertices;i++)
    {	
		nivelDoVertice[i] = 0;//inicialmente nao conhecemos o nivel de cada vertice.
	    paiDoVertice[i] = -1;//inicialmente nao conhecemos o pai de cada vertice.
		visitados[i] = 0;//coloca todos os vertices como nao visitado
    	fila[i] = -1;//inicia fila com indices impossiveis
	}
   
    visitados[verticeInicial] = 1;// marca o vertice escolhido inicialmente como visitado
    fila[tras++] = verticeInicial;// enfileira ele.
	
    while (frente != tras) //quando forem iguais, indica que a fila esta vazia, e todos os elementos foram desenfileirados.
    {
        int verticeAtual =  fila[frente++];//vertice inicial eh igual ao proximo elemento na fila
    	
        printf("%d ->", verticeAtual);
        fprintf(arq,"VERTICE: %d. PAI: %d. NIVEL %d\n", verticeAtual, paiDoVertice[verticeAtual], nivelDoVertice[verticeAtual]);
		
        for (i = 0;i < grafo->qtdVertices;i++) 
        {	
        	//1 = visitado. 0 = nao visitado
            if ((grafo->grafo[verticeAtual][i] > 0) && (visitados[i] == 0))//se ha aresta e o adjacente i nao foi visitado
            {
				fila[tras++] = i;//enfileira ele
                visitados[i] = 1;//marca o adjacente do verticeAtual como visitado
                paiDoVertice[i] = verticeAtual;
                nivelDoVertice[i] = nivelDoVertice[verticeAtual] + 1;
            }
        }
    }
    
    printf("\nFUNCAO: bfsMatriz() - caminho salvo no arquivo 'caminhoPercorridoBFSMatriz'.");
    fclose(arq);
} 

ComponenteConexa* encontrarComponente(GrafoMatrizAdj *grafo, int visitados[], int verticeInicial)
{
    //DESTINADO A MATRIZ DE ADJACENCIA 
  	//COMENTARIO PARA A EXPLICACAO DESSA FUNCAO-----
	//	encontrarComponente() eh recomendado ser usado com listarComponentesConexas(), pois ele nao reinicia o vetor de visitados
	//	(assim conseguimos saber os vertices que nao foram visitados ainda) e tambem porque ele cria uma lista de vertices visitados
	//	para uma componente conexa. OBS: OBRIGATORIO QUE O verticeInicial NAO TENHA SIDO VISITADO PARA NAO CAUSAR PROBLEMAS. 
	//DATA: 13/06/2024 HORARIO: 22:43 AUTOR: MIKAEL PEREIRA GOMES.-----
	
	//MODIFICADO----
	//	Agora ele salva o pai e o nivel de cada vertice corretamente.
	//DATA: 19/06/2024 HORARIO: 15:37 AUTOR: MIKAEL PEREIRA GOMES----
	
    if(grafo == NULL)
	{
		printf("\nFUNCAO: encontrarComponente() - nao ha grafo definido. Retornando...");
		return NULL;
	}
	
	if(visitados[verticeInicial] == 1)
	{
		printf("\nFUNCAO: encontrarComponente() - o verticeInicial passado como argumento ja foi visitado. ");
		return NULL;
	}
	
    int nivelDoVertice[grafo->qtdVertices],//vetor que armazena o nivel de cada vertice i
		fila[grafo->qtdVertices],//Criar uma fila para o BFS
        frente = 0, //indice do primeiro elemento da fila.
		tras = 0, //indice onde o proximo elemento sera inserido.
        i;
    
    for(i = 0;i < grafo->qtdVertices;i++)
    {	
		nivelDoVertice[i] = 0;//inicialmente nao conhecemos o nivel de cada vertice.
    	fila[i] = -1;//inicia fila com indices impossiveis
	}
   
    visitados[verticeInicial] = 1;// marca o vertice escolhido inicialmente como visitado
    fila[tras++] = verticeInicial;// enfileira ele.
	
	ComponenteConexa *newConexos = criarListaConexos();//cria a lista da componente conexa
	if(newConexos == NULL)//impedir que a funcao continue caso ocorra erros na criacao
		return NULL;
		
	addVisitado(newConexos, verticeInicial, -1, 0); //adiciona o verticeInicial na lista
	
    // iterar pela fila
    while (frente != tras) //quando forem iguais, indica que a fila esta vazia, e todos os elementos foram desenfileirados.
    {
        int verticeAtual =  fila[frente++];//vertice inicial eh igual ao proximo elemento na fila
		
        for (i = 0;i < grafo->qtdVertices;i++) 
        {	
        	//1 = visitado. 0 = nao visitado
            if ((grafo->grafo[verticeAtual][i] == 1) && (visitados[i] == 0))//se ha aresta e o adjacente i nao foi visitado
            {
				fila[tras++] = i;//enfileira ele
                visitados[i] = 1;//marca o adjacente do verticeAtual como visitado
                nivelDoVertice[i] =  nivelDoVertice[verticeAtual] + 1;
              	addVisitado(newConexos, i, verticeAtual, nivelDoVertice[i]); //adiciona o vertice enfileirado na lista da Componente conexa.
            }
        }
    }
    
    grafo->qtdComponentesConexas++;
    return newConexos;//retorna a componente conexa encontrada
} 

void dfsMatriz(GrafoMatrizAdj *grafo, int verticeInicial) 
{
	//COMENTARIO PARA A EXPLICACAO DESSA FUNCAO:
	//	O algoritimo dfs encontra o caminho mais profundo partindo partindo de um VERTICE INICIAL, usando uma pilha para identificar
	//	o proximo vertice a ser visitado. Inicia empilhando o VERTICE INICIAL, entra no while, desempilha ele e marca como visitado,
	//	apos isso, entra em um for que empilha todos os vertices adjacentes ao VERTICE INICIAL que nao foram visitados, incrementando o topo
	//	E continua o while enquanto o TOPO for maior ou igual a 0, pois se o TOPO for negativo, indica que nao ha mais vertices empilhados.
	//	A funcao tambem mostra no arquivo o caminho percorrido.
	//DATA: 18/06/2024 HORARIO: 11:00 AUTOR: MIKAEL PEREIRA GOMES
		
	//---------------- AINDA NAO FIZ ELE ESCREVENDO NO ARQUIVO------------------------
	
    if (grafo == NULL) 
	{
        printf("FUNCAO: dfsMatriz() - Grafo nao definido\n");
        return;
    }
    if (verticeInicial < 0 || verticeInicial >= grafo->qtdVertices) {
        printf("FUNCAO: dfsMatriz() - vertice inicial invalido.\n");
        return;
    }
    
    FILE *arq = fopen("caminhoPercorridoDFSMatriz.txt","w");
    int nivelDoVertice[grafo->qtdVertices],
	    paiDoVertice[grafo->qtdVertices],
		visitados[grafo->qtdVertices],
		pilha[grafo->qtdVertices],
	    topo = -1,
		i;
      
    for (i = 0; i < grafo->qtdVertices; i++) 
	{
        nivelDoVertice[i] = 0;
	    paiDoVertice[i] = -1;
		visitados[i] = 0;
    }
    
    // Empilha o vertice inicial
    pilha[++topo] = verticeInicial;
	
    while (topo >= 0) 
	{
        int verticeAtual = pilha[topo--]; // Desempilha

        if (visitados[verticeAtual] == 0) 
		{
            printf("%d ->", verticeAtual);
            fprintf(arq,"VERTICE: %d. PAI: %d. NIVEL: %d\n", verticeAtual, paiDoVertice[verticeAtual], nivelDoVertice[verticeAtual]);
            visitados[verticeAtual] = 1;

            // Empilha todos os vizinhos nao visitados
            for (i = grafo->qtdVertices - 1; i >= 0; i--) 
			{
                if (grafo->grafo[verticeAtual][i] > 0 && visitados[i] == 0) 
				{
                    pilha[++topo] = i;
                    paiDoVertice[i] = verticeAtual;
                    nivelDoVertice[i] = nivelDoVertice[verticeAtual] + 1;
                }
            }
        }
    }
   printf("\nFUNCAO: dfsMatriz() - Caminho salvo no arquivo 'caminhoPercorridoDFSMatriz'.");
   fclose(arq);
}

int grauDoVertice(GrafoMatrizAdj *grafo, int vertice)
{
	//DESTINADO A MATRIZ DE ADJACENCIA
	//COMENTARIO PARA A EXPLICACAO DESSA FUNCAO-----
	//	Essa funcao tem o objetivo de calcular o grau de um dado vertice em um grafo (no formato de MATRIZ DE ADJACENCIA), o calculo
	//	eh dado pela quantidade de vertices que incidem sobre esse dado vertice.
	//DATA: 14/06/2024 HORARIO: 14:00 AUTOR: MIKAEL PEREIRA GOMES.-----
	
	//MODIFICADO:
	//	Agora essa fun��o lida com pesos entre os vertices
	//DATA: 06/07/2024. HORARIO: 14:25. AUTOR: MIKAEL PEREIRA GOMES.
	if(grafo == NULL)
	{
		printf("\nFUNCAO: grauDoVertice() - nao ha grafo definido. Retornando...");
		return -1;
	}
	
	if(vertice >= grafo->qtdVertices)
	{
		printf("\nFUNCAO: grauDoVertice() - o vertice %d nao existe",vertice);
		return -1;
	}
	
	int i,
		grau = 0;
	
	for(i = 0;i < grafo->qtdVertices;i++)
	{
		if(grafo->grafo[i][vertice] > 0)//se algum vertice i tem peso para o vertice atual, entao aumenta seu grau
			grau += 1;
	}
	
	return grau;
}

int grauMinimo(GrafoMatrizAdj *grafo)
{
	//DESTINADO A MATRIZ DE ADJACENCIA
	//COMENTARIO PARA A EXPLICACAO DESSA FUNCAO-----
	//	Essa funcao tem o objetivo de encontrar o grau minimo de um grafo, o grau minimo � o menor grau entre todos os outros
	//	graus de todos os outros vertices.Inicia se pegando o grau do primeiro indice para garantir que temos um grau existente
	//	para comecar, nao podemos iniciar como 0, pois vai que ocorre de todos tiverem grau acima de 0.
	//DATA: 16/06/2024 HORARIO: 21:26 AUTOR: MIKAEL PEREIRA GOMES.-----
	
	if(grafo == NULL)
	{
		printf("\nFUNCAO: grauMinimo() - nao ha grafo definido. Retornando...");
		return -1;
	}
	
	int grauMinimo = grauDoVertice(grafo,0),
		i;
	
	for (i = 1;i < grafo->qtdVertices;i++) 
    {	
    	int grau = grauDoVertice(grafo,i);
    	if(grau < grauMinimo)
    		grauMinimo = grau;
    }
    
	return grauMinimo;
}

int grauMaximo(GrafoMatrizAdj *grafo)
{
	//DESTINADO A MATRIZ DE ADJACENCIA
	//COMENTARIO PARA A EXPLICACAO DESSA FUNCAO-----
	//	Essa funcao tem o objetivo de encontrar o grau maximo de um grafo, o grau maximo � o maior grau entre todos os outros
	//	graus de todos os outros vertices.
	//DATA: 16/06/2024 HORARIO: 21:47 AUTOR: MIKAEL PEREIRA GOMES.-----
	
	if(grafo == NULL)
	{
		printf("\nFUNCAO: grauMaximo() - nao ha grafo definido. Retornando...");
		return -1;
	}
	
	int grauMaximo = grauDoVertice(grafo,0),
		i;
	
	for (i = 1;i < grafo->qtdVertices;i++) 
    {	
    	int grau = grauDoVertice(grafo,i);
    	if(grau > grauMaximo)
    		grauMaximo = grau;
    }
    
	return grauMaximo;
}

float grauMedio(GrafoMatrizAdj *grafo)
{
	//DESTINADO A MATRIZ DE ADJACENCIA
	//COMENTARIO PARA A EXPLICACAO DESSA FUNCAO----
	//	Tem o objetivo de calcular o grau medio entre os graus de todos os outros vertices. O modo para se calcular eh somando todos os graus
	//	de todos os vertices e dividindo pela quantidade de vertices.
	//DATA: 16/06/2024 DATA: 21:58	AUTOR: MIKAEL PEREIRA GOMES----
    
	if(grafo == NULL )
	{
		printf("\nFUNCAO: grauMedio() - nao ha grafo definido. Retornando...");
		return -1;
	}
	
	if(grafo->qtdVertices == 0 )
	{
		printf("\nFUNCAO: grauMedio() - grafo contem 0 vertices, nao ha como realizar o calculo do grau medio.");
		return -1;
	}
	
	float somaDosGraus = 0,
		i;
	
	for(i = 0;i < grafo->qtdVertices;i++)
		somaDosGraus += grauDoVertice(grafo,i);
	
	return somaDosGraus/grafo->qtdVertices;
}

float medianaDeGrau(GrafoMatrizAdj *grafo)
{
	//COMENTARIO PARA A EXPLICACAO DESSA FUNCAO:
	//	o calculo da mediana dos graus de um grafo eh dado por: se a quantidade de vertices for impar, entao a mediana eh o valor do centro
	//	se for par, a mediana eh a soma dos dois graus mais proximos do centro / 2. OBS: os graus devem estar listados em ordem.
	//DATA: 18/06/2024 HORARIO: 22:48 AUTOR: MIKAEL PEREIRA GOMES.
	
	int i,
		verticesGrau[grafo->qtdVertices];
		
	for(i = 0;i < grafo->qtdVertices;i++)
		verticesGrau[i] = grauDoVertice(grafo,i);
	
	quicksortGraus(verticesGrau,0,grafo->qtdVertices - 1);
	
	if (grafo->qtdVertices % 2 == 0) 
	{
        int meio1 = grafo->qtdVertices / 2 - 1;
        int meio2 = grafo->qtdVertices / 2;
        return (verticesGrau[meio1] + verticesGrau[meio2]) / 2.0; 
    } 
	else 
	{
        int meio = (grafo->qtdVertices - 1) / 2;
        return verticesGrau[meio];
    }
}

float distanciaEntreVertices(GrafoMatrizAdj *grafo, int verticeOrigem, int verticeDestino)
{
    float distancia[grafo->qtdVertices]; // armazena a distancia entre o verticeInicial para i
	int determinado[grafo->qtdVertices],
		count,
		v,
		i; 
    
    for (i = 0; i < grafo->qtdVertices; i++)//inicializa todas as distancias com INFINITO e os determinados como 0.
    {
    	distancia[i] = FLT_MAX;
		determinado[i] = 0;
	}
       
    distancia[verticeOrigem] = 0;//inicializa a distancia do vertice inicial como 0

    for (count = 0; count < (grafo->qtdVertices - 1); count++)//garantir que se repita para todos os vertices.
	{
        int u = minDistance(distancia, determinado, grafo->qtdVertices);// Pick the minimum distance vertex from the set of vertices not yet processed. u is always equal to src in the first iteration.
		
		if(u == -1)
			break;
			
        determinado[u] = 1;//marca o vertice como determinado 

        for (v = 0; v < grafo->qtdVertices; v++)
            // Atualiza as distancias se os vertices nao tiverem sido determinados, se ha uma aresta
			// e o peso total do caminho do verticeInicial para V atraves de U � o menor que o valor atual de distancia[V] 
            if ((determinado[v] == 0 ) && (grafo->grafo[u][v] > 0) && (distancia[u] != FLT_MAX) && (distancia[u] + grafo->grafo[u][v] < distancia[v]))
                distancia[v] = distancia[u] + grafo->grafo[u][v];
    }
	
    return distancia[verticeDestino];
}

float diametroGrafoMatriz(GrafoMatrizAdj *grafo)
{
	//COMENTARIO PARA A EXPLICACAO DESSA FUNCAO----
	//	Tem o objetivo de encontrar o maior caminho gerado partindo de cada vertice do grafo.
	//DATA: 16/06/2024 HORARIO: 16:57 AUTOR: MIKAEL PEREIRA GOMES----
	
	if(grafo == NULL)
	{
		printf("\nFUNCAO: diametroGrafoMatriz() - nao ha grafo definido. Retornando...");
		return -1;
	}
	
	int maiorDistancia = 0,
		inicio = grafo->qtdVertices /2,
		i,j;
	
	printf("\n ENCONTRANDO DIAMETRO APROXIMADO DO GRAFO: ");
	for(i = inicio;i < grafo->qtdVertices;i++)
	{
		Heap *heap = caminhoMinimoParaTodos(grafo,i);//encontra uma heap apartir de um vertice
		for(j = i; j < grafo->qtdVertices; j++)
		{
			if((heap[j].distancia > maiorDistancia) && (heap[j].distancia != FLT_MAX))//procura a maior distancia
			{
				maiorDistancia = heap[i].distancia;
			}
		}
		printf("\n --VERIFICANDO %d de %d ainda kk...",i, grafo->qtdVertices);
		
		free(heap);//desaloca a heap
	}
	
	return maiorDistancia;
}

int contarArestasMatriz(GrafoMatrizAdj *grafo)
{
	//COMENTARIO PARA A EXPLICACAO DESSA FUNCAO----
	//	Tem o objetivo de calcular a quantidade de arestas de um grafo NAO-DIRECIONADO em formato de matriz de adjacencia.
	//	Uma matriz de adjacencia de um grafo NAO-DIRECIONADO eh uma matriz simetrica, em que cada aresta eh contada duas vezes
	//	para otimizar um pouco, percebi que so precisava iterar sobre a diagonal principal e a parte inferior da matriz(podia ser a superior tbm)
	//DATA: 17/06/2024 HORARIO: 22:40 AUTOR: MIKAEL PEREIRA GOMES
	
	//MODIFICADO:
	//	agora essa funcao lida com pesos.
	//DATA: 05/07/2024. HOR�RIO: 21:28. AUTOR: MIKAEL PEREIRA GOMES.
	
	if(grafo == NULL)
	{
		printf("\nFUNCAO: contarArestasMatriz() - nao ha grafo definido. Retornando...");
		return -1;
	}
	
	int i,
		j,
		arestas = 0;
	
	for(i = 0;i < grafo->qtdVertices;i++)
	{
		for(j = 0;j <= i;j++)// j <= i faz com que ele execute ate atingir um elemento da diagonal principal
		{
			if( grafo->grafo[i][j] > 0)// >0 indica que ha peso
				arestas++;
		}
	}
	
	return arestas;
}

void mostrarComponentesConexas(ComponenteConexa listaConexos[], int qtdComponentes)
{	
	//COMENTARIO PARA A EXPLICACAO DESSA FUNCAO----
	//	Essa funcao tem o objetivo de ser mostrar toda a lista de componentes conexas encontradas, junto com seus vertices.
	//DATA:15/06/2024. HORARIO: 14:48 AUTOR: MIKAEL PEREIRA GOMES----
	
	int i;
	No *nodoAtual,
	   *nodoProx;
	
	for(i = 0;i < qtdComponentes;i++)
	{
		nodoAtual = listaConexos[i].cabeca;//recebe a cabeca de cada lista
		printf("\n---COMPONENTE CONEXA. QTD DE VERTICES: %d\n",listaConexos[i].qtdVertices);
		while(nodoAtual != NULL)//percorrendo os nodos/vertices de uma componente conexa.
		{
			nodoProx = nodoAtual->prox;
			printf("\n 	VERTICE: %d NIVEL: %d. PAI: %d",nodoAtual->vertice, nodoAtual->nivel, nodoAtual->verticePai);
			nodoAtual = nodoProx;
		}	
	}
}

void informacoesGrafoMatrizToArq(GrafoMatrizAdj *grafo)
{
	if(grafo == NULL)
	{
		printf("\nFUNCAO: informacoesGrafoMatrizToArq() - nao ha grafo definido. Retornando...");
		return;
	}
	
	FILE *arq = fopen("informacoesDoGrafoMatriz.txt","w");
	if(arq == NULL)
	{
		printf("\nFUNCAO: informacoesGrafoMatrizToArq() - nao foi possivel abrir o arquivo.");
		return;
	}
	
	int grauMin = grauMinimo(grafo),
		grauMax = grauMaximo(grafo),
		diametro = diametroGrafoMatriz(grafo);
	float grauMed = grauMedio(grafo),
		  mediana = medianaDeGrau(grafo);
		
		
	//mostra ao usuario
	printf("INFORMACOES DO GRAFO DE MATRIZ DE ADJACENCIA: \n");
	printf("QUANTIDADE DE VERTICES: %d\n", grafo->qtdVertices);
	printf("QUANTIDADE DE ARESTAS: %d\n", grafo->qtdArestas);
	printf("GRAU MINIMO %d\n", grauMin);
	printf("GRAU MAXIMO %d\n", grauMax);
	printf("GRAU MEDIO %f\n", grauMed);
	printf("MEDIANA DE GRAU %f\n", mediana);
	printf("DIAMETRO DO GRAFO %f\n", diametro);
	
	//salva no arquivo
	fprintf(arq,"INFORMACOES DO GRAFO DE MATRIZ DE ADJACENCIA: \n");
	fprintf(arq,"QUANTIDADE DE VERTICES: %d\n", grafo->qtdVertices);
	fprintf(arq,"QUANTIDADE DE ARESTAS: %d\n", grafo->qtdArestas);
	fprintf(arq,"GRAU MINIMO %d\n", grauMin);
	fprintf(arq,"GRAU MAXIMO %d\n", grauMax);
	fprintf(arq,"GRAU MEDIO %f\n", grauMed);
	fprintf(arq,"MEDIANA DE GRAU %f\n", mediana);
	fprintf(arq,"DIAMETRO DO GRAFO %f\n", diametro);
	
	fclose(arq);
}

void componentesConexasToArq(ComponenteConexa listaConexos[], int qtdComponentes)
{
	//COMENTARIO PARA A EXPLICACAO DESSA FUNCAO----
	//	Essa funcao tem o objetivo de salvar no arquivo a lista de componentes conexas encontradas, junto com seus vertices.
	//DATA:16/06/2024. HORARIO: 15:49 AUTOR: MIKAEL PEREIRA GOMES----
	
	//MODIFICADO----
	//	Agora ele abre um arquivo diferente, agora abre um arquivo que nao mostra apenas as componentes conexas, mas tambem outras 
	//	informacoes do grafo.
	//DATA: 17/06/2024.	HORARIO: 21:27 AUTOR: MIKAEL PEREIRA GOMES----
	
	FILE *arq = fopen("informacoesDoGrafoMatriz.txt","a");
	if(arq == NULL)
	{
		printf("\nFUNCAO: componentesConexasToArq() - nao foi possivel listar as componentes conexas no arquivo.");
		return;
	}
	
	int i;
	No *nodoAtual,
		 *nodoProx;
	
	for(i = 0;i < qtdComponentes;i++)
	{
		nodoAtual = listaConexos[i].cabeca;//recebe a cabeca de cada lista
		fprintf( arq, "\n---COMPONENTE CONEXA. QTD DE VERTICES: %d\n", listaConexos[i].qtdVertices);
		while(nodoAtual != NULL)//percorrendo os nodos/vertices de uma componente conexa.
		{
			nodoProx = nodoAtual->prox;
			fprintf(arq, "\n 	VERTICE: %d NIVEL: %d. PAI: %d",nodoAtual->vertice, nodoAtual->nivel, nodoAtual->verticePai);
			nodoAtual = nodoProx;
		}	
	}
	printf("\nComponentes conexas escrito no arquivo.");
	fclose(arq);
}

void listarComponentesConexas(GrafoMatrizAdj *grafo)
{
	//COMENTARIO PARA A EXPLICACAO DESSA FUNCAO----
	//	Essa funcao tem o objetivo de ser o ponto inicial para organizar as componentes conexas, realizando chamadas ao 
	//	encontrarComponente() para encontrar cada uma delas, adiciona-las a um vetor que eh realocado em addComponenteConexa()
	//	e apos coletar todas, chama o algoritimo quicksort() para organizar elas em ordem decrescente a partir da quantidade
	//	de vertices de cada uma e salvar elas no arquivo e liberar a memoria alocada para elas.
	//DATA:15/06/2024. HORARIO: 14:46. AUTOR: MIKAEL PEREIRA GOMES----
	
	if(grafo == NULL)
	{
		printf("\nFUNCAO: listarComponentesConexas() - nao ha grafo definido. Retornando...");
		return;
	}
	
	int i,
		visitados[grafo->qtdVertices];//vetor de vertices visitados
	ComponenteConexa *listaConexos = criarListaConexos();//inicializa o vetor de componentes conexas
		
	for(i = 0;i < grafo->qtdVertices;i++)//inicializa todos os vertices como nao visitados;
		visitados[i] = 0;
	
	for(i = 0;i < grafo->qtdVertices;i++)//inicia uma passagem para verificar quais vertices nao foram visitados
	{
		if(visitados[i] == 0)//se o vertice i nao foi visitado
		{	
			ComponenteConexa *novaComponente = encontrarComponente(grafo, visitados, i);//encontra uma nova componente a partir dele
			addComponenteConexa(&listaConexos, *novaComponente, grafo->qtdComponentesConexas);//adicionar uma nova componente no vetor
		}
	}
	
	quicksortComponentes(listaConexos, 0, grafo->qtdComponentesConexas - 1);
	mostrarComponentesConexas(listaConexos, grafo->qtdComponentesConexas);
	componentesConexasToArq(listaConexos,grafo->qtdComponentesConexas);
	freeComponentesConexas(&listaConexos, grafo->qtdComponentesConexas);//libera memoria alocada no vetor, junto com os nodos/vertices de cada componente
}

void freeGrafoLista(GrafoListaAdj **grafo) //libera memoria alcoada para o grafo de lista
{
    int i;
    for(i = 0; i<(*grafo)->qtdVertices; i++){ // loop sobre todos os vertices do grafo
        No *atual = (*grafo)->listaAdj[i]; //aqui inicializa o ponteiro atual p/ percorrer a lista de adj do vertice i

        while(atual != NULL){ //enquanto river nos no vertice lista de adj
            No *temp = atual; // salva o no atual em temp
            atual = atual->prox; // proximo no
            free(temp); //libera temp
        }

    }
    free((*grafo)->listaAdj); //libera vetor de listas de ajs
    free(*grafo); //libera grafo
    *grafo = NULL;
    printf("Liberou grafo Lista Adj");
}

void criarGrafoLista(GrafoListaAdj **grafo, int qtdVertices) //cria grafo usando lista adjs a partir de um valor de vertices
{
    GrafoListaAdj *tempGrafo = (GrafoListaAdj *) malloc(sizeof(GrafoListaAdj)); // alcoa memoria para estrutura do grafo
    if(tempGrafo == NULL)
    {
        printf("\nFUNCAO: criarGrafoLista() - Nao foi possivel criar o grafo temporario.");
        return;
    }
	
	tempGrafo->qtdArestas = 0;
	tempGrafo->qtdComponentesConexas = 0;
    tempGrafo->qtdVertices = qtdVertices; //define a quantidade de vertices no grafo temporario
    tempGrafo->listaAdj=(No**)malloc(sizeof(No*) * qtdVertices); //aloca memoria para o vetor de listas de adj
	
    if(tempGrafo->listaAdj == NULL)
    {
        printf("\n FUNCAO: criarGrafoLista() - nao foi possivel alocar vetir de listas de adj");
        free(tempGrafo);
        return;
    }

    int i;
    for(i = 0; i < qtdVertices; i++)
    {
        tempGrafo->listaAdj[i] = NULL; //inicia todas as lista adj vazia
    }
    
    *grafo = tempGrafo; // atribui ao grafo parametro o grafo temporario
    printf("\nFoi criado com sucesso lista de adj");

}

int grauDoVerticeLista(GrafoListaAdj *grafo, int vertice)
{
	int grau = 0;
	
    No *atual = grafo->listaAdj[vertice]; //inicializa o ponteiro atual 
    while(atual != NULL) // enquanto houver nos
    {
    	grau++;
        atual = atual->prox; //avaca para o proximo no da lista
    }
	
	return grau;
}

int grauMinimoLista(GrafoListaAdj *grafo)
{
	int grauMinimo = grauDoVerticeLista(grafo,0),
		i;
	
	for (i = 1;i < grafo->qtdVertices;i++) 
    {	
    	int grau = grauDoVerticeLista(grafo,i);
    	if(grau < grauMinimo)
    		grauMinimo = grau;
    }
    
	return grauMinimo;
}

int grauMaximoLista(GrafoListaAdj *grafo)
{
	int grauMaximo = grauDoVerticeLista(grafo,0),
		i;
	
	for (i = 1;i < grafo->qtdVertices;i++) 
    {	
    	int grau = grauDoVerticeLista(grafo,i);
    	if(grau > grauMaximo)
    		grauMaximo = grau;
    }
    
	return grauMaximo;
}

float grauMedioLista(GrafoListaAdj *grafo)
{
	float somaDosGraus = 0,
		i;
	
	for(i = 0;i < grafo->qtdVertices;i++)
		somaDosGraus += grauDoVerticeLista(grafo,i);
	
	return somaDosGraus/grafo->qtdVertices;
}

float medianaDeGrauLista(GrafoListaAdj *grafo)
{
	int i, verticesGrau[grafo->qtdVertices];
		
	for(i = 0;i < grafo->qtdVertices;i++)
		verticesGrau[i] = grauDoVerticeLista(grafo,i);
	
	quicksortGraus(verticesGrau,0,grafo->qtdVertices - 1);
	
	if (grafo->qtdVertices % 2 == 0) 
	{
        int meio1 = grafo->qtdVertices / 2 - 1;
        int meio2 = grafo->qtdVertices / 2;
        return (verticesGrau[meio1] + verticesGrau[meio2]) / 2.0; 
    } 
	else 
	{
        int meio = (grafo->qtdVertices - 1) / 2;
        return verticesGrau[meio];
    }
}

void dijkstraListaComHeap(GrafoListaAdj *grafo, int verticeInicial)
{
    float distancia[grafo->qtdVertices]; 
	int posicaoNaHeap[grafo->qtdVertices],
	    determinado[grafo->qtdVertices],
		tamanhoDaHeap = 0,
		i; 
	Heap heap[grafo->qtdVertices],
		  novoElemento;
		  
    for (i = 0; i < grafo->qtdVertices; i++)
    {
    	distancia[i] = FLT_MAX;
		determinado[i] = 0;
		posicaoNaHeap[i] = -1;
	}
	
    distancia[verticeInicial] = 0;
    
    for (i = 0; i < grafo->qtdVertices; i++)
    {
    	heap[i].distancia = distancia[i];
    	heap[i].vertice = i;
	}
	
    novoElemento.distancia = distancia[verticeInicial];
    novoElemento.vertice = verticeInicial;
    adicionaNaHeap(heap,posicaoNaHeap,&tamanhoDaHeap, novoElemento);
    
	while(tamanhoDaHeap > 0)
	{
		int vizinho,
			min = removeDaHeap(heap,posicaoNaHeap, &tamanhoDaHeap);  //pega o vertice nao determinado com a menor distancia
			
		if(min == -1)
			break;
			
        determinado[min] = 1;//marca o vertice como determinado 
		
		No* verticeAtual = grafo->listaAdj[min];
        while (verticeAtual != NULL) 
		{
            int vizinho = verticeAtual->vertice;
            float peso = verticeAtual->peso;
            
            if((determinado[vizinho] == 0) && (distancia[min] != FLT_MAX) && (distancia[min] + peso < distancia[vizinho])) 
			{
                distancia[vizinho] = distancia[min] + peso;
                novoElemento.distancia = distancia[vizinho];
            	novoElemento.vertice = vizinho;
            	adicionaNaHeap(heap, posicaoNaHeap, &tamanhoDaHeap, novoElemento);
            }
            verticeAtual = verticeAtual->prox;
        }
	}
}

Heap *caminhoMinimoParaTodoslista(GrafoListaAdj *grafo, int verticeInicial)
{
    float distancia[grafo->qtdVertices]; 
	int posicaoNaHeap[grafo->qtdVertices],
	    determinado[grafo->qtdVertices],
		tamanhoDaHeap = 0,
		i; 
	Heap *heap = (Heap*)malloc(sizeof(Heap) * grafo->qtdVertices),
		  novoElemento;
		  
    for (i = 0; i < grafo->qtdVertices; i++)
    {
    	distancia[i] = FLT_MAX;
		determinado[i] = 0;
		posicaoNaHeap[i] = -1;
	}
	
    distancia[verticeInicial] = 0;
    
    for (i = 0; i < grafo->qtdVertices; i++)
    {
    	heap[i].distancia = distancia[i];
    	heap[i].vertice = i;
	}
	
	for(i = 0;i < grafo->qtdVertices;i++)
	{
		int vizinho,
			min = minDistance(distancia, determinado, grafo->qtdVertices);  //pega o vertice nao determinado com a menor distancia
			
		if(min == -1)
			break;
			
        determinado[min] = 1;//marca o vertice como determinado 
		
		No* verticeAtual = grafo->listaAdj[min];
        while (verticeAtual != NULL) 
		{
            int vizinho = verticeAtual->vertice;
            float peso = verticeAtual->peso;
            
            if((determinado[vizinho] == 0) && (distancia[min] != FLT_MAX) && (distancia[min] + peso < distancia[vizinho])) 
			{
                distancia[vizinho] = distancia[min] + peso;
            }
            verticeAtual = verticeAtual->prox;
        }
	}
		
	
	for(i=0;i<grafo->qtdVertices;i++)
    {
    	novoElemento.distancia = distancia[i];
    	novoElemento.vertice = i;
    	adicionaNaHeap(heap, posicaoNaHeap, &tamanhoDaHeap, novoElemento);
	}
	
	return heap;
}

void dijkstraListaSemHeap(GrafoListaAdj* grafo, int verticeInicial)
{
    int determinado[grafo->qtdVertices],
		count,
		i;
    float distancia[grafo->qtdVertices];
   
    for (i = 0; i < grafo->qtdVertices; i++) 
	{
        distancia[i] = FLT_MAX;
        determinado[i] = 0;
    }
    
    distancia[verticeInicial] = 0;

    for (count = 0; count < (grafo->qtdVertices - 1); count++) 
	{
        int u = minDistance(distancia, determinado, grafo->qtdVertices);
        
        if (u == -1) 
			break;
			
        determinado[u] = 1;

        No* verticeAtual = grafo->listaAdj[u];
        
        while (verticeAtual != NULL) 
		{
            int v = verticeAtual->vertice;
            float peso = verticeAtual->peso;
            
            if((determinado[v] == 0) && (distancia[u] != FLT_MAX) && (distancia[u] + peso < distancia[v])) 
			{
                distancia[v] = distancia[u] + peso;
            }
            verticeAtual = verticeAtual->prox;
        }
    }
}

float diametroGrafoLista(GrafoListaAdj *grafo)
{
	if(grafo == NULL)
	{
		printf("\nFUNCAO: diametroGrafoLista() - nao ha grafo definido. Retornando...");
		return -1;
	}
	
	int maiorDistancia = 0,
		inicio = grafo->qtdVertices /2,
		i,j;
	
	printf("\n ENCONTRANDO DIAMETRO APROXIMADO DO GRAFO: ");
	for(i = inicio;i < grafo->qtdVertices;i++)
	{
		Heap *heap = caminhoMinimoParaTodoslista(grafo,i);//encontra uma heap apartir de um vertice
		for(j = i; j < grafo->qtdVertices; j++)
		{
			if((heap[j].distancia > maiorDistancia) && (heap[j].distancia != FLT_MAX))//procura a maior distancia
			{
				maiorDistancia = heap[i].distancia;
			}
		}
		printf("\n --VERIFICANDO %d de %d ainda kk...",i, grafo->qtdVertices);
		
		free(heap);//desaloca a heap
	}
	
	return maiorDistancia;
}

int contarArestasLista(GrafoListaAdj *grafo)
{
	int arestas = 0,i;
	
    for(i = 0; i < grafo->qtdVertices; i++) // passa por todos os vertices do grafo
    {
        No *atual = grafo->listaAdj[i]; //inicializa o ponteiro atual 
        while(atual != NULL) // enquanto houver nos
        {
        	arestas++;
            atual = atual->prox; //avaca para o proximo no da lista
        }
    }
    
	return arestas/2;
}

void lerArquivoDeGrafoLista(GrafoListaAdj **grafo) //ler de um .txt e carrega na estrutura de lista adj
{
    FILE *arquivo = fopen("grafo_W_1.txt","r"); //abre arquivo para leitura
    if(arquivo == NULL)
    {
        printf("\nFUNCAO: lerArquivoDeGrafoLista() - Nao foi possivel abrir o arquivo");
        return;
    }

    int qtdVertices = 0,
        linha = 2, // come�a a ler a partir da linha 2 (considerando a primeira linha como quantidade de v�rtices)
        i, j;
    float peso;
    
    fscanf(arquivo, "%d\n", &qtdVertices); // le a quantidade de vertices do arquivo

    *grafo = NULL;

    criarGrafoLista(grafo, qtdVertices); //chama funcao criaGrafoLista para criar a estrutura com "qtdVertices"
    if(*grafo == NULL)
    {
        printf("\nFUNCAO: lerArquivoDeGrafoLista() - Como nao foi possivel criar um grafo, o arquivo nao sera lido.");
        return;
    }

    while(fscanf(arquivo, "%d %d %f\n", &i, &j, &peso) != EOF)//le cada linha subsequente do arquivo, onde i e j sao vertices que formam uma aresta
    {
        if(i >= qtdVertices || j >= qtdVertices)
        {
            //printf("\nFUNCAO: lerArquivoDeGrafoLista() - Um vertice extrapola o valor maximo de indice para a lista de adjacencia. LINHA %d do arquivo ", linha);
            //freeGrafoLista(grafo);
            //return; 
            i--;
            j--;//so pra nao ficar ajustando manualmente
        }

        // Criando aresta nao direcionada
        No *novoNo1 = (No *)malloc(sizeof(No)); //aloca memoria para um novo no na lista de adj
        novoNo1->vertice = j;
        novoNo1->peso = peso;
        novoNo1->prox = (*grafo)->listaAdj[i];
        (*grafo)->listaAdj[i] = novoNo1;

        No *novoNo2 = (No*) malloc(sizeof(No));
        novoNo2->vertice = i;
        novoNo2->peso = peso;
        novoNo2->prox = (*grafo)->listaAdj[j];
        (*grafo)->listaAdj[j] = novoNo2;
		
        linha++;
    }
    
    (*grafo)->qtdArestas = contarArestasLista(*grafo);
    printf("\nArquivo Lido\n");
    
    fclose(arquivo);
}
void mostrarGrafoLista(GrafoListaAdj **grafo) // mostra em tela representacao de grafo lista adj
{
    int i;
    
    for(i = 0; i < (*grafo)->qtdVertices; i++) // passa por todos os vertices do grafo
    {
        No *atual = (*grafo)->listaAdj[i]; //inicializa o ponteiro atual 
       
        while(atual != NULL) // enquanto houver nos
        {	
            printf("%d/PESO: %f -> ", atual->vertice, atual->peso); // imprime vertice ajdacente
            atual = atual->prox; //avaca para o proximo no da lista
        }
        printf("NULL\n");
    }
}

void bfsLista(GrafoListaAdj grafo, int verticeInicial) //busca em largura a partir de um vertice inicial
{
	FILE *arq = fopen("caminhoPercorridoBFSlista.txt","w");

    int visitados[grafo.qtdVertices], nivelDoVertice[grafo.qtdVertices]; //alcoa memoria
    int fila[grafo.qtdVertices], paiDoVertice[grafo.qtdVertices]; // aloca memoria pra fila
    int frente = 0, tras = 0;
    int i;
	
    for(i = 0; i < grafo.qtdVertices; i++) //inicializa vertices como nao visitados e a fila com -1
    {
    	nivelDoVertice[i] = 0;//inicialmente nao conhecemos o nivel de cada vertice.
	    paiDoVertice[i] = -1;//inicialmente nao conhecemos o pai de cada vertice.
        visitados[i] = 0;
        fila[i] = -1;
    }

    visitados[verticeInicial] = 1;
    fila[tras++] = verticeInicial;

    while(frente != tras) //enquanto fila nao tiver vazia
    {
        int verticeAtual = fila[frente++]; //remove o vertice do inicio da fila
        
        printf("%d ->", verticeAtual);
        fprintf(arq,"VERTICE: %d. PAI: %d. NIVEL %d\n", verticeAtual, paiDoVertice[verticeAtual], nivelDoVertice[verticeAtual]);
		
        No *atual = grafo.listaAdj[verticeAtual]; //inicializa atual pra percorrer lista de adj do vertice atual
        while(atual != NULL)
        {
            if(visitados[atual->vertice] == 0)
            {
                fila[tras++] = atual->vertice;
                visitados[atual->vertice] = 1;
                paiDoVertice[atual->vertice] = verticeAtual;
                nivelDoVertice[atual->vertice] = nivelDoVertice[verticeAtual] + 1;
            }
            atual = atual->prox;
        }
    }
    
    printf("\nFUNCAO: bfsLista() - caminho salvo no arquivo 'caminhoPercorridoBFSLista'.");
    fclose(arq);
}

float distanciaEntreVerticesLista(GrafoListaAdj* grafo, int verticeOrigem, int verticeDestino)
{
	
    int determinado[grafo->qtdVertices],
		count,
		i;
    float distancia[grafo->qtdVertices];
    for (i = 0; i < grafo->qtdVertices; i++) 
	{
        distancia[i] = FLT_MAX;
        determinado[i] = 0;
    }
   
    distancia[verticeOrigem] = 0;
	
	
    for (count = 0; count < (grafo->qtdVertices - 1); count++) 
	{
        int u = minDistance(distancia, determinado, grafo->qtdVertices);
        
        if (u == -1) 
			break;
			
        determinado[u] = 1;

        No* verticeAtual = grafo->listaAdj[u];
        
        while (verticeAtual != NULL) 
		{
            int v = verticeAtual->vertice;
            float peso = verticeAtual->peso;
            
            if((determinado[v] == 0) && (distancia[u] != FLT_MAX) && (distancia[u] + peso < distancia[v])) 
			{
                distancia[v] = distancia[u] + peso;
            }
            verticeAtual = verticeAtual->prox;
        }
    }
    
    return distancia[verticeDestino];
}

void dfsLista(GrafoListaAdj *grafo, int verticeInicial) 
{
    if (grafo == NULL) 
    {
        printf("FUNCAO: dfsLista() - Grafo nao definido\n");
        return;
    }
    if (verticeInicial < 0 || verticeInicial >= grafo->qtdVertices) 
    {
        printf("FUNCAO: dfsLista() - vertice inicial invalido.\n");
        return;
    }

    FILE *arq = fopen("caminhoPercorridoDFSListaAdj.txt", "w");
    int nivelDoVertice[grafo->qtdVertices],
        paiDoVertice[grafo->qtdVertices],
        visitados[grafo->qtdVertices],
        pilha[grafo->qtdVertices],
        topo = -1,
        i;

    for (i = 0; i < grafo->qtdVertices; i++) 
    {
        nivelDoVertice[i] = 0;
        paiDoVertice[i] = -1;
        visitados[i] = 0;
    }

    // Empilha o vertice inicial
    pilha[++topo] = verticeInicial;

    while (topo >= 0) 
    {
        int verticeAtual = pilha[topo--]; // Desempilha

        if (visitados[verticeAtual] == 0) 
        {
            printf("%d ->", verticeAtual);
            fprintf(arq, "VERTICE: %d. PAI: %d. NIVEL: %d\n", verticeAtual, paiDoVertice[verticeAtual], nivelDoVertice[verticeAtual]);
            visitados[verticeAtual] = 1;

            // Empilha todos os vizinhos nao visitados
            No *adjacente = grafo->listaAdj[verticeAtual];
            while (adjacente != NULL) 
            {
                int verticeAdj = adjacente->vertice;
                if (visitados[verticeAdj] == 0) 
                {
                    pilha[++topo] = verticeAdj;
                    paiDoVertice[verticeAdj] = verticeAtual;
                    nivelDoVertice[verticeAdj] = nivelDoVertice[verticeAtual] + 1;
                }
                adjacente = adjacente->prox;
            }
        }
    }
    printf("\nFUNCAO: dfsListaAdj() - Caminho salvo no arquivo 'caminhoPercorridoDFSListaAdj'.");
    
    fclose(arq);
}

ComponenteConexa* encontrarComponenteLista(GrafoListaAdj *grafo, int visitados[], int verticeInicial) //busca em largura a partir de um vertice inicial
{
    int fila[grafo->qtdVertices];// aloca memoria pra fila
    int frente = 0, tras = 0;
    int i, nivelDoVertice[grafo->qtdVertices];

    for(i = 0; i < grafo->qtdVertices; i++) //inicializa vertices como nao visitados e a fila com -1
    {
    	nivelDoVertice[i] = 0;
        fila[i] = -1;
    }

    visitados[verticeInicial] = 1;
    fila[tras++] = verticeInicial;
    
    ComponenteConexa *newConexos = criarListaConexos();//cria a lista da componente conexa
	if(newConexos == NULL)//impedir que a funcao continue caso ocorra erros na criacao
		return NULL;

	addVisitado(newConexos, verticeInicial, -1, 0); //adiciona o verticeInicial na lista
	
    while(frente != tras) //enquanto fila nao tiver vazia
    {
        int verticeAtual = fila[frente++]; //remove o vertice do inicio da fila

        No *atual = grafo->listaAdj[verticeAtual]; //inicializa atual pra percorrer lista de adj do vertice atual
        while(atual != NULL)
        {
            if(visitados[atual->vertice] == 0)
            {
                fila[tras++] = atual->vertice;
                visitados[atual->vertice] = 1;
                nivelDoVertice[atual->vertice] =  nivelDoVertice[verticeAtual] + 1;
              	addVisitado(newConexos, atual->vertice, verticeAtual, nivelDoVertice[atual->vertice]); //adiciona o vertice enfileirado na lista da Componente conexa.
            }
            atual = atual->prox;
        }
    }
	
	grafo->qtdComponentesConexas++;
    return newConexos;
}

void componentesConexasListaToArq(ComponenteConexa listaConexos[], int qtdComponentes)
{
	FILE *arq = fopen("informacoesGrafoLista.txt","a");
	if(arq == NULL)
	{
		printf("\nFUNCAO: componentesConexasListaToArq() - nao foi possivel listar as componentes conexas no arquivo.");
		return;
	}
	
	int i;
	No *nodoAtual,
		 *nodoProx;
	
	for(i = 0;i < qtdComponentes;i++)
	{
		nodoAtual = listaConexos[i].cabeca;//recebe a cabeca de cada lista
		fprintf( arq, "\n---COMPONENTE CONEXA. QTD DE VERTICES: %d\n", listaConexos[i].qtdVertices);
		while(nodoAtual != NULL)//percorrendo os nodos/vertices de uma componente conexa.
		{
			nodoProx = nodoAtual->prox;
			fprintf(arq, "\n 	VERTICE: %d NIVEL: %d. PAI: %d",nodoAtual->vertice, nodoAtual->nivel, nodoAtual->verticePai);
			nodoAtual = nodoProx;
		}	
	}
	printf("\nComponentes conexas escrito no arquivo.");
	fclose(arq);
}

void listarComponentesConexasLista(GrafoListaAdj *grafo)
{
	//COMENTARIO PARA A EXPLICACAO DESSA FUNCAO----
	//	Essa funcao tem o objetivo de ser o ponto inicial para organizar as componentes conexas, realizando chamadas ao 
	//	bfsVisitados() para encontrar cada uma delas, adiciona-las a um vetor que eh realocado em addComponenteConexa()
	//	e apos coletar todas, chama o algoritimo quicksort() para organizar elas em ordem decrescente a partir da quantidade
	//	de vertices de cada uma e salvar elas no arquivo e liberar a memoria alocada para elas.
	//DATA:15/06/2024. HORARIO: 14:46. AUTOR: MIKAEL PEREIRA GOMES----
	
	if(grafo == NULL)
	{
		printf("\nFUNCAO: listarComponentesConexasLista() - nao ha grafo definido. Retornando...");
		return;
	}
	
	int i,
		visitados[grafo->qtdVertices];//vetor de vertices visitados
	ComponenteConexa *listaConexos = criarListaConexos();//inicializa o vetor de componentes conexas
		
	for(i = 0;i < grafo->qtdVertices;i++)//inicializa todos os vertices como nao visitados;
		visitados[i] = 0;
	
	for(i = 0;i < grafo->qtdVertices;i++)//inicia uma passagem para verificar quais vertices nao foram visitados
	{
		if(visitados[i] == 0)//se o vertice i nao foi visitado
		{	
			ComponenteConexa *novaComponente = encontrarComponenteLista(grafo, visitados, i);//encontra uma nova componente a partir dele
			addComponenteConexa(&listaConexos, *novaComponente, grafo->qtdComponentesConexas);//adicionar uma nova componente no vetor
		}
	}
	
	quicksortComponentes(listaConexos, 0, grafo->qtdComponentesConexas - 1);
	mostrarComponentesConexas(listaConexos, grafo->qtdComponentesConexas);
	componentesConexasListaToArq(listaConexos, grafo->qtdComponentesConexas);
	freeComponentesConexas(&listaConexos, grafo->qtdComponentesConexas);//libera memoria alocada no vetor, junto com os nodos/vertices de cada componente
}

void informacoesGrafoListaToArq(GrafoListaAdj *grafo)
{
	if(grafo == NULL)
	{
		printf("\nFUNCAO: informacoesGrafoListaToArq() - nao ha grafo definido. Retornando...");
		return;
	}
	
	FILE *arq = fopen("informacoesGrafoLista.txt","w");
	if(arq == NULL)
	{
		printf("\nFUNCAO: informacoesGrafoListaToArq() - nao foi possivel abrir o arquivo.");
		return;
	}
	
	int grauMin = grauMinimoLista(grafo),
		grauMax = grauMaximoLista(grafo),
		diametro = diametroGrafoLista(grafo);
	float grauMed = grauMedioLista(grafo),
		  mediana = medianaDeGrauLista(grafo);
		
		
	//mostra ao usuario
	printf("INFORMACOES DO GRAFO DE MATRIZ DE ADJACENCIA: \n");
	printf("QUANTIDADE DE VERTICES: %d\n", grafo->qtdVertices);
	printf("QUANTIDADE DE ARESTAS: %d\n", grafo->qtdArestas);
	printf("GRAU MINIMO %d\n", grauMin);
	printf("GRAU MAXIMO %d\n", grauMax);
	printf("GRAU MEDIO %f\n", grauMed);
	printf("MEDIANA DE GRAU %f\n", mediana);
	printf("DIAMETRO DO GRAFO %f\n", diametro);
	
	//salva no arquivo
	fprintf(arq,"INFORMACOES DO GRAFO DE MATRIZ DE ADJACENCIA: \n");
	fprintf(arq,"QUANTIDADE DE VERTICES: %d\n", grafo->qtdVertices);
	fprintf(arq,"QUANTIDADE DE ARESTAS: %d\n", grafo->qtdArestas);
	fprintf(arq,"GRAU MINIMO %d\n", grauMin);
	fprintf(arq,"GRAU MAXIMO %d\n", grauMax);
	fprintf(arq,"GRAU MEDIO %f\n", grauMed);
	fprintf(arq,"MEDIANA DE GRAU %f\n", mediana);
	fprintf(arq,"DIAMETRO DO GRAFO %f\n", diametro);

	fclose(arq);
}


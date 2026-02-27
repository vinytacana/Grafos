#  Biblioteca de Grafos (ED2 - Trabalho 1)

Este projeto consiste em uma biblioteca completa para manipulação de grafos em **Linguagem C**, desenvolvida sob o paradigma de **Tipo Abstrato de Dados (TAD)**. O sistema foi projetado para lidar com grafos de grande escala, oferecendo representações eficientes em memória e algoritmos otimizados para análise de redes.

---

## Estrutura de Dados 

A arquitetura do projeto foca em **encapsulamento** e **polimorfismo**. Abaixo, detalhamos as estruturas fundamentais localizadas em `include/Graph.h` e `include/Utils.h`:

### 1. TAD Grafo (`Graph`)
A peça central do projeto. Utiliza uma abordagem de ponteiro genérico para suportar duas representações:
- **Matriz de Adjacência (`GRAPH_MATRIX`):** Implementada como um array bidimensional de `float` (`float**`). 
    - **Vantagem:** Acesso $O(1)$ para verificar se existe aresta entre dois vértices.
    - **Uso ideal:** Grafos densos (onde o número de arestas é próximo a $V^2$).
- **Lista de Adjacência (`GRAPH_LIST`):** Implementada como um array de ponteiros para listas encadeadas de structs `Edge`.
    - **Vantagem:** Economia de memória drástica para grafos esparsos ($O(V+E)$).
    - **Uso ideal:** Redes reais (redes sociais, colaboração), onde a maioria dos vértices tem poucos vizinhos.
- **Atributos de Metadados:** Armazena `num_vertices`, `num_edges`, um booleano `has_negative_weights` (para segurança no Dijkstra) e o mapeamento de nomes `vertex_names`.

### 2. Iterador de Vizinhos (`NeighborIterator`)
Este é um padrão de projeto (Iterator) implementado para que os algoritmos sejam **independentes da representação**.
- Quando você pede os vizinhos de um vértice, o iterador decide internamente se deve percorrer uma linha da matriz ou uma lista encadeada.
- Isso permite que o código da BFS, DFS e Dijkstra seja escrito uma única vez e funcione para ambas as representações.

### 3. Fila Circular (`Queue`)
- Implementada em `Utils.c` com gerenciamento de memória dinâmico.
- Essencial para a **BFS**, garantindo a ordem de visitação por níveis.

### 4. Pilha (`Stack`)
- Implementada para a **DFS iterativa**. 
- Ao contrário da recursão, a pilha em memória heap permite processar grafos com milhões de vértices sem causar *Stack Overflow* do sistema operacional.

### 5. Min-Heap (`MinHeap`)
- Uma árvore binária completa onde o pai é sempre menor que os filhos.
- **Uso no Dijkstra:** Permite extrair o vértice com a menor distância atual em tempo $O(\log V)$. 
- Possui uma função de `update` (decremento de chave) otimizada para atualizar distâncias rapidamente.

---

## Algoritmos

### 1. BFS (Busca em Largura)
- **Objetivo:** Explorar o grafo nível por nível a partir de uma origem.
- **Complexidade:** $O(V + E)$ em lista, $O(V^2)$ em matriz.
- **Saída:** Gera um arquivo com a estrutura da árvore (Vértice, Pai, Nível). Útil para encontrar caminhos mínimos em grafos sem peso.

### 2. DFS (Busca em Profundidade)
- **Objetivo:** Explorar cada ramo o mais longe possível antes de retroceder.
- **Saída:** Arquivo com a ordem de descoberta, níveis e relações de parentesco.

### 3. Componentes Conexas
- Utiliza múltiplas chamadas de BFS para garantir que todos os vértices sejam visitados.
- **Diferencial:** Após encontrar todas as componentes, o algoritmo as ordena por tamanho em ordem **decrescente** (requisito do trabalho).

### 4. Diâmetro do Grafo
- **Exato:** Calcula o caminho mínimo entre todos os pares de vértices ($V \times$ BFS). Extremamente custoso para grafos > 5.000 vértices.
- **Aproximado (2-BFS):** 
    1. Escolhe um vértice aleatório $A$ e encontra o mais distante $B$.
    2. Realiza uma segunda BFS a partir de $B$ para encontrar o vértice mais distante $C$.
    3. A distância entre $B$ e $C$ é a aproximação do diâmetro. 
    - **Vantagem:** Roda em tempo linear $O(V+E)$, sendo ideal para grafos gigantes.

### 5. Caminho Mínimo (Dijkstra)
- **Versão com Vetor:** Varre um array para achar o mínimo ($O(V^2)$).
- **Versão com Heap:** Utiliza a Min-Heap ($O(E \log V)$).
- **Rede de Colaboração:** Utiliza os pesos das arestas (inversamente proporcionais ao número de artigos em coautoria) para encontrar o caminho de menor "distância acadêmica".

---

## Guia de Uso (Passo a Passo)

### 1. Preparação do Ambiente
Certifique-se de que os arquivos de dados estão na pasta `data/` conforme a estrutura:
```text
data/d1/grafo_1.txt
data/d2/rede_colaboracao.txt
data/d2/rede_colaboracao_vertices.txt
```

### 2. Compilação
Abra o terminal na pasta raiz e digite:
```bash
make clean && make
```
*Isso removerá resquícios de compilações antigas e gerará o executável `main`.*

### 3. Execução Passo a Passo (Exemplo Real)

1. **Inicie o programa:**
   ```bash
   ./main
   ```

2. **Carregar o Grafo (Opção 2 - Lista):**
   - Escolha a opção `2`.
   - Digite o caminho: `data/d2/rede_colaboracao.txt`.
   - O programa exibirá o uso de memória (ex: 80 MB).

3. **Carregar Nomes (Opção 10 - Obrigatório para a Parte 2):**
   - Escolha a opção `10`.
   - Digite: `data/d2/rede_colaboracao_vertices.txt`.
   - Agora você pode buscar por nomes em vez de números.

4. **Calcular Distância (Opção 5):**
   - Escolha a opção `5`.
   - Origem: `Edsger W. Dijkstra`
   - Destino: `Alan M. Turing`
   - O programa calculará e exibirá a distância acadêmica.

5. **Gerar Relatório (Opção 6):**
   - Escolha a opção `6`.
   - Escolha `2` para diâmetro aproximado (recomendado para a rede de colaboração).
   - Verifique o arquivo gerado em: `output/informacoes_grafo.txt`.

### 4. Entendendo a Saída (`output/`)
- `saida_bfs.txt`: Mostra a árvore gerada. Ex: `Vértice 10, Pai 5, Nível 2`.
- `informacoes_grafo.txt`: Contém o grau médio, mediana, diâmetro e a lista de todas as componentes conexas ordenadas do maior para o menor.

 ## Estudos de Caso                                                                                          
                                                                                                                  
O programa inclui uma opção de **Benchmark** (Opção 9) que executa 100 buscas aleatórias e calcula o tempo       
médio. Isso é fundamental para responder às perguntas do relatório sobre desempenho de memória e tempo de     
execução entre diferentes representações e algoritmos.                                                           
                                     
---


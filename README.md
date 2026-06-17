# agm-algorithm-comparison-study
Estudo Comparativo de Algoritmos de Árvore Geradora Mínima (AGM): Prim vs Kruskal.
Este repositório contém o desenvolvimento e a análise comparativa dos algoritmos de **Prim** e **Kruskal** para a determinação de Árvores Geradoras Mínimas (Minimum Spanning Trees - MST). 
    O principal objetivo, além da correta implementação teórica, foi avaliar a eficiência de tempo de execução (CPU) e o consumo de memória de ambas as abordagens ao manipular grafos de grande escala.

---

## 🚀 Sobre os Algoritmos

### 1. Algoritmo de Prim
* **Abordagem:** Gulosa, baseada em vértices. Cresce a árvore a partir de um vértice inicial, adicionando sempre a aresta de menor peso que conecta a árvore a um novo vértice.
* **Otimização Utilizada:** Para reduzir drasticamente o tempo de busca da aresta de menor peso, foi utilizada uma estrutura de **Heap Mínimo (Fila de Prioridade)**.
* **Complexidade:** $O((V + E) \log V)$ com Heap Binário.

### 2. Algoritmo de Kruskal
* **Abordagem:** Gulosa, baseada em arestas. Ordena todas as arestas do grafo pelo peso e as adiciona uma a uma, desde que não criem um ciclo.
* **Otimização Utilizada:** Implementado com a estrutura **Union-Find (Disjoint-Set)** para detecção eficiente de ciclos em tempo quase constante, e ordenação das arestas utilizando o algoritmo **Quicksort**.
* **Complexidade:** $O(E \log E)$ devido à etapa de ordenação.

---

## 🛠️ Estruturas de Dados Implementadas

O projeto foi desenvolvido em **C** e faz uso das seguintes estruturas customizadas:
* **Grafo por Lista de Adjacência:** Representação otimizada para grafos esparsos, onde cada vértice possui uma lista encadeada de suas conexões (`ADJACENCIA`).
* **Vetor Linear de Arestas (`ARESTA`):** Utilizado de forma global e contígua na memória para otimizar o acesso do Quicksort e o fluxo de relaxamento/varredura.
* **Heap Mínimo (Binary Heap):** Tabela estática dimensionada para suportar a alta volumetria de dados de prioridade.

---

## 📊 Benchmarking e Coleta de Dados

Para medir o desempenho real dos algoritmos sob estresse, os testes foram submetidos a instâncias massivas de redes de transporte realistas (como os benchmarks de estradas do **DIMACS**(http://www.diag.uniroma1.it/challenge9/download.shtml), ex.:*USA-road-d.CAL.gr* com cerca de 1.9 milhões de vértices e 4.6 milhões de arestas).

### Métricas Monitoradas:
1. **Tempo de CPU:** Medido de forma precisa através da biblioteca `<sys/times.h>` (`times()`), isolando o tempo real gasto pelo processador.
2. **Custo Total da AGM:** Somatório do peso de todas as arestas que compõem a árvore geradora mínima gerada, garantindo a corretude dos dois algoritmos.

Os resultados obtidos de forma automatizada são persistidos em arquivos de texto (`saidaPrim.txt` e `saidaKruskal.txt`) no seguinte formato:
```text
Arquivo: USA-road-d.CAL.gr
Custo total: [VALOR_DO_CUSTO]
Tempo total: X.XXXX segundos

## 💻 Como Executar o Projeto

### Pré-requisitos
* Sistema operacional baseado em Unix (Linux/macOS) para total compatibilidade com a biblioteca `<sys/times.h>`.
* Compilador `gcc`.

### Compilação e Execução
1. Certifique-se de que o arquivo do grafo (ex: `USA-road-d.CAL.gr` ou `teste.gr`) está no mesmo diretório do código fonte.
2. Compile o código via terminal:
   ```bash
   gcc -O3 main.c -o agm_study
   ```
   *(A flag `-O3` habilita otimizações agressivas de compilação, ideal para testar grandes volumes de dados).*

3. Execute o programa:
   ```bash
   ./agm_study
   ```

---

## ✒️ Autor

* **Guilherme da Silva Ferraz** - Estudante de Ciência da Computação.

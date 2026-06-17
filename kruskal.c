#include <stdio.h>
#include <stdlib.h>
#define true 1
#define false 0
#define INT_MAX 2147483647
typedef int bool;
typedef int TIPOPESO;
long long custo_total;

#include <sys/times.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

typedef struct adjacencia{
    int vertice;
    TIPOPESO peso;
    struct adjacencia *prox;
} ADJACENCIA;

typedef struct vertice{
    int grau;
    /* Dados armazenados vao aqui */
    ADJACENCIA *cab;
} VERTICE;

typedef struct grafo {
    int vertices;
    int arestas;
    VERTICE *adj;
} GRAFO;


// Estrutura auxiliar para realizar a ordenação das arestas do grafo
typedef struct{ 
    int u;
    int v;
    int peso;
} ARESTA;

ARESTA *arestas;
/* Criando um grafo */
GRAFO *criarGrafo(int v){
    GRAFO *g = (GRAFO *) malloc(sizeof(GRAFO));
    
    g->vertices     = v;
    g->arestas      = 0;
    g->adj          = (VERTICE *) malloc(v*sizeof(VERTICE));
    int i;
    
    for (i=0; i<v; i++){
        g->adj[i].cab = NULL;
        g->adj[i].grau=0;
    }
    return g;
}

ADJACENCIA *criaAdj(int v,int peso){
    ADJACENCIA *temp = (ADJACENCIA *) malloc(sizeof(ADJACENCIA));
    temp->vertice   = v;
    temp->peso      = peso;
    temp->prox      = NULL;
    return (temp);
}

ADJACENCIA *inseriListAdj(ADJACENCIA *adj, int vf, TIPOPESO p)
{

    ADJACENCIA *ant = adj;
    ADJACENCIA *atual = adj;
    int control = 0;

    ADJACENCIA *novo = criaAdj(vf, p);

    while (atual != NULL && atual->vertice < vf)
    {
        ant = atual;
        atual = atual->prox;
        control = 1;
    }
    if (ant == NULL && control == 0)
    {
        novo->prox = atual;
        
        return novo;
    }
    else if (control == 0)
    {
        novo->prox = atual;
        return novo;
    }else{
        novo->prox = atual;
        ant->prox = novo;
    }

    return adj;
}

void liberaGrafo(GRAFO* grafo){
    for(int i = 0; i < grafo->vertices; i++){
        ADJACENCIA *adj = grafo->adj[i].cab;
        while(adj != NULL){
            ADJACENCIA *temp = adj;
            adj = adj->prox;
            free(temp);
        }
    }
    free(grafo->adj);
    free(grafo);
}

bool criaAresta(GRAFO *gr, int vi, int vf, TIPOPESO p){
    if (!gr)
        return(false);
    if((vf<0) || (vf >= gr->vertices))
        return(false);
    if((vi<0) || (vi >= gr->vertices))
        return(false);
    
    gr->adj[vi].cab = inseriListAdj(gr->adj[vi].cab, vf, p);
    gr->adj[vf].cab = inseriListAdj(gr->adj[vf].cab, vi, p);
    
    gr->arestas++;
    return (true);
}



void imprime(GRAFO *gr){
    printf("Vertices: %d. Arestas: %d, \n", gr->vertices,gr->arestas);
    
    int i;
    for(i=0;i<gr->vertices; i++){
        printf("v%d: ",i);
        ADJACENCIA *ad = gr->adj[i].cab;
        while(ad){
            printf("v%d(%d) ", ad->vertice,ad->peso);
            ad = ad->prox;
        }
        
        printf("\n");
    }
}

// ----------- KRUSKAL ---------------------------

// Eu vou utilizar o algoritmo de ordenação QuickSort para ordernar as arestas do grafo, eu implementei ela em Projeto e Análise de Algoritmos para um trabalho
// em que foi verificado que o Quick Sort com pivô aleatório foi um dos mais rápidos
long int quickPivoAlea(ARESTA *lst, int inicio, int fim) {
    if (inicio >= fim) return 0; // Caso base para parar a recursão

    int random_index = rand() % (fim - inicio + 1) + inicio;
    ARESTA pivo = lst[random_index];
    int i = inicio - 1;
    int j = fim + 1;
    long int cont = 0;

    while (1) {
        do {
            i++;
        } while (lst[i].peso < pivo.peso);

        do {
            j--;
        } while (lst[j].peso > pivo.peso);

        if (i >= j) break;

        // Troca os elementos
        ARESTA aux = lst[i];
        lst[i] = lst[j];
        lst[j] = aux;
        cont++;
    }

    // Chamada recursiva para as duas partes
    cont += quickPivoAlea(lst, inicio, j);
    cont += quickPivoAlea(lst, j + 1, fim);

    return cont;
}

int find_Set(int *subConjunto, int i){
    if(subConjunto[i] == -1)
        return i;
    return subConjunto[i] = find_Set(subConjunto, subConjunto[i]);
}

void union_uv(int *sub_conjunto, int u, int v){
    int raiz_u = find_Set(sub_conjunto, u);
    int raiz_v = find_Set(sub_conjunto, v);
    if (raiz_u != raiz_v)
        sub_conjunto[raiz_u] = raiz_v;
}
void agm_kruskal(GRAFO *gr, int *pai){
    // implemente o agoritmo de kruskal
    int V = gr->vertices;
    int maxArestas = gr->arestas;
    // Variáveis auxiliares para contar as arestas do grafo e permitir a ordenação no passo 2
    ARESTA *arestas = (ARESTA*) malloc(maxArestas*sizeof(ARESTA));
    int cont_arestas = 0;
    int *sub_conjuntos = (int*)malloc(V*sizeof(int));

    //Passo 1: para cada v ∈ V(G)faça
    for(int i = 0; i < V; i++){
        sub_conjuntos[i] = -1; // cada vertice começa em seu proprio conjunto 
        pai[i] = -1; 
    }

    //Passo 2: Ordena arestas de E ( G ) por p não decrescente
    
    //Primeiro vou percorrer todo o grafo para obter todas arestas e depois vou ordená-las
    for(int u = 0; u < V; u++){
        ADJACENCIA *adj = gr->adj[u].cab;
        while(adj){
            if(u < adj->vertice){
                arestas[cont_arestas].u = u;
                arestas[cont_arestas].v = adj->vertice;
                arestas[cont_arestas].peso = adj->peso;
                cont_arestas++;
            }
            adj = adj->prox;
        }
    }

    quickPivoAlea(arestas, 0, cont_arestas - 1);

    // Passo 3: S = ∅
    int arestasAGM = 0;
    custo_total = 0;

    // Passo 4: para cada ( u , v ) ∈ E ( G ) em ordem não-decrescente faça
    for(int i = 0; i < cont_arestas && arestasAGM < V-1; i++){
        int u = arestas[i].u;
        int v = arestas[i].v;
        int raiz_u = find_Set(sub_conjuntos, u);
        int raiz_v = find_Set(sub_conjuntos, v);
        if(raiz_u != raiz_v){
            union_uv(sub_conjuntos, raiz_u, raiz_v);
            pai[v] = u;
            custo_total += arestas[i].peso;
            arestasAGM++;
        }
    }
    free(arestas);
    free(sub_conjuntos);
}

void lerArquivo_proc_Kruskal(char *nomeArq){
    FILE *arq = fopen(nomeArq, "r");
    GRAFO *gr = NULL;
    if(arq == NULL)
    {
        perror("[ERRO] Erro ao abrir arquivo:");
        exit(1);
    }
    char linha[256], c;
    int numV, numA;
    int u, v, peso;
    while(fscanf(arq, " %c", &c) != EOF){
        if(c == 'c'){
            if (fgets(linha, sizeof(linha), arq)) {}
        }else if(c == 'p'){
            if (fscanf(arq, " sp %d %d", &numV, &numA) != 2) {
                fprintf(stderr, "[ERRO] formato invalido na linha p do arquivo %s\n", nomeArq);
                fclose(arq);
                exit(1);
            }
            gr = criarGrafo(numV+1);
        } else if(c == 'a'){
            if (gr == NULL) {
                fprintf(stderr, "[ERRO] linha a antes de p no arquivo %s\n", nomeArq);
                fclose(arq);
                exit(1);
            }
            if (fscanf(arq, "%d %d %d", &u, &v, &peso) != 3) {
                fprintf(stderr, "[ERRO] formato invalido na linha a do arquivo %s\n", nomeArq);
                fclose(arq);
                liberaGrafo(gr);
                exit(1);
            }
            criaAresta(gr, u, v, peso);
        }
    } 
    fclose(arq);

    int *pai = (int*) malloc(gr->vertices*sizeof(int));

    //para medir o tempo
    struct tms start, end;
    clock_t start_time, end_time;

    start_time = times(&start);//Começa a medir o tempo de CPU
    agm_kruskal(gr, pai);
    end_time = times(&end);

    double tempo_total = (end_time - start_time) / (double)sysconf(_SC_CLK_TCK);

    FILE *arqSaida = fopen("saidaKruskal.txt", "a");
    if(arqSaida != NULL){
        fprintf(arqSaida, "Arquivo: %s\n", nomeArq);
        fprintf(arqSaida, "Custo total: %lld\n", custo_total);
        fprintf(arqSaida, "Tempo total: %.4f segundos\n", tempo_total);
    }
    fclose(arqSaida);
    liberaGrafo(gr);
    free(pai);
}


int main(){
    
    // crie o main para fazer a leitura do arquivo
    char *nome_arq1 = "USA-road-d.CAL.gr", *nome_arq2 = "USA-road-d.NY.gr";
    lerArquivo_proc_Kruskal(nome_arq1);
    lerArquivo_proc_Kruskal(nome_arq2);
    printf("Processamento concluído. Resultados salvos em 'saidaKruskal.txt'.\n");
    return 0;
}
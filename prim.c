#include <stdio.h>
#include <stdlib.h>
#define true 1
#define false 0
#define INT_MAX 2147483647
typedef int bool;
typedef int TIPOPESO;
#define MAX_HEAP 2966700

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

//Heap minímo, vou utilizar um algoritmo de heap desenvolvido em ED1. Só vou precisar modificar de heap max para heap minimo
// Define estrutura que armazena a prioridade e o valor/dado
struct est_dado_heap {
    int prioridade;
    int dado;
};

// Define estrutura que representa toda tabela Heap
struct est_heap {
    int qnt;
    struct est_dado_heap heap[MAX_HEAP];
};

// Define um alias para struct est_dado_heap
typedef struct est_dado_heap tipo_dado_heap;

// Define um alias para struct est_heap
typedef struct est_heap tipo_heap;

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

// Funções heap para auxiliar no algoritmo de Prim, citado nos slides

void inicializaHeap(tipo_heap *hp)
{
    hp->qnt = 0;
}

int filhoEsqHeap(int i)
{
    return (2*i+1);
}

int filhoDirHeap(int i)
{
    return (2*i+2);
}

int paiHeap(int i)
{
    return (int) (i-1)/2;
}

void sobeHeap(tipo_heap *hp, int pos)
{
    tipo_dado_heap aux;
    if(pos > 0)
    {
        //Verifica se prioridade do pai eh menor que a prioridade do filho
        if(hp->heap[pos].prioridade < hp->heap[(paiHeap(pos))].prioridade)
        {
            //Faz a troca
            aux =  (hp->heap[pos]);
            hp->heap[pos] = hp->heap[(paiHeap(pos))];
            hp->heap[paiHeap(pos)] = aux;
            sobeHeap(hp, paiHeap(pos));
        }
    }
}

void desceHeap(tipo_heap *hp, int pos)
{
    int pos_filho;
    tipo_dado_heap aux;
    if(filhoEsqHeap(pos) < hp->qnt) // Verifica se tem pelo menos um filho (à esquerda)
    {
        pos_filho = filhoEsqHeap(pos); // Assume que o filho à esquerda é o de maior prioridade
        // Verifica se tem um filho à direita e se ele tem prioridade maior que o filho à esquerda
        if(filhoDirHeap(pos) < hp->qnt && hp->heap[filhoDirHeap(pos)].prioridade <hp->heap[pos_filho].prioridade)
        {
            pos_filho = filhoDirHeap(pos);
        }
        
        if(hp->heap[pos].prioridade > hp->heap[pos_filho].prioridade)
        {
            aux = hp->heap[pos];
            hp->heap[pos] = hp->heap[pos_filho];
            hp->heap[pos_filho] = aux;
            desceHeap(hp, pos_filho);
        }
    }
}

void insereHeap(tipo_heap *hp, int prioridade, int dado)
{
    if(hp->qnt < MAX_HEAP)
    {
        hp->heap[hp->qnt].prioridade = prioridade;
        hp->heap[hp->qnt].dado = dado;
        hp->qnt++;
        sobeHeap(hp, hp->qnt-1);
    }
}

tipo_dado_heap removeHeap(tipo_heap *hp)
{
    tipo_dado_heap aux = {INT_MAX, -1}; // Retorna um valor de prioridade máxima e dado inválido caso o heap esteja vazio
    if(hp->qnt > 0)
    {
        aux = hp->heap[0];
        hp->heap[0] = hp->heap[hp->qnt - 1]; 
        hp->qnt--; 
        desceHeap(hp, 0);
    }
    return aux;
}

// ----------- PRIM ---------------------------
void agm_prim(GRAFO *gr, int orig, int *pai){
    int V = gr->vertices;
    int *chave = (int*) malloc(V*sizeof(int));
    bool *visitado = (bool*) malloc(V*sizeof(bool)); 
    int u;
    tipo_dado_heap min;
    tipo_heap *hp = (tipo_heap*)malloc(sizeof(tipo_heap));
    inicializaHeap(hp);
    
    for(int i = 0; i < V; i++){
        chave[i] = INT_MAX;
        pai[i] = -1;
        visitado[i] = false;
    }
    
    chave[orig] = 0;
    insereHeap(hp, 0, orig);
    
    while(hp->qnt > 0){
        min = removeHeap(hp);
        u = min.dado;
        
        // Se este vértice já saiu do heap antes e foi fechado, ignore as réplicas
        if(visitado[u]) continue; 
        
        visitado[u] = true; // Marca como definitivo na árvore
        
        ADJACENCIA *adj = gr->adj[u].cab;
        while(adj != NULL){
            int v = adj->vertice;
            int peso = adj->peso;
            
            // Só avalia se 'v' ainda não faz parte da AGM de forma definitiva
            if(!visitado[v] && peso < chave[v]){
                pai[v] = u;
                chave[v] = peso;
                insereHeap(hp, peso, v);
            }
            adj = adj->prox;
        }
    }
    free(chave);
    free(visitado);
    free(hp);
}

void lerArquivo_proc(char *nomeArq){
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
            if(fgets(linha, sizeof(linha), arq)){};
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
    agm_prim(gr, 1, pai);
    end_time = times(&end);

    double tempo_total = (end_time - start_time) / (double)sysconf(_SC_CLK_TCK);

    long long custo_total = 0;
    for(int i = 0; i < gr->vertices; i++){
        if(pai[i] != -1)
        {
            ADJACENCIA *adj = gr->adj[i].cab;
            while(adj != NULL){
                if(adj->vertice == pai[i]){
                    custo_total += adj->peso;
                    break;
                }
                adj = adj->prox;
            }
        }
    }

    FILE *arqSaida = fopen("saidaPrim.txt", "a");
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
    //GRAFO *gr = NULL;
    lerArquivo_proc(nome_arq1);
    lerArquivo_proc(nome_arq2);
    printf("Processamento concluído. Resultados salvos em 'saidaPrim.txt'.\n");
    return 0;
}
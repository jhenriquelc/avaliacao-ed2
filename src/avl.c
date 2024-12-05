/*Avaliação de Estruturas de Dados 2

Membros do grupo:
- João Henrique Linhares Corrêa: 256446
- Marcelo Henrique Guidini Angeli: 2564530
- Pedro Henrique Sauné: 2564572

*/

#include <errno.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum direcao {
    DIREITA,
    ESQUERDA
} Direcao;

typedef struct node {
    struct node* esq;
    struct node* dir;
    long int valor;
    size_t altura;
} Node;

// Obtem a altura de um nó.
size_t altura(Node* no) { return no != NULL ? no->altura : 0; }

size_t maior(size_t a, size_t b) { return a > b ? a : b; }

void arrumar_altura(Node* no){
    if (no==NULL){
        return;
    }
    no -> altura = maior(altura(no -> dir), altura(no -> esq)) +1;
}

// Inicializa uma nova folha alocada na heap.
Node* novo_node(long valor) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        perror("avl");
        exit(EXIT_FAILURE);
    }
    node->esq = NULL;
    node->dir = NULL;
    node->valor = valor;
    node->altura = 1;
    return node;
}

// Roda free() em todos os nós alocados.
void deletar(Node* raiz) {
    if (raiz == NULL)
        return;

    if (raiz->esq != NULL)
        deletar(raiz->esq);
    if (raiz->dir != NULL)
        deletar(raiz->dir);

    free(raiz);

    return;
}

// Obtêm a diferença entre alturas das subárvores (negativo: esquerda; positivo:
// direita).
int balanco(Node* raiz) {
    if (raiz == NULL)
        return 0;
    return (int)(altura(raiz->dir) - (altura(raiz->esq)));
}

// Executa uma rotação simples à esquerda.
Node* rse(Node* pivo) {
    if (pivo == NULL) {
        fprintf(stderr, "avl: NULL passado para 'rse'. Parando...\n");
        exit(EXIT_FAILURE);
    }
    printf("RSE %ld\n", pivo -> valor);
    Node* dir_pivo = pivo->dir;
    if (dir_pivo == NULL) {
        fprintf(stderr, "avl: rse inválido. Parando...\n");
        exit(EXIT_FAILURE);
    }
    Node* esq_dir_pivo = dir_pivo->esq;
    dir_pivo->esq = pivo;
    pivo->dir = esq_dir_pivo;
    arrumar_altura(pivo);
    arrumar_altura(dir_pivo);
    return dir_pivo;
}

// Executa uma rotação simples à direita.
Node* rsd(Node* pivo) {
    if (pivo == NULL) {
        fprintf(stderr, "avl: NULL passado para 'rsd'. Parando...\n");
        exit(EXIT_FAILURE);
    }
    printf("RSD %ld\n", pivo -> valor);
    Node* esq_pivo = pivo->esq;
    if (esq_pivo == NULL) {
        fprintf(stderr, "avl: rsd inválido. Parando...\n");
        exit(EXIT_FAILURE);
    }
    Node* dir_esq_pivo = esq_pivo->dir;
    esq_pivo->dir = pivo;
    pivo->esq = dir_esq_pivo;
    arrumar_altura(pivo);
    arrumar_altura(esq_pivo);
    return esq_pivo;
}

// Executa uma rotação dupla à esquerda.
Node* rde(Node* pivo){
    if (pivo == NULL) {
        fprintf(stderr, "avl: NULL passado para 'rde'. Parando...\n");
        exit(EXIT_FAILURE);
    }
    printf("RDE %ld\n", pivo -> valor);
    Node* dir_pivo = pivo->dir;
    if (dir_pivo == NULL) {
        fprintf(stderr, "avl: rde inválido. Parando...\n");
        exit(EXIT_FAILURE);
    }
    Node* esq_dir_pivo = dir_pivo -> esq;
    if (esq_dir_pivo == NULL){
        fprintf(stderr, "avl: rde inválido. Parando...\n");
        exit(EXIT_FAILURE);
    }
    pivo -> dir = esq_dir_pivo -> esq;
    dir_pivo -> esq = esq_dir_pivo -> dir;
    esq_dir_pivo -> dir = dir_pivo;
    esq_dir_pivo -> esq = pivo;
    arrumar_altura(pivo);
    arrumar_altura(dir_pivo);
    arrumar_altura(esq_dir_pivo);
    return esq_dir_pivo;
}

// Executa uma rotação dupla à direita.
Node* rdd(Node* pivo){
    if (pivo == NULL) {
        fprintf(stderr, "avl: NULL passado para 'rdd'. Parando...\n");
        exit(EXIT_FAILURE);
    }
    printf("RDD %ld\n", pivo -> valor);
    Node* esq_pivo = pivo->esq;
    if (esq_pivo == NULL) {
        fprintf(stderr, "avl: rdd inválido. Parando...\n");
        exit(EXIT_FAILURE);
    }
    Node* dir_esq_pivo = esq_pivo -> dir;
    if (dir_esq_pivo == NULL){
        fprintf(stderr, "avl: rdd inválido. Parando...\n");
        exit(EXIT_FAILURE);
    }
    pivo -> esq = dir_esq_pivo -> dir;
    esq_pivo -> dir = dir_esq_pivo -> esq;
    dir_esq_pivo -> esq = esq_pivo;
    dir_esq_pivo -> dir = pivo;
    arrumar_altura(pivo);
    arrumar_altura(esq_pivo);
    arrumar_altura(dir_esq_pivo);
    return dir_esq_pivo;
}

// Seleciona o algoritmo correto para o balanceamento e o executa.
Node* balancear(Node* raiz, Direcao inseriu_em){
    if (raiz==NULL){
        return raiz;
    }
    // checar balanço de raiz -> inseriu_em e balanço da raiz, calculando seu produto
    Node* filho = (inseriu_em == DIREITA ? raiz -> dir : raiz -> esq);
    int produto = balanco(raiz) * balanco(filho);

    // se o produto for maior que 0 (sinal igual), rotação simples na direção oposta a inseriu_em
    if (produto > 0){
        return (inseriu_em == ESQUERDA ? rsd(raiz) : rse(raiz));
    }

    // se o produto for menor que 0 (sinal oposto), rotação dupla na direção oposta a inseriu_em
    if (produto < 0){
        return (inseriu_em == ESQUERDA ? rdd(raiz) : rde(raiz));
    }

    // se o produto for 0 já está balanceado
    return raiz;
        
}

// Insere o valor na estrutura, balanceando conforme necessário.
Node* inserir(Node* raiz, long valor) {

    // inserir
    if (raiz == NULL) {
        return novo_node(valor);
    }
    Direcao inseriu_em;
    if (valor < raiz->valor) {
        raiz->esq = inserir(raiz->esq, valor);
        inseriu_em = ESQUERDA;
    } else {
        raiz->dir = inserir(raiz->dir, valor);
        inseriu_em = DIREITA;
    }

    // ajustar altura
    arrumar_altura(raiz);

    // balancear se necessário
    if (abs(balanco(raiz)) > 1)
        // passando como argumento a direção da inserção
        raiz = balancear(raiz, inseriu_em);

    return raiz;
}

// Coloca o próximo valor inserido em *valor_var, se houver;
// Indica se há próximo no booleano de retorno;
// Em caso de erros, encerra o programa.
bool proximo(int* valor_var) {
    errno = 0;
    int matches;
    while ((matches = scanf("%d", valor_var)) != EOF) {
        if (matches < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                continue;
            perror("avl");
            exit(EXIT_FAILURE);
        }
        if (matches == 0) {
            fprintf(stderr, "avl: entrada inválida (não é número). Parando...\n");
            exit(EXIT_FAILURE);
        }

        return *valor_var != -1;
    }
    return 0;
}

// Rotina principal
int main() {
    setlocale(LC_ALL, "Portuguese");

    Node* avl = NULL;
    int entrada;

    while (proximo(&entrada)) {

        if (entrada == 0) {
            printf("Bal = %d\n", balanco(avl));
            deletar(avl);
            avl = NULL;
            continue;
        }

        avl = inserir(avl, entrada);
    }

    printf("fim\n");
    exit(EXIT_SUCCESS);
}

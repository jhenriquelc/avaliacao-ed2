/*Avaliação de Estruturas de Dados 2

Membros do grupo:
- João Henrique Linhares Corrêa: 256446
- Marcelo Henrique Guidini Angeli: 2564530
- Pedro Henrique Sauné: 2564572

Entrega 1
*/

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    struct node* esq;
    struct node* dir;
    long int valor;
    size_t altura;
} Node;

// insere o valor na estrutura, balanceando conforme necessário.
Node* inserir(Node* raiz, long valor);

// Roda free() em todos os nós alocados.
void deletar(Node* raiz);

// Obtêm a diferença entre alturas das subárvores (negativo: esquerda; positivo:
// direita).
int balanco(Node* raiz) {
    if (raiz == NULL)
        return 0;
    return (int)((raiz->dir != NULL ? raiz->dir->altura : 0) -
                 (raiz->esq != NULL ? raiz->esq->altura : 0));
};

// Executa uma rotação simples à esquerda.
Node* rse(Node* pivo);

// Executa uma rotação simples à direita.
Node* rsd(Node* pivo);

// Executa uma rotação dupla à esquerda.
Node* rde(Node* pivo);

// Executa uma rotação dupla à direita.
Node* rdd(Node* pivo);

// Coloca o próximo valor inserido em *valor_var, se houver;
// Indica se há próximo no booleano de retorno;
// Em caso de erros, encerra o programa.
bool proximo(int* valor_var) {
    errno = 0;
    int matches;
    while ((matches = scanf("%d", valor_var)) != EOF) {
        if (matches < 0) {
            if (errno == EAGAIN)
                continue;
            perror("avl");
            exit(EXIT_FAILURE);
        }

        return *valor_var != -1;
    }
    return 0;
}

// Rotina principal
int main(int argc, char* argv[]) {

    Node* avl = NULL;
    int entrada;

    while (proximo(&entrada)) {

        if (entrada == 0) {
            printf("Bal=%d\n", balanco(avl));
            deletar(avl);
            avl = NULL;
            continue;
        }

        avl = inserir(avl, entrada);
    }

    exit(EXIT_SUCCESS);
}

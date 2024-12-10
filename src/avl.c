/*Avaliação de Estruturas de Dados 2

Membros do grupo:
- João Henrique Linhares Corrêa: 256446
- Marcelo Henrique Guidini Angeli: 2564530
- Pedro Henrique Sauné: 2564572

Testado nos SOs/compiladores:
- Fedora 41 (x86_64) / gcc version 14.2.1 20240912 (Red Hat 14.2.1-3) (GCC)
- Windows 11 (x86_64) / gcc version 14.2.0 (Rev2, Built by MSYS2 project)
- Windows 11 (x86_64) / Microsoft (R) C/C++ Optimizing Compiler Versão 19.42.34435 para x64

*/

#include <errno.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Enumeração para direção de inserção.
typedef enum direcao {
    DIREITA,
    ESQUERDA,
} Direcao;

// Estrutura de um nó da árvore AVL.
typedef struct node {
    struct node* esq;
    struct node* dir;
    long valor;
    size_t altura;
} Node;

// Formata uma string de erro, printa no stderr, e fecha (mata) o programa.
void morrer(char* mensagem) {
    fprintf(stderr, "avl: %s. Parando...\n", mensagem);
    exit(EXIT_FAILURE);
}

// Obtem a altura de um nó.
size_t altura(Node* no) { return no != NULL ? no->altura : 0; }

size_t maior(size_t a, size_t b) { return a > b ? a : b; }

// Arruma a altura de um nó baseado na altura de seus filhos.
void arrumar_altura(Node* no) {
    if (no == NULL) {
        return;
    }
    no->altura = maior(altura(no->dir), altura(no->esq)) + 1;
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

    deletar(raiz->esq);
    deletar(raiz->dir);

    free(raiz);

    return;
}

// Obtêm a diferença entre alturas das subárvores
// (negativo: esquerda; positivo: direita).
int balanco(Node* raiz) {
    if (raiz == NULL)
        return 0;
    return (int)(altura(raiz->dir) - (altura(raiz->esq)));
}

// Executa uma rotação simples à esquerda.
Node* rse(Node* pivo) {
    if (pivo == NULL) {
        morrer("NULL passado para 'rse'");
    }

    Node* dir_pivo = pivo->dir;
    if (dir_pivo == NULL) {
        morrer("rse inválido");
    }

    printf("RSE %ld\n", pivo->valor);

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
        morrer("NULL passado para 'rsd'");
    }

    Node* esq_pivo = pivo->esq;
    if (esq_pivo == NULL) {
        morrer("rsd inválido");
    }

    printf("RSD %ld\n", pivo->valor);

    Node* dir_esq_pivo = esq_pivo->dir;
    esq_pivo->dir = pivo;
    pivo->esq = dir_esq_pivo;

    arrumar_altura(pivo);
    arrumar_altura(esq_pivo);

    return esq_pivo;
}

// Executa uma rotação dupla à esquerda.
Node* rde(Node* pivo) {
    if (pivo == NULL) {
        morrer("NULL passado para 'rde'");
    }

    Node* dir_pivo = pivo->dir;
    if (dir_pivo == NULL) {
        morrer("rde inválido");
    }

    Node* esq_dir_pivo = dir_pivo->esq;
    if (esq_dir_pivo == NULL) {
        morrer("rde inválido");
    }

    printf("RDE %ld\n", pivo->valor);

    pivo->dir = esq_dir_pivo->esq;
    dir_pivo->esq = esq_dir_pivo->dir;
    esq_dir_pivo->dir = dir_pivo;
    esq_dir_pivo->esq = pivo;

    arrumar_altura(pivo);
    arrumar_altura(dir_pivo);
    arrumar_altura(esq_dir_pivo);

    return esq_dir_pivo;
}

// Executa uma rotação dupla à direita.
Node* rdd(Node* pivo) {
    if (pivo == NULL) {
        morrer("NULL passado para 'rdd'");
    }

    Node* esq_pivo = pivo->esq;
    if (esq_pivo == NULL) {
        morrer("rdd inválido");
    }

    Node* dir_esq_pivo = esq_pivo->dir;
    if (dir_esq_pivo == NULL) {
        morrer("rdd inválido");
    }

    printf("RDD %ld\n", pivo->valor);

    pivo->esq = dir_esq_pivo->dir;
    esq_pivo->dir = dir_esq_pivo->esq;
    dir_esq_pivo->esq = esq_pivo;
    dir_esq_pivo->dir = pivo;

    arrumar_altura(pivo);
    arrumar_altura(esq_pivo);
    arrumar_altura(dir_esq_pivo);

    return dir_esq_pivo;
}

// Seleciona o algoritmo correto para o balanceamento e o executa.
Node* balancear(Node* raiz, Direcao inseriu_em) {
    if (raiz == NULL) {
        return raiz;
    }
    // checar balanço de raiz -> inseriu_em e balanço da raiz,
    // calculando seu produto
    Node* filho = (inseriu_em == DIREITA ? raiz->dir : raiz->esq);
    int produto = balanco(raiz) * balanco(filho);

    // se o produto for maior que 0 (sinal igual),
    // rotação simples na direção oposta a inseriu_em
    if (produto > 0) {
        return (inseriu_em == ESQUERDA ? rsd(raiz) : rse(raiz));
    }

    // se o produto for menor que 0 (sinal oposto)
    // rotação dupla na direção oposta a inseriu_em
    if (produto < 0) {
        return (inseriu_em == ESQUERDA ? rdd(raiz) : rde(raiz));
    }

    // se o produto for 0 já está balanceado
    return raiz;
}

// Insere o valor na estrutura, balanceando conforme necessário e ajustando as
// alturas.
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
bool proximo(long* valor_var) {
    errno = 0;
    int matches;
    while ((matches = scanf("%ld", valor_var)) != EOF) {
        if (matches < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                continue;
            perror("avl");
            exit(EXIT_FAILURE);
        }

        if (matches == 0) {
            morrer("entrada inválida (não é número)");
        }

        // valor definido como valor de saída pelo professor.
        return *valor_var != -1;
    }
    return false;
}

// Rotina principal
int main() {
    setlocale(LC_ALL, "Portuguese");

    Node* avl = NULL;
    long entrada;

    while (proximo(&entrada)) {

        // valor definido como valor para excluir árvore pelo professor.
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

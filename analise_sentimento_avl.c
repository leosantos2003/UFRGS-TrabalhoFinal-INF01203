#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Variável global para contar as comparações
long long int comp_avl = 0;

// Estrutura do Nó da Árvore AVL
typedef struct NoAVL {
    char *palavra;
    float score;
    struct NoAVL *esq, *dir;
    int altura;
} NoAVL;

// Função para obter a altura de um nó
int altura(NoAVL *N) {
    if (N == NULL) return 0;
    return N->altura;
}

// Função utilitária para obter o máximo de dois inteiros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Função para criar um novo nó AVL
NoAVL* criarNoAVL(char *palavra, float score) {
    NoAVL* no = (NoAVL*)malloc(sizeof(NoAVL));
    no->palavra = strdup(palavra);
    no->score = score;
    no->esq = no->dir = NULL;
    no->altura = 1; // Novo nó é inicialmente adicionado na folha
    return no;
}

// Rotação à Direita
NoAVL *rotacaoDireita(NoAVL *y) {
    NoAVL *x = y->esq;
    NoAVL *T2 = x->dir;
    x->dir = y;
    y->esq = T2;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    return x;
}

// Rotação à Esquerda
NoAVL *rotacaoEsquerda(NoAVL *x) {
    NoAVL *y = x->dir;
    NoAVL *T2 = y->esq;
    y->esq = x;
    x->dir = T2;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    return y;
}

// Obter o fator de balanceamento de um nó
int getBalance(NoAVL *N) {
    if (N == NULL) return 0;
    return altura(N->esq) - altura(N->dir);
}

// Função para converter uma string para minúsculas
void paraMinusculas(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Função para inserir um nó na árvore AVL
NoAVL* inserirAVL(NoAVL* no, char* palavra, float score) {
    if (no == NULL) return criarNoAVL(palavra, score);

    int cmp = strcmp(palavra, no->palavra);
    if (cmp < 0)
        no->esq = inserirAVL(no->esq, palavra, score);
    else if (cmp > 0)
        no->dir = inserirAVL(no->dir, palavra, score);
    else // Palavras iguais não são permitidas no léxico (ou podem ser atualizadas)
        return no;

    no->altura = 1 + max(altura(no->esq), altura(no->dir));
    int balance = getBalance(no);

    // Casos de desbalanceamento
    // Esq-Esq
    if (balance > 1 && strcmp(palavra, no->esq->palavra) < 0)
        return rotacaoDireita(no);
    // Dir-Dir
    if (balance < -1 && strcmp(palavra, no->dir->palavra) > 0)
        return rotacaoEsquerda(no);
    // Esq-Dir
    if (balance > 1 && strcmp(palavra, no->esq->palavra) > 0) {
        no->esq = rotacaoEsquerda(no->esq);
        return rotacaoDireita(no);
    }
    // Dir-Esq
    if (balance < -1 && strcmp(palavra, no->dir->palavra) < 0) {
        no->dir = rotacaoDireita(no->dir);
        return rotacaoEsquerda(no);
    }

    return no;
}

// Função para buscar uma palavra na AVL e retornar seu score
NoAVL* consultarAVL(NoAVL *raiz, char *palavra) {
    NoAVL *atual = raiz;
    while (atual != NULL) {
        comp_avl++; // Incrementa o contador de comparações
        int cmp = strcmp(palavra, atual->palavra);
        if (cmp == 0) {
            return atual;
        }
        if (cmp < 0) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }
    return NULL; // Palavra não encontrada
}

// Função para liberar a memória da árvore
void liberarArvore(NoAVL* raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);
        free(raiz->palavra);
        free(raiz);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <arquivo_lexico> <arquivo_sentencas> <arquivo_saida>\n", argv[0]);
        return 1;
    }

    char *lexico_path = argv[1];
    char *sentencas_path = argv[2];
    char *saida_path = argv[3];

    FILE *f_lexico = fopen(lexico_path, "r");
    if (!f_lexico) {
        perror("Erro ao abrir o arquivo de lexico.");
        return 1;
    }

    NoAVL *raiz_avl = NULL;
    char palavra_lexico[100];
    float score_lexico;

    // Carregar o léxico na árvore
    while (fscanf(f_lexico, "%s %f", palavra_lexico, &score_lexico) == 2) {
        paraMinusculas(palavra_lexico);
        raiz_avl = inserirAVL(raiz_avl, palavra_lexico, score_lexico);
    }
    fclose(f_lexico);

    FILE *f_sentencas = fopen(sentencas_path, "r");
    if (!f_sentencas) {
        perror("Erro ao abrir o arquivo de sentencas.");
        liberarArvore(raiz_avl);
        return 1;
    }

    FILE *f_saida = fopen(saida_path, "w");
    if (!f_saida) {
        perror("Erro ao criar o arquivo de saida");
        fclose(f_sentencas);
        liberarArvore(raiz_avl);
        return 1;
    }

    char linha[1024];
    // Processar cada sentença
    while (fgets(linha, sizeof(linha), f_sentencas)) {
        linha[strcspn(linha, "\n")] = 0;

        float score_sentenca = 0.0f;
        char linha_copia[1024];
        strcpy(linha_copia, linha);

        char *palavra = strtok(linha_copia, " .,;!?()\t\n");
        while (palavra != NULL) {
            paraMinusculas(palavra);
            NoAVL* no_encontrado = consultarAVL(raiz_avl, palavra);
            if (no_encontrado != NULL) {
                score_sentenca += no_encontrado->score;
            }
            palavra = strtok(NULL, " .,;!?()\t\n");
        }
        fprintf(f_saida, "Score: %.2f | Sentença: %s\n", score_sentenca, linha);
    }

    fprintf(f_saida, "\n--------------------------------------------------\n");
    fprintf(f_saida, "Analise com Arvore AVL:\n");
    fprintf(f_saida, "Numero total de comparacoes: %lld\n", comp_avl);

    printf("Analise com AVL concluida. Resultados salvos em '%s'.\n", saida_path);
    printf("Total de comparacoes (AVL): %lld\n", comp_avl);

    fclose(f_sentencas);
    fclose(f_saida);
    liberarArvore(raiz_avl);

    return 0;
}
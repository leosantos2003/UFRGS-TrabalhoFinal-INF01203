#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Variável global para contar as comparações
long long int comp_bst = 0;

// Estrutura do Nó da Árvore Binária de Busca
typedef struct NoBST {
    char *palavra;
    float score;
    struct NoBST *esq, *dir;
} NoBST;

// Função para criar um novo nó
NoBST* criarNoBST(char *palavra, float score) {
    NoBST *novoNo = (NoBST*)malloc(sizeof(NoBST));
    novoNo->palavra = strdup(palavra); // Aloca memória e copia a palavra
    novoNo->score = score;
    novoNo->esq = novoNo->dir = NULL;
    return novoNo;
}

// Função para converter uma string para minúsculas
void paraMinusculas(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Função para inserir um nó na BST
NoBST* inserirBST(NoBST *raiz, char *palavra, float score) {
    if (raiz == NULL) {
        return criarNoBST(palavra, score);
    }
    int cmp = strcmp(palavra, raiz->palavra);
    if (cmp < 0) {
        raiz->esq = inserirBST(raiz->esq, palavra, score);
    } else if (cmp > 0) {
        raiz->dir = inserirBST(raiz->dir, palavra, score);
    }
    return raiz;
}

// Função para buscar uma palavra na BST e retornar seu score
NoBST* consultarBST(NoBST *raiz, char *palavra) {
    NoBST *atual = raiz;
    while (atual != NULL) {
        comp_bst++; // Incrementa o contador de comparações
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
void liberarArvore(NoBST* raiz) {
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

    NoBST *raiz_bst = NULL;
    char palavra_lexico[100];
    float score_lexico;

    // Carregar o léxico na árvore
    while (fscanf(f_lexico, "%s %f", palavra_lexico, &score_lexico) == 2) {
        paraMinusculas(palavra_lexico);
        raiz_bst = inserirBST(raiz_bst, palavra_lexico, score_lexico);
    }
    fclose(f_lexico);

    FILE *f_sentencas = fopen(sentencas_path, "r");
    if (!f_sentencas) {
        perror("Erro ao abrir o arquivo de sentencas.");
        liberarArvore(raiz_bst);
        return 1;
    }

    FILE *f_saida = fopen(saida_path, "w");
    if (!f_saida) {
        perror("Erro ao criar o arquivo de saida.");
        fclose(f_sentencas);
        liberarArvore(raiz_bst);
        return 1;
    }

    char linha[1024];
    // Processar cada sentença
    while (fgets(linha, sizeof(linha), f_sentencas)) {
        // Remover quebra de linha
        linha[strcspn(linha, "\n")] = 0;

        float score_sentenca = 0.0f;
        char linha_copia[1024];
        strcpy(linha_copia, linha);

        char *palavra = strtok(linha_copia, " .,;!?()\t\n");
        while (palavra != NULL) {
            paraMinusculas(palavra);
            NoBST* no_encontrado = consultarBST(raiz_bst, palavra);
            if (no_encontrado != NULL) {
                score_sentenca += no_encontrado->score;
            }
            palavra = strtok(NULL, " .,;!?()\t\n");
        }
        fprintf(f_saida, "Score: %.2f | Sentenca: %s\n", score_sentenca, linha);
    }

    fprintf(f_saida, "\n--------------------------------------------------\n");
    fprintf(f_saida, "Analise com Arvore Binaria de Busca (BST):\n");
    fprintf(f_saida, "Numero total de comparacoes: %lld\n", comp_bst);

    printf("Analise com BST concluida. Resultados salvos em '%s'.\n", saida_path);
    printf("Total de comparacoes (BST): %lld\n", comp_bst);

    fclose(f_sentencas);
    fclose(f_saida);
    liberarArvore(raiz_bst);

    return 0;
}
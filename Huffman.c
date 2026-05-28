/*
 * Estrutura de Dados II - Avaliacao 1
 * Compactacao de Arquivos com Algoritmo de Huffman
 *
 * Grupo: 2
 * Integrantes:
 *   - Antonella dos Santos Mota - 2023101236 - Turma 33.1
 *   - Caio Fernandes Coswosk - 2022200473 - Turma 3704.1
 *   - Felipe Oliveira Tigre - 2024201593 - Turma 3704.1
 *   - Lívia Endlich da Cruz - 2023200164 - Turma 3704.1
 *
 * Arquivo: Huffman.c
 * Descricao: Implementacao das funcoes para compactacao e
 *            descompactacao de arquivos de texto usando o
 *            algoritmo de Huffman. Sera preenchida ao longo
 *            das tarefas T03, T07, T09, T10, T12 e T13.
 */

#include <stdio.h>
#include <stdlib.h>
#include "Huffman.h"
#include "heap.h"

void inicializarFrequencias(int frequencias[256]) {
    for (int i=0; i<256; i++) {
        frequencias[i] = 0;
    }
}

int calcularFrequencias(const char *nomeArquivo, int frequencias[256]) {
    FILE *arquivo = fopen(nomeArquivo, "rb");   
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo.");
        return 0;
    }

    inicializarFrequencias(frequencias);

    int c;
    while ((c = fgetc(arquivo)) != EOF) {
        frequencias[(unsigned char)c]++;
    }

    fclose(arquivo);
    return 1;
}

void imprimirFrequencias(int frequencias[256]) {
    for (int i=0; i<256; i++) {
        if (frequencias[i] > 0) {
            printf("%d", i);

            if (i >= 32 && i <= 126) {
                printf(" ('%c')", i);
            }

            printf(": %d\n", frequencias[i]);
        }
    }
}

NoHuffman *criarNo(unsigned char caractere, int frequencia, NoHuffman *esq, NoHuffman *dir) {
    NoHuffman *novo = malloc(sizeof(NoHuffman));
    if (novo == NULL) {
        printf("Erro ao alocar no de Huffman.");
        return NULL;
    }

    novo->caractere = caractere;
    novo->frequencia = frequencia;
    novo->esq = esq;
    novo->dir = dir;

    return novo;
}

int ehFolha(NoHuffman *no) {
    return (no != NULL && no->esq == NULL && no->dir == NULL);
}

void liberarArvore(NoHuffman *raiz) {
    if (raiz == NULL) {
        return;
    }

    liberarArvore(raiz->esq);
    liberarArvore(raiz->dir);

    free(raiz);
}

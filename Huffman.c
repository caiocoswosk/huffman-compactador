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
#include <string.h>
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

NoHuffman *construirArvoreHuffman(int frequencias[256]) {
    int qtdCaracteres = 0;
    for (int i = 0; i < 256; i++) {
        if (frequencias[i] > 0) {
            qtdCaracteres++;
        }
    }

    if (qtdCaracteres == 0) {
        return NULL;
    }

    int capacidade = (qtdCaracteres == 1) ? 2 : qtdCaracteres;
    Heap *heap = criaFila(capacidade);

    for (int i = 0; i < 256; i++) {
        if (frequencias[i] > 0) {
            NoHuffman *folha = criarNo((unsigned char)i, frequencias[i], NULL, NULL);
            if (folha == NULL) {
                liberaHeap(heap);
                return NULL;
            }
            insere(heap, folha, frequencias[i]);
        }
    }

    /* Caso degenerado: arquivo com apenas 1 caractere distinto. Envolvemos a
     * folha em um no interno para que o codigo do caractere seja "0" e nao
     * uma string vazia. */
    if (qtdCaracteres == 1) {
        Elemento unico = extrairMinimo(heap);
        NoHuffman *folha = (NoHuffman *)unico.dado;
        NoHuffman *raiz = criarNo(0, folha->frequencia, folha, NULL);
        liberaHeap(heap);
        return raiz;
    }

    while (heap->tam > 1) {
        Elemento eA = extrairMinimo(heap);
        Elemento eB = extrairMinimo(heap);
        NoHuffman *a = (NoHuffman *)eA.dado;
        NoHuffman *b = (NoHuffman *)eB.dado;

        int somaFreq = a->frequencia + b->frequencia;
        NoHuffman *interno = criarNo(0, somaFreq, a, b);
        if (interno == NULL) {
            liberaHeap(heap);
            return NULL;
        }

        insere(heap, interno, somaFreq);
    }

    Elemento eRaiz = extrairMinimo(heap);
    NoHuffman *raiz = (NoHuffman *)eRaiz.dado;
    liberaHeap(heap);

    return raiz;
}

void liberarArvore(NoHuffman *raiz) {
    if (raiz == NULL) {
        return;
    }

    liberarArvore(raiz->esq);
    liberarArvore(raiz->dir);

    free(raiz);
}

void gerarTabelaCodigos(NoHuffman *raiz, char tabela[256][CODIGO_MAX], char codigoAtual[CODIGO_MAX], int profundidade){
    int i;

    if(raiz == NULL){
        return;
    }

    if(profundidade == 0){
        for(i = 0; i < 256; i++){
            tabela[i][0] = '\0';
        }
    }

    if(ehFolha(raiz)){
        codigoAtual[profundidade] = '\0';
        strcpy(tabela[raiz->caractere], codigoAtual);
        return;
    }


    codigoAtual[profundidade] = '0';
    gerarTabelaCodigos(raiz->esq, tabela, codigoAtual, profundidade + 1);

    codigoAtual[profundidade] = '1';
    gerarTabelaCodigos(raiz->dir, tabela, codigoAtual, profundidade + 1);
}

void imprimeArvore(NoHuffman *raiz, int nivel){
    int i;

    if(raiz == NULL){
        return;
    }

    for(i = 0; i < nivel; i++){
        printf(" ");
    }

    if(ehFolha(raiz)){
        if(raiz->caractere == '\n'){
            printf("'\\n' : %d\n", raiz->frequencia);
        } else if(raiz->caractere == '\t'){
            printf("'\\t' : %d\n", raiz->frequencia);
        } else if(raiz->caractere == ' '){
            printf("' ' : %d\n", raiz->frequencia);
        } else if(raiz->caractere >= 32 && raiz->caractere <= 126){
            printf("'%c' : %d\n", raiz->caractere, raiz->frequencia);
        } else{
            printf("%d : %d\n", raiz->caractere, raiz->frequencia);
        }
    } else{
        printf("%d\n", raiz->frequencia);
    }

    imprimeArvore(raiz->esq, nivel + 1);
    imprimeArvore(raiz->dir, nivel + 1);
}
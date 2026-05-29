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
        printf("  ");
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

static int escreverCabecalho(FILE *saida, int frequencias[256], unsigned char padding){
    if(fwrite(HUFF_MAGIC, 1, HUFF_MAGIC_BYTES, saida) != HUFF_MAGIC_BYTES){
        return 0;
    }

    if(fputc(padding, saida) == EOF){
        return 0;
    }

    for(int i = 0; i < 256; i++){
        uint32_t frequencia = (uint32_t)frequencias[i];

        if(fwrite(&frequencia, sizeof(uint32_t), 1, saida) != 1){
            return 0;
        }
    }

    return 1;
}

static int atualizarPadding(FILE *saida, unsigned char padding){
    if(fseek(saida, HUFF_MAGIC_BYTES, SEEK_SET) != 0){
        return 0;
    }

    if(fputc(padding, saida) == EOF){
        return 0;
    }

    return 1;
}

static int escreverBit(FILE *saida, int bit, unsigned char *buffer, int *bitsNoBuffer){
    *buffer = (unsigned char)(*buffer << 1);

    if(bit == 1){
        *buffer = (unsigned char)(*buffer | 1);
    }

    (*bitsNoBuffer)++;

    if(*bitsNoBuffer == 8){
        if(fputc(*buffer, saida) == EOF){
            return 0;
        }

        *buffer = 0;
        *bitsNoBuffer = 0;
    }

    return 1;
}

static int escreverCodigo(FILE *saida, const char *codigo, unsigned char *buffer, int *bitsNoBuffer){
    for(int i = 0; codigo[i] != '\0'; i++){
        int bit = (codigo[i] == '1') ? 1 : 0;

        if(!escreverBit(saida, bit, buffer, bitsNoBuffer)){
            return 0;
        }
    }

    return 1;
}

static int finalizarBuffer(FILE *saida, unsigned char *buffer, int *bitsNoBuffer, unsigned char *padding){
    *padding = 0;

    if(*bitsNoBuffer > 0){
        *padding = (unsigned char)(8 - *bitsNoBuffer);
        *buffer = (unsigned char)(*buffer << *padding);

        if(fputc(*buffer, saida) == EOF){
            return 0;
        }

        *buffer = 0;
        *bitsNoBuffer = 0;
    }

    return 1;
}

int compactarArquivo(const char *nomeEntrada, const char *nomeSaida){
    int frequencias[256];
    char tabela[256][CODIGO_MAX];
    char codigoAtual[CODIGO_MAX];
    NoHuffman *raiz;
    FILE *entrada;
    FILE *saida;
    unsigned char buffer = 0;
    unsigned char padding = 0;
    int bitsNoBuffer = 0;
    int c;

    if(!calcularFrequencias(nomeEntrada, frequencias)){
        return 0;
    }

    raiz = construirArvoreHuffman(frequencias);

    entrada = fopen(nomeEntrada, "rb");
    if(entrada == NULL){
        printf("Erro ao abrir arquivo de entrada.\n");
        liberarArvore(raiz);
        return 0;
    }

    saida = fopen(nomeSaida, "wb");
    if(saida == NULL){
        printf("Erro ao abrir arquivo de saida.\n");
        fclose(entrada);
        liberarArvore(raiz);
        return 0;
    }

    if(!escreverCabecalho(saida, frequencias, 0)){
        printf("Erro ao escrever cabecalho do arquivo compactado.\n");
        fclose(entrada);
        fclose(saida);
        liberarArvore(raiz);
        return 0;
    }

    if(raiz != NULL){
        gerarTabelaCodigos(raiz, tabela, codigoAtual, 0);

        while((c = fgetc(entrada)) != EOF){
            unsigned char caractere = (unsigned char)c;

            if(!escreverCodigo(saida, tabela[caractere], &buffer, &bitsNoBuffer)){
                printf("Erro ao escrever dados compactados.\n");
                fclose(entrada);
                fclose(saida);
                liberarArvore(raiz);
                return 0;
            }
        }

        if(!finalizarBuffer(saida, &buffer, &bitsNoBuffer, &padding)){
            printf("Erro ao finalizar dados compactados.\n");
            fclose(entrada);
            fclose(saida);
            liberarArvore(raiz);
            return 0;
        }
    }

    if(!atualizarPadding(saida, padding)){
        printf("Erro ao atualizar padding do arquivo compactado.\n");
        fclose(entrada);
        fclose(saida);
        liberarArvore(raiz);
        return 0;
    }

    fclose(entrada);
    fclose(saida);
    liberarArvore(raiz);

    return 1;
}

int descompactarArquivo(const char *nomeEntrada, const char *nomeSaida){
    FILE *entrada;
    FILE *saida;
    char magic[HUFF_MAGIC_BYTES];
    unsigned char padding;
    uint32_t frequencia32[256];
    int frequencias[256];
    long tamArquivo;
    long tamCorpo;
    long bitsValidos;
    long bitsConsumidos = 0;
    NoHuffman *raiz;
    NoHuffman *atual;
    int byte, i, bit;


    entrada = fopen(nomeEntrada, "rb");

    if(entrada == NULL){
        printf("Erro ao abrir arquivo compactado!\n");
        return 0;
    }

    saida = fopen (nomeSaida, "wb");
    if(saida == NULL){
        printf("Erro ao criar o aquivo de saida!\n");
        fclose(entrada);
        return 0;
    }

    if((fread(magic, 1, HUFF_MAGIC_BYTES, entrada) != HUFF_MAGIC_BYTES) || (memcmp(magic, HUFF_MAGIC, HUFF_MAGIC_BYTES) != 0)){
        printf("Arquivo invalido\n");
        fclose(entrada);
        fclose(saida);
        return 0;
    }

    if(fread(&padding, 1, 1, entrada) != 1 || padding > 7){
        printf("Padding invalido\n");
        fclose(entrada);
        fclose(saida);
        return 0;
    }

    if(fread(frequencia32, sizeof(uint32_t), 256, entrada) != 256){
        printf("Tabela de frequencias invalida\n");
        fclose(entrada);
        fclose(saida);
        return 0;
    }

    for(i = 0; i < 256; i++){
        frequencias[i] = (int)frequencia32[i];
    }

    if(fseek(entrada, 0, SEEK_END) != 0){
        fclose(entrada);
        fclose(saida);
        return 0;
    }

    tamArquivo = ftell(entrada);
    if(tamArquivo < 0){
        fclose(entrada);
        fclose(saida);
        return 0;
    }

    tamCorpo = tamArquivo - HUFF_CABECALHO_BYTES;
    bitsValidos = tamCorpo * 8 - padding;

    if(tamCorpo < 0 || bitsValidos < 0){
        printf("Arquivo compactado corrompido.\n");
        fclose(entrada);
        fclose(saida);
        return 0;
    }


    if(fseek(entrada, HUFF_CABECALHO_BYTES, SEEK_SET) != 0){
        fclose(entrada);
        fclose(saida);
        return 0;
    }

    raiz = construirArvoreHuffman(frequencias);

    if(raiz == NULL){
        fclose(entrada);
        fclose(saida);
        return 1;
    }

    atual = raiz;

    while(bitsConsumidos < bitsValidos && (byte = fgetc(entrada)) != EOF){
        for(bit = 7; bit >= 0 && bitsConsumidos < bitsValidos; bit--){
            int valorBit = (byte >> bit) & 1;

            if(valorBit == 0){
                atual = atual->esq;
            } else{
                atual = atual->dir;
            }

            if(atual == NULL){
                printf("Erro ao percorrer a arvore. Arquivo corrompido.\n");
                liberarArvore(raiz);
                fclose(entrada);
                fclose(saida);
                return 0;
            }

            if(ehFolha(atual)){
                if(fputc(atual->caractere, saida) == EOF){
                    liberarArvore(raiz);
                    fclose(entrada);
                    fclose(saida);
                    return 0;
                }

                atual = raiz;
            }

            bitsConsumidos++;
        }
    }

    liberarArvore(raiz);
    fclose(entrada);
    fclose(saida);

    return 1;
}
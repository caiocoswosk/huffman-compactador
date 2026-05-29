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
 * Arquivo: Huffman.h
 * Descricao: Interface das funcoes para compactacao e
 *            descompactacao de arquivos de texto usando
 *            o algoritmo de Huffman.
 */

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdint.h>

#define HUFF_MAGIC              "HUFF"
#define HUFF_MAGIC_BYTES        4
#define HUFF_PADDING_BYTES      1
#define HUFF_NUM_SIMBOLOS       256
#define HUFF_FREQ_ENTRADA_BYTES 4
#define HUFF_TABELA_BYTES       (HUFF_NUM_SIMBOLOS * HUFF_FREQ_ENTRADA_BYTES)
#define HUFF_CABECALHO_BYTES    (HUFF_MAGIC_BYTES + HUFF_PADDING_BYTES + HUFF_TABELA_BYTES)

#define CODIGO_MAX 256

typedef struct noHuffman NoHuffman;

struct noHuffman {
    unsigned char caractere;
    int frequencia;
    NoHuffman *esq;
    NoHuffman *dir;
};

void inicializarFrequencias(int frequencias[256]);

int calcularFrequencias(const char *nomeArquivo, int frequencias[256]);

void imprimirFrequencias(int frequencias[256]);

NoHuffman *criarNo(unsigned char caractere, int frequencia, NoHuffman *esq, NoHuffman *dir);

int ehFolha(NoHuffman *no);

NoHuffman *construirArvoreHuffman(int frequencias[256]);

void liberarArvore(NoHuffman *raiz);

void gerarTabelaCodigos(NoHuffman *raiz, char tabela[256][CODIGO_MAX], char codigoAtual[CODIGO_MAX], int profundidade);

void imprimeArvore(NoHuffman *raiz, int nivel);

int compactarArquivo(const char *nomeEntrada, const char *nomeSaida);

int descompactarArquivo(const char *nomeEntrada, const char *nomeSaida);

#endif /* HUFFMAN_H */

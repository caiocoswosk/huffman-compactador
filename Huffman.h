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
 *            A implementacao detalhada sera feita nas tarefas T03, T07,
 *            T09, T10, T12 e T13.
 */

#ifndef HUFFMAN_H
#define HUFFMAN_H

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

void liberarArvore(NoHuffman *raiz);

#endif /* HUFFMAN_H */

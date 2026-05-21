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
 * Arquivo: heap.h
 * Descricao: Interface da fila de prioridades generica
 *            implementada como heap binario de minimo.
 *            A implementacao detalhada sera feita na tarefa T02.
 */

#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>

typedef struct elemento{
    void *dado;
    int chave;
} Elemento;

typedef struct heap{
    Elemento *vetor;
    int tam;
    int capacidade;
} Heap;

Heap *criaFila(int capacidade);

void insere(Heap *h, void *dado, int chave);

Elemento extrairMinimo(Heap *h);

Elemento minimo(Heap *h);

void decrementa(Heap *h, int indice, int novaChave);

void deleta(Heap *h, int indice);

void imprimir(Heap *h);

void liberaHeap(Heap *h);


#endif /* HEAP_H */

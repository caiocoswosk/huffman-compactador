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
 * Arquivo: heap.c
 * Descricao: Implementacao da fila de prioridades generica
 *            como heap binario de minimo. Sera preenchida na tarefa T02.
 */

#include "heap.h"

// Funções auxiliares
int pai(int i){
    return ((i - 1)/2);
}

int esquerda(int i){
    return (2* i+1);
}

int direita(int i){
    return (2* i+2);
}

void troca(Elemento *a, Elemento *b){
    Elemento aux;

    aux = *a;
    *a = *b;
    *b = aux;
}

// Função para fazer um elemento descer até encontrar sua posição correta
void desceFila(Heap *h, int indice){
    int menor, esq, dir;

    menor = indice;
    esq = esquerda(indice);
    dir = direita(indice);

    // avalia filho esquerdo
    if((esq < h->tam) && (h->vetor[esq].chave < h->vetor[menor].chave)){
        menor = esq;
    }

    // avalia filho direito
    if((dir < h->tam) && (h->vetor[dir].chave < h->vetor[menor].chave)){
        menor = dir;
    }

    // Quando encontra o menor filho ocorre a troca
    if(menor != indice){
        troca(&h->vetor[indice], &h->vetor[menor]);
        desceFila(h, menor);
    }
}

void sobeFila(Heap *h, int indice){
    while(indice > 0 && h->vetor[pai(indice)].chave > h->vetor[indice].chave){
        troca(&h->vetor[pai(indice)], &h->vetor[indice]);
        indice = pai(indice);

    }
}


Heap *criaFila(int capacidade){
    Heap *h = (Heap *)malloc(sizeof(Heap));

    if(h == NULL){
        printf("Erro de alocacao.\n");
        exit(1);
    }

    h->vetor = (Elemento *)malloc(capacidade * sizeof(Elemento));

    if(h->vetor == NULL){
        printf("Erro de alocacao.\n");
        free(h);
        exit(1);
    }

    h->tam = 0;
    h->capacidade = capacidade;

    return h;
}

void insere(Heap *h, void *dado, int chave){
    int i;

    if(h->tam == h->capacidade){
        printf("Heap em sua capacidade maxima\n");
        return;
    }

    i = h->tam;

    h->vetor[i].dado = dado;
    h->vetor[i].chave = chave;

    h->tam++;

    sobeFila(h, i);

}

Elemento minimo(Heap *h){ // não sei será útil 
    if(h->tam <= 0){
        printf("Heap vazio\n");

        Elemento vz;
        vz.dado = NULL;
        vz.chave = -1;

        return vz;
    }

    return h->vetor[0];
}

Elemento extrairMinimo(Heap *h){
    if (h->tam <= 0){
        printf("Heap vazio\n");

        Elemento vz;
        vz.dado = NULL;
        vz.chave = -1;

        return vz;
    }

    Elemento raiz = h->vetor[0];

    h->vetor[0] = h->vetor[h->tam-1];
    h->tam--;

    desceFila(h, 0);

    return raiz;

}

void decrementa(Heap *h, int indice, int novaChave){
    if(indice >= h->tam){
        printf("Indice invalido\n");
        return;
    }

    if(novaChave > h->vetor[indice].chave){
        printf("Nova chave eh maior que a atual\n");
        return;
    }

    h->vetor[indice].chave = novaChave;

    sobeFila(h, indice);
}

void deleta(Heap *h, int indice){
    if(indice >= h->tam){
        printf("Indice invalido\n");
        return;
    }

    h->vetor[indice] = h->vetor[h->tam - 1];

    h->tam--;

    if(indice > 0 && h->vetor[indice].chave < h->vetor[pai(indice)].chave){
        sobeFila(h, indice);
    }else{
        desceFila(h, indice);
    }
}

void imprimir(Heap *h){
    int i;

    printf("HEAP: ");
    for(i = 0; i < h->tam; i++){
        printf("%d ", h->vetor[i].chave);
    }
    printf("\n");
}

void liberaHeap(Heap *h){
    free(h->vetor);
    free(h);
}
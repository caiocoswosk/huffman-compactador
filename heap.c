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
 *            como heap binario de minimo.
 */

#include <limits.h>
#include "heap.h"

/* Heap 1-based: a raiz fica em vetor[1] e os indices validos sao 1..tam.
 * O slot vetor[0] e reservado e nao e utilizado, seguindo a convencao
 * apresentada na aula 03 (slide 3). */

int pai(int i){
    return i / 2;
}

int esquerda(int i){
    return 2 * i;
}

int direita(int i){
    return 2 * i + 1;
}

void troca(Elemento *a, Elemento *b){
    Elemento aux;

    aux = *a;
    *a = *b;
    *b = aux;
}

/* MinHeapify: faz o elemento em 'indice' descer ate restaurar
 * a propriedade de heap minimo. Equivalente ao procedimento da
 * aula 03 (slide 9) adaptado para heap minimo. */
void desceFila(Heap *h, int indice){
    int menor, esq, dir;

    esq = esquerda(indice);
    dir = direita(indice);
    menor = indice;

    if(esq <= h->tam && h->vetor[esq].chave < h->vetor[menor].chave){
        menor = esq;
    }

    if(dir <= h->tam && h->vetor[dir].chave < h->vetor[menor].chave){
        menor = dir;
    }

    if(menor != indice){
        troca(&h->vetor[indice], &h->vetor[menor]);
        desceFila(h, menor);
    }
}

/* Sift-up usado por DecrementaChave (aula 03, slide 17). */
void sobeFila(Heap *h, int indice){
    while(indice > 1 && h->vetor[pai(indice)].chave > h->vetor[indice].chave){
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

    /* Aloca capacidade+1 slots para acomodar a indexacao 1-based:
     * vetor[0] fica reservado e usamos vetor[1..capacidade]. */
    h->vetor = (Elemento *)malloc((capacidade + 1) * sizeof(Elemento));

    if(h->vetor == NULL){
        printf("Erro de alocacao.\n");
        free(h);
        exit(1);
    }

    h->tam = 0;
    h->capacidade = capacidade;

    return h;
}

/* InserirHeapMinimo (aula 03, slide 17): insere o elemento com chave
 * +infinito no fim do heap e em seguida chama DecrementaChave para
 * trazer a chave real ao seu lugar. */
void insere(Heap *h, void *dado, int chave){
    if(h->tam == h->capacidade){
        printf("Heap em sua capacidade maxima\n");
        return;
    }

    h->tam++;
    h->vetor[h->tam].dado = dado;
    h->vetor[h->tam].chave = INT_MAX;

    decrementa(h, h->tam, chave);
}

Elemento minimo(Heap *h){
    if(h->tam < 1){
        printf("Heap vazio\n");

        Elemento vz;
        vz.dado = NULL;
        vz.chave = -1;

        return vz;
    }

    return h->vetor[1];
}

Elemento extrairMinimo(Heap *h){
    if(h->tam < 1){
        printf("Heap vazio\n");

        Elemento vz;
        vz.dado = NULL;
        vz.chave = -1;

        return vz;
    }

    Elemento raiz = h->vetor[1];

    h->vetor[1] = h->vetor[h->tam];
    h->tam--;

    desceFila(h, 1);

    return raiz;
}

void decrementa(Heap *h, int indice, int novaChave){
    if(indice < 1 || indice > h->tam){
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
    if(indice < 1 || indice > h->tam){
        printf("Indice invalido\n");
        return;
    }

    h->vetor[indice] = h->vetor[h->tam];
    h->tam--;

    if(indice > 1 && h->vetor[indice].chave < h->vetor[pai(indice)].chave){
        sobeFila(h, indice);
    } else{
        desceFila(h, indice);
    }
}

void imprimir(Heap *h){
    int i;

    printf("HEAP: ");
    for(i = 1; i <= h->tam; i++){
        printf("%d ", h->vetor[i].chave);
    }
    printf("\n");
}

void liberaHeap(Heap *h){
    free(h->vetor);
    free(h);
}

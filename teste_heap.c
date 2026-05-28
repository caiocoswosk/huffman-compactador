#include <stdio.h>
#include <string.h>
#include "heap.h"

typedef struct tarefaTeste {
    char nome[30];
    int prioridade;
} TarefaTeste;

int testarInteiros(void);
int testarStrings(void);
int testarStructs(void);

static int verificarChave(Elemento elemento, int esperado, const char *teste) {
    if (elemento.chave != esperado) {
        printf("[ERRO] %s: esperado chave %d, obtido %d.\n",
               teste, esperado, elemento.chave);
        return 0;
    }

    return 1;
}

int testarInteiros(void) {
    int valores[] = {50, 10, 30, 5};
    int esperado[] = {5, 10, 30, 50};
    int total = 4;
    Heap *heap = criaFila(total);

    for (int i = 0; i < total; i++) {
        insere(heap, &valores[i], valores[i]);
    }

    for (int i = 0; i < total; i++) {
        Elemento elemento = extrairMinimo(heap);
        int *valor = (int *)elemento.dado;

        if (!verificarChave(elemento, esperado[i], "inteiros") || valor == NULL || *valor != esperado[i]) {
            printf("[ERRO] inteiros: valor extraido incorreto.\n");
            liberaHeap(heap);
            return 0;
        }
    }

    liberaHeap(heap);
    printf("[OK] Teste com inteiros passou.\n");
    return 1;
}

int testarStrings(void) {
    char *palavras[] = {"banana", "oi", "casa", "a"};
    char *esperado[] = {"a", "oi", "casa", "banana"};
    int total = 4;
    Heap *heap = criaFila(total);

    for (int i = 0; i < total; i++) {
        insere(heap, palavras[i], (int)strlen(palavras[i]));
    }

    for (int i = 0; i < total; i++) {
        Elemento elemento = extrairMinimo(heap);
        char *palavra = (char *)elemento.dado;

        if (!verificarChave(elemento, (int)strlen(esperado[i]), "strings") || palavra == NULL || strcmp(palavra, esperado[i]) != 0) {
            printf("[ERRO] strings: esperado \"%s\", obtido \"%s\".\n",
                   esperado[i], palavra != NULL ? palavra : "NULL");
            liberaHeap(heap);
            return 0;
        }
    }

    liberaHeap(heap);
    printf("[OK] Teste com strings passou.\n");
    return 1;
}

int testarStructs(void) {
    TarefaTeste tarefas[] = {
        {"media", 20},
        {"alta", 5},
        {"baixa", 50}
    };
    char *esperado[] = {"alta", "media", "baixa"};
    int prioridadesEsperadas[] = {5, 20, 50};
    int total = 3;
    Heap *heap = criaFila(total);

    for (int i = 0; i < total; i++) {
        insere(heap, &tarefas[i], tarefas[i].prioridade);
    }

    for (int i = 0; i < total; i++) {
        Elemento elemento = extrairMinimo(heap);
        TarefaTeste *tarefa = (TarefaTeste *)elemento.dado;

        if (!verificarChave(elemento, prioridadesEsperadas[i], "structs") || tarefa == NULL || strcmp(tarefa->nome, esperado[i]) != 0 || tarefa->prioridade != prioridadesEsperadas[i]) {
            printf("[ERRO] structs: tarefa extraida incorreta.\n");
            liberaHeap(heap);
            return 0;
        }
    }

    liberaHeap(heap);
    printf("[OK] Teste com structs passou.\n");
    return 1;
}

int main(void) {
    int ok = 1;

    ok = testarInteiros() && ok;
    ok = testarStrings() && ok;
    ok = testarStructs() && ok;

    if (ok) {
        printf("Todos os testes do heap passaram.\n");
        return 0;
    }

    printf("Algum teste do heap falhou.\n");
    return 1;
}
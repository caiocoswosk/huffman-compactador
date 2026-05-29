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
 * Arquivo: main.c
 * Descricao: Ponto de entrada do programa. Implementacao do menu
 *            interativo com as 5 opcoes do enunciado (Secao 3.1).
 *            Nesta etapa (T04) as chamadas reais ainda nao estao
 *            ligadas — cada opcao apenas le os dados necessarios e
 *            imprime um placeholder "nao implementado". A amarracao
 *            com as funcoes de Huffman acontece em T15.
 */

#include <stdio.h>
#include <string.h>

#include "Huffman.h"
#include "heap.h"

#define TAM_LINHA 256
#define TAM_NOME_ARQ 256

/*
 * Le uma linha do stdin para 'destino' (capacidade 'capacidade'),
 * removendo o '\n' final quando presente. Retorna 1 em sucesso e
 * 0 em EOF/erro. Em caso de linha maior que o buffer, descarta o
 * restante ate o proximo '\n' para nao contaminar a proxima leitura.
 */
static int ler_linha(char *destino, size_t capacidade) {
    if (fgets(destino, (int)capacidade, stdin) == NULL) {
        return 0;
    }

    size_t len = strlen(destino);
    if (len > 0 && destino[len - 1] == '\n') {
        destino[len - 1] = '\0';
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            /* descarta resto da linha */
        }
    }
    return 1;
}

static void exibir_menu(void) {
    printf("\n=========================================\n");
    printf(" Compactador de Huffman - Grupo 2\n");
    printf("=========================================\n");
    printf(" 1. Comprimir um arquivo\n");
    printf(" 2. Imprimir contagem de ocorrencias\n");
    printf(" 3. Imprimir codigo (arvore) de Huffman\n");
    printf(" 4. Descomprimir um arquivo\n");
    printf(" 5. Sair\n");
    printf("-----------------------------------------\n");
    printf("Escolha uma opcao: ");
}

/*
 * Le a opcao do menu como inteiro entre 1 e 5. Retorna a opcao
 * lida em sucesso e -1 em EOF ou entrada invalida.
 */
static int ler_opcao(void) {
    char linha[TAM_LINHA];
    if (!ler_linha(linha, sizeof linha)) {
        return -1;
    }

    int opcao;
    char extra;
    if (sscanf(linha, " %d %c", &opcao, &extra) == 1 &&
        opcao >= 1 && opcao <= 5) {
        return opcao;
    }
    return -1;
}

static int ler_nome_arquivo(const char *rotulo, char *destino, size_t capacidade) {
    printf("%s", rotulo);
    if (!ler_linha(destino, capacidade)) {
        return 0;
    }
    if (destino[0] == '\0') {
        printf("Nome de arquivo vazio.\n");
        return 0;
    }
    return 1;
}

static void opcao_comprimir(char *arquivo_entrada, char *arquivo_saida, int *entrada_definida) {
    char entrada[TAM_NOME_ARQ];
    char saida[TAM_NOME_ARQ];

    if (!ler_nome_arquivo("Arquivo de entrada (.txt): ", entrada, sizeof entrada)) {
        return;
    }
    if (!ler_nome_arquivo("Arquivo de saida: ", saida, sizeof saida)) {
        return;
    }

    strcpy(arquivo_entrada, entrada);
    strcpy(arquivo_saida, saida);
    *entrada_definida = 1;

    if (compactarArquivo(arquivo_entrada, arquivo_saida)) {
        printf("\nArquivo compactado com sucesso: %s\n", arquivo_saida);
    } else {
        printf("\nErro ao compactar o arquivo.\n");
    }
}

static void opcao_contagem(const char *arquivo_entrada, int entrada_definida) {
    if (!entrada_definida) {
        printf("\nNenhum arquivo foi informado ainda. Use a opcao 1 primeiro.\n");
        return;
    }

    int frequencias[256];

    if (calcularFrequencias(arquivo_entrada, frequencias)) {
        imprimirFrequencias(frequencias);
    } else {
        printf("\nErro ao calcular frequencias.\n");
    }
}

static void opcao_imprimir_arvore(const char *arquivo_entrada, int entrada_definida) {
    if (!entrada_definida) {
        printf("\nNenhum arquivo foi informado ainda. Use a opcao 1 primeiro.\n");
        return;
    }

    int frequencias[256];

    if (!calcularFrequencias(arquivo_entrada, frequencias)) { 
        printf("\nErro ao calcular frequencias.\n");
        return;
    }

    NoHuffman *raiz = construirArvoreHuffman(frequencias);
    if (raiz == NULL) {
        printf("\nArquivo vazio. Nao ha arvore para imprimir.\n");
        return;
    }

    imprimeArvore(raiz, 0);
    liberarArvore(raiz);
}

static void opcao_descomprimir(void) {
    char entrada[TAM_NOME_ARQ];
    char saida[TAM_NOME_ARQ];

    if (!ler_nome_arquivo("Arquivo compactado: ", entrada, sizeof entrada)) {
        return;
    }
    if (!ler_nome_arquivo("Arquivo de saida: ", saida, sizeof saida)) {
        return;
    }

    if (descompactarArquivo(entrada, saida)) {
        printf("\nArquivo descompactado com sucesso: %s\n", saida);
    } else {
        printf("\nErro ao descompactar o arquivo.\n");
    }
}

int main(void) {
    char arquivo_entrada[TAM_NOME_ARQ] = "";
    char arquivo_saida[TAM_NOME_ARQ] = "";
    int entrada_definida = 0;

    for (;;) {
        exibir_menu();
        int opcao = ler_opcao();

        if (opcao == -1) {
            if (feof(stdin)) {
                printf("\nEntrada encerrada. Saindo.\n");
                return 0;
            }
            printf("\nOpcao invalida. Informe um numero entre 1 e 5.\n");
            continue;
        }

        switch (opcao) {
            case 1:
                opcao_comprimir(arquivo_entrada, arquivo_saida, &entrada_definida);
                break;
            case 2:
                opcao_contagem(arquivo_entrada, entrada_definida);
                break;
            case 3:
                opcao_imprimir_arvore(arquivo_entrada, entrada_definida);
                break;
            case 4:
                opcao_descomprimir();
                break;
            case 5:
                printf("\nEncerrando o programa.\n");
                return 0;
        }
    }
}

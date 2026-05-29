# Formato do arquivo binário `.huff` — Grupo 2 (ED2 Av.1)

Este documento define o **contrato binário** entre a função de compactação
(T12) e a função de descompactação (T13). Qualquer divergência entre os dois
lados quebra a verificação final por `md5sum` exigida no enunciado, então este
formato deve ser tratado como fonte única da verdade.

Documento fechado em **2026-05-28** (T09 do plano de execução). Mudanças neste
contrato exigem combinar com Dev 1 (T13) e Dev 2 (T12) antes de implementar.

---

## 1. Layout geral

Um arquivo `.huff` tem exatamente esta sequência de bytes, do início ao fim,
sem separadores entre seções:

```
+-------------------+   <- offset 0
|  MAGIC  (4 bytes) |
+-------------------+   <- offset 4
| PADDING (1 byte)  |
+-------------------+   <- offset 5
|  FREQS (1024 bytes)|
+-------------------+   <- offset 1029
|  CORPO (n bytes)  |
+-------------------+   <- offset 1029 + n  (fim do arquivo)
```

O cabeçalho tem **tamanho fixo de 1029 bytes**. O corpo tem tamanho variável
(zero ou mais bytes). Não há `EOF marker` no final — o tamanho do arquivo
delimita o corpo.

---

## 2. Cabeçalho

### 2.1. `MAGIC` — 4 bytes

Sequência ASCII fixa `H`, `U`, `F`, `F` (bytes `0x48 0x55 0x46 0x46`). Serve
para detectar arquivos inválidos ou corrompidos logo na abertura. A
descompactação **deve recusar** qualquer arquivo cujos 4 primeiros bytes não
sejam esses.

### 2.2. `PADDING` — 1 byte

Número de bits de padding zero no **último** byte do corpo, no intervalo
`0..7`. Significa "este último byte tem `8 - PADDING` bits válidos; os
`PADDING` bits menos significativos são zeros de enchimento e devem ser
ignorados ao descompactar".

Regras:
- Se `CORPO` tem zero bytes (caso de arquivo de entrada vazio),
  `PADDING = 0`.
- Caso contrário, `PADDING` é determinado no fechamento do último byte:
  se a quantidade total de bits de código for múltiplo de 8, `PADDING = 0`;
  caso contrário, `PADDING = 8 - (total_bits mod 8)`.

### 2.3. `FREQS` — 1024 bytes (256 × 4)

Tabela completa de frequências do arquivo de entrada, indexada por byte
(0..255). Cada entrada é um inteiro **`uint32_t` em little-endian** com o
número de ocorrências daquele byte no arquivo original.

A escolha de gravar a tabela inteira (256 entradas), em vez de só os
caracteres com frequência > 0, é deliberada:

- **Simplicidade:** ler/gravar é um `fwrite`/`fread` único de 1024 bytes.
- **Reuso de T07:** a descompactação chama exatamente a mesma
  `construirArvoreHuffman()` da compactação, sem precisar de outro caminho
  para reconstruir a árvore.
- **Overhead aceitável:** 1024 bytes constantes não são problema para
  arquivos de teste (≥100 KB no enunciado da Seção 5).

A descompactação reconstrói a árvore alimentando essas frequências de volta
para `construirArvoreHuffman()`. **É obrigatório que a iteração de inserção
das folhas no heap percorra o vetor de frequências do índice 0 ao 255, em
ordem crescente, em ambos os lados** — assim, empates de frequência caem na
mesma ordem de inserção e a árvore reconstruída é idêntica à original.

---

## 3. Corpo

Sequência de bits dos códigos de Huffman concatenados na ordem em que os
caracteres do arquivo original aparecem, empacotados em bytes.

### 3.1. Ordem dos bits dentro de cada byte: **MSB-first**

O primeiro bit do stream de códigos vai no bit **mais significativo** (bit 7)
do primeiro byte do corpo. O segundo bit vai no bit 6 do primeiro byte. E
assim por diante. Quando os 8 bits do byte estão preenchidos, o próximo bit
vai no bit 7 do byte seguinte.

```
byte do corpo:    bit 7  bit 6  bit 5  bit 4  bit 3  bit 2  bit 1  bit 0
                  ^                                                  ^
                  primeiro bit do stream            ...      oitavo bit
```

### 3.2. Último byte e padding

Se o número total de bits gerados não for múltiplo de 8, os bits restantes do
último byte são preenchidos com **zero** e o número de bits inúteis é
registrado em `PADDING` (Seção 2.2). A descompactação **deve parar de
consumir bits** quando atingir `bits_validos = 8 * tamanho_corpo - PADDING`.

> ⚠️ Não confiar em "chegar em uma folha" como condição de parada — em casos
> patológicos os bits de padding podem coincidentemente formar um código
> válido e gerar um caractere espúrio no fim do arquivo descomprimido.

---

## 4. Casos extremos

### 4.1. Arquivo de entrada vazio

- `FREQS`: 256 zeros (4096 bits, 1024 bytes de zeros).
- `CORPO`: 0 bytes.
- `PADDING`: 0.
- Tamanho total do `.huff`: 1029 bytes.

Na descompactação, `construirArvoreHuffman()` retorna `NULL` para essa tabela
e o arquivo de saída deve ser criado vazio.

### 4.2. Arquivo com apenas 1 caractere distinto

A árvore construída em T07 envolve a folha única em um nó interno extra
(`esq = folha, dir = NULL`), garantindo que o código do caractere seja `"0"`
em vez de string vazia. O corpo, portanto, terá tantos bits `0` quanto o
número de ocorrências do caractere, agrupados MSB-first em bytes.

### 4.3. Arquivo com 256 caracteres distintos

Limite superior nominal. Nenhum tratamento especial: o cabeçalho continua
com 1024 bytes e o corpo é gerado normalmente. A profundidade máxima de
código teórica em Huffman para 256 símbolos é 255 — a tabela de códigos em
T10 deve dimensionar buffers considerando essa cota (por exemplo, 256
caracteres + terminador).

---

## 5. Conversão prática (resumo para T12/T13)

**Compactação (T12):**
1. `fwrite("HUFF", 1, 4, saida)`
2. Reservar 1 byte para `PADDING` (gravar `0` agora, sobrescrever no fim).
3. `fwrite(frequencias_uint32, 4, 256, saida)` (já são uint32 little-endian
   se a máquina for x86/x86_64 — confirmar com `assert(sizeof(uint32_t) == 4)`).
4. Reler arquivo de entrada byte a byte; para cada byte, concatenar os bits
   do código (vindo da tabela de T10) num acumulador `uint8_t buffer = 0`,
   `int bits_no_buffer = 0`. Quando `bits_no_buffer == 8`, gravar `buffer` e
   resetar.
5. No fim, se `bits_no_buffer > 0`, deslocar `buffer` para o lado esquerdo
   completando 8 bits (os zeros à direita viram padding), gravar o byte, e
   registrar `PADDING = 8 - bits_no_buffer`.
6. `fseek(saida, 4, SEEK_SET); fwrite(&padding, 1, 1, saida);`

**Descompactação (T13):**
1. Ler 4 bytes e validar contra `"HUFF"`. Se diferente, erro.
2. Ler 1 byte para `padding`.
3. Ler 1024 bytes para `frequencias_uint32` (256 entradas).
4. Determinar `bytes_corpo = tamanho_arquivo - 1029` (com `fseek/ftell`).
5. `bits_validos = 8 * bytes_corpo - padding`.
6. Chamar `construirArvoreHuffman(frequencias)` para reconstruir a árvore.
7. Ler o corpo byte a byte; para cada byte, ler os 8 bits MSB-first e navegar
   na árvore (`0` = esq, `1` = dir). Ao chegar em folha, gravar o caractere
   no arquivo de saída e voltar à raiz. Parar exatamente quando
   `bits_consumidos == bits_validos`.

---

## 6. Constantes do contrato

As constantes abaixo estão exportadas em `Huffman.h` para que T12 e T13 não
hardcodem números mágicos:

| Constante                | Valor      | Significado                          |
|--------------------------|------------|--------------------------------------|
| `HUFF_MAGIC`             | `"HUFF"`   | Magic bytes do cabeçalho             |
| `HUFF_MAGIC_BYTES`       | `4`        | Tamanho do magic                     |
| `HUFF_PADDING_BYTES`     | `1`        | Tamanho do campo padding             |
| `HUFF_NUM_SIMBOLOS`      | `256`      | Tamanho da tabela de frequências     |
| `HUFF_FREQ_ENTRADA_BYTES`| `4`        | Tamanho de uma entrada (uint32_t)    |
| `HUFF_TABELA_BYTES`      | `1024`     | Tamanho total de `FREQS`             |
| `HUFF_CABECALHO_BYTES`   | `1029`     | Tamanho total do cabeçalho           |

/*
 * Autores: 
 *  - John William Vicente <ra118237@uem.br>
 *  - Gabriel Valentim de Oliveira Dacie <ra118419@uem.br>
*/

#include "arvoreB.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    ERRO,
    PROMOCAO,
    SEM_PROMOCAO,
    ENCONTRADO,
    NAO_ENCONTRADO
} Codigos;

bool busca(int, int, int *, int *);
bool buscaNaPagina(int, Pagina, int *);
Pagina criaPaginaVazia();
PaginaAuxiliar criaPaginaAuxiliarVazia();
Pagina getPaginaPeloRRN(int);
int leiaIntDasChaves(FILE *, int *);
Codigos inseriChave(int, int, int *, int *);
Pagina inseriNaPagina(int, int, Pagina);
void escrevePagina(Pagina, int);
FILE *criaArquivoEscrita(char *);
FILE *abreArquivo(char *);
int byteOffsetApartirDoRRN(int);
void inseriChavePaginaAuxiliar(int, int, PaginaAuxiliar *);
void divide(int chave, int filhoDireita, Pagina *pag, int *chavePromovida, int *rrn, Pagina *novaPagina);

int qtd = 0;

void geraArvoreB(char *nomeArquivo) {
    int numero;
    int raiz = 0;
    Pagina pag = criaPaginaVazia();
    FILE *arquivoDados = abreArquivo(nomeArquivo);
    FILE *arvore = criaArquivoEscrita(ARQUIVO_DADOS);
    fwrite(&raiz, sizeof(int), 1, arvore);
    fwrite(&pag, sizeof(Pagina), 1, arvore);
    fclose(arvore);

    int chavePromovida, filhoDireita;
    while (leiaIntDasChaves(arquivoDados, &numero) != EOF) {
        Codigos c = inseriChave(raiz, numero, &chavePromovida, &filhoDireita);
        if (c == PROMOCAO) {
            Pagina novapag = criaPaginaVazia();
            novapag.chaves[0] = chavePromovida;
            novapag.filhos[0] = raiz;
            novapag.filhos[1] = filhoDireita;
            novapag.numeroDeChaves = 1;
            qtd += 1;
            escrevePagina(novapag, qtd);
            raiz = qtd;
            printf("raiz %d\n", raiz);
        }
    }

    for (int i = 0; i <= qtd; i++) {
        Pagina pag = getPaginaPeloRRN(i);
        puts("\n------------------");
        printf("RRN: %d\n", i);
        printf("Numero elementos: %d", pag.numeroDeChaves);
        printf("\nchaves: ");
        for (int i = 0; i < ORDEM_ARVORE_B - 1; i++) {
            printf("%d |", pag.chaves[i]);
        }

        printf("\nfilhos: ");
        for (int i = 0; i < ORDEM_ARVORE_B; i++) {
            printf("%d |", pag.filhos[i]);
        }
        puts("\n------------------\n ");
    }
    printf("RAIZ %d\n", raiz);
    fclose(arquivoDados);
}

void imprimeArvoreB() {
    for (int i = 0; i < 3; i++) {
        Pagina pag = getPaginaPeloRRN(i);
        puts("\n------------------");
        printf("RRN: %d\n", i);
        printf("Numero elementos: %d", pag.numeroDeChaves);
        printf("\nchaves: ");
        for (int i = 0; i < ORDEM_ARVORE_B - 1; i++) {
            printf("%d |", pag.chaves[i]);
        }

        printf("\nfilhos: ");
        for (int i = 0; i < ORDEM_ARVORE_B; i++) {
            printf("%d |", pag.filhos[i]);
        }
        puts("\n------------------\n ");
    }
}
void mostraChavesOrdenadasArvoreB() {
}

int leiaIntDasChaves(FILE *file, int *num) {
    return fscanf(file, "%d|", num);
}

Codigos inseriChave(int rrn, int chave, int *chavePromovida, int *filhoDireita) {
    if (rrn < 0) {
        *chavePromovida = chave;
        *filhoDireita = -1;

        return PROMOCAO;
    }

    int pos = 0;
    Pagina pag = getPaginaPeloRRN(rrn);
    bool encontrada = buscaNaPagina(chave, pag, &pos);
    printf("CHAVE : %d - POSICAO : %d\n", chave, pos);

    if (encontrada) {
        return ERRO;
    }

    Codigos codigo = inseriChave(pag.filhos[pos], chave, chavePromovida, filhoDireita);

    if (codigo == ERRO || codigo == SEM_PROMOCAO) {
        return codigo;
    }

    if (pag.numeroDeChaves < ORDEM_ARVORE_B - 1) {
        pag = inseriNaPagina(*chavePromovida, *filhoDireita, pag);
        escrevePagina(pag, rrn);

        return SEM_PROMOCAO;
    } else {
        Pagina pagaux = criaPaginaVazia();
        divide(chave, *filhoDireita, &pag, chavePromovida, filhoDireita, &pagaux);

        escrevePagina(pag, rrn);
        escrevePagina(pagaux, *filhoDireita);
        return PROMOCAO;
    }
}

Pagina criaPaginaVazia() {
    Pagina *pag = (Pagina *)malloc(sizeof(Pagina));
    pag->numeroDeChaves = 0;

    for (int i = 0; i < ORDEM_ARVORE_B - 1; i++) {
        pag->chaves[i] = -1;
    }

    for (int i = 0; i < ORDEM_ARVORE_B; i++) {
        pag->filhos[i] = -1;
    }

    return *pag;
}

PaginaAuxiliar criaPaginaAuxiliarVazia() {
    PaginaAuxiliar *pag = (PaginaAuxiliar *)malloc(sizeof(Pagina));

    for (int i = 0; i < ORDEM_ARVORE_B; i++) {
        pag->chaves[i] = -1;
    }

    for (int i = 0; i < ORDEM_ARVORE_B + 1; i++) {
        pag->filhos[i] = -1;
    }

    return *pag;
}

void escrevePagina(Pagina pag, int rrn) {
    FILE *arquiArvoreB = abreArquivo(ARQUIVO_DADOS);
    fseek(arquiArvoreB, byteOffsetApartirDoRRN(rrn), SEEK_SET);
    fwrite(&pag, sizeof(Pagina), 1, arquiArvoreB);
    fclose(arquiArvoreB);
}

Pagina inseriNaPagina(int chave, int filho, Pagina pag) {
    int i = pag.numeroDeChaves;

    while (i > 0 && chave < pag.chaves[i - 1]) {
        pag.chaves[i] = pag.chaves[i - 1];
        pag.filhos[i + 1] = pag.filhos[i];
        i -= 1;
    }

    pag.chaves[i] = chave;
    pag.filhos[i + 1] = filho;
    pag.numeroDeChaves += 1;

    return pag;
}

bool busca(int rrn, int chave, int *rrn_encontrado, int *pos_encontrada) {
    if (rrn < 0) {
        return false;
    }
    Pagina pag = criaPaginaVazia();
    int pos;

    pag = getPaginaPeloRRN(rrn);

    bool encontrado = buscaNaPagina(chave, pag, &pos);

    if (encontrado) {
        *rrn_encontrado = rrn;
        *pos_encontrada = pos;

        return true;
    }

    return busca(pag.filhos[pos], chave, rrn_encontrado, pos_encontrada);
}

Pagina getPaginaPeloRRN(int rrn) {
    FILE *arquiArvoreB = abreArquivo(ARQUIVO_DADOS);
    Pagina pag;

    fseek(arquiArvoreB, byteOffsetApartirDoRRN(rrn), SEEK_SET);
    fread(&pag, sizeof(Pagina), 1, arquiArvoreB);

    fclose(arquiArvoreB);
    return pag;
}

bool buscaNaPagina(int chave, Pagina pag, int *pos) {
    int i = 0;
    while (i < pag.numeroDeChaves && chave > pag.chaves[i]) {
        i++;
        *pos = i;
        if (i < pag.numeroDeChaves && chave == pag.chaves[i]) {
            return true;
        }
    }
    return false;
}

void divide(int chave, int filhoDireita, Pagina *pag, int *chavePromovida, int *rrn, Pagina *novaPagina) {
    PaginaAuxiliar paux;
    int tam = (ORDEM_ARVORE_B - 1);

    for (int i = 0; i < tam; i++) {
        paux.chaves[i] = pag->chaves[i];
    }
    for (int i = 0; i < ORDEM_ARVORE_B; i++) {
        paux.filhos[i] = pag->filhos[i];
    }
    inseriChavePaginaAuxiliar(chave, filhoDireita, &paux);

    int meio = ORDEM_ARVORE_B / 2;

    for (int i = 0; i < tam; i++) {
        if (i < meio) {
            pag->chaves[i] = paux.chaves[i];
        } else {
            pag->chaves[i] = -1;
        }
    }

    for (int i = 0; i < tam + 1; i++) {
        if (i < meio) {
            pag->filhos[i] = paux.filhos[i];
        } else {
            pag->filhos[i] = -1;
        }
    }

    for (int i = meio + 1; i < ORDEM_ARVORE_B; i++) {
        novaPagina->chaves[i - (meio + 1)] = paux.chaves[i];
    }

    for (int i = meio + 1; i < ORDEM_ARVORE_B + 1; i++) {
        novaPagina->filhos[i - (meio + 1)] = paux.filhos[i];
    }
    novaPagina->filhos[meio] = paux.filhos[tam + 1];

    *chavePromovida = paux.chaves[meio];
    pag->numeroDeChaves = meio;
    novaPagina->numeroDeChaves = meio - 1;
    qtd += 1;
    *rrn = qtd;
}

void inseriChavePaginaAuxiliar(int chave, int filhoDireita, PaginaAuxiliar *pag) {
    int i = ORDEM_ARVORE_B - 1;

    while (i > 0 && chave < pag->chaves[i - 1]) {
        pag->chaves[i] = pag->chaves[i - 1];
        pag->filhos[i + 1] = pag->filhos[i];
        i -= 1;
    }

    pag->chaves[i] = chave;
    pag->filhos[i + 1] = filhoDireita;
    pag->numeroDeChaves += 1;
}

FILE *criaArquivoEscrita(char *nomeArquivo) {
    /*
        Ambos r+e w+podem ler e gravar em um arquivo. No entanto, r+ não exclui o conteúdo do
        arquivo e não cria um novo arquivo se tal arquivo não existir, enquanto w+ exclui o
        conteúdo do arquivo e o cria se ele não existir.

        URL: https://stackoverflow.com/questions/21113919/difference-between-r-and-w-in-fopen
    */
    FILE *arquivo = fopen(nomeArquivo, "w");

    if (arquivo == NULL) {
        fprintf(stderr, "Nao foi possivel abrir o aquivo %s\n", nomeArquivo);
        exit(1);
    }

    return arquivo;
}

FILE *abreArquivo(char *nomeArquivo) {
    /*
        Ambos r+e w+podem ler e gravar em um arquivo. No entanto, r+ não exclui o conteúdo do
        arquivo e não cria um novo arquivo se tal arquivo não existir, enquanto w+ exclui o
        conteúdo do arquivo e o cria se ele não existir.

        URL: https://stackoverflow.com/questions/21113919/difference-between-r-and-w-in-fopen
    */
    FILE *arquivo = fopen(nomeArquivo, "r+");

    if (arquivo == NULL) {
        fprintf(stderr, "Nao foi possivel abrir o aquivo %s\n", nomeArquivo);
        exit(1);
    }

    return arquivo;
}

int byteOffsetApartirDoRRN(int rrn) {
    return sizeof(int) + (rrn * sizeof(Pagina));
}
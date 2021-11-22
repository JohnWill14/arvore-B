#ifndef _PAGINA_UTIL_
#define _PAGINA_UTIL_

#include <stdio.h>
#include <stdlib.h>

#include "arvoreB.h"
#include "fileUtil.h"

Pagina criaPaginaVazia();
PaginaAuxiliar criaPaginaAuxiliarVazia();
Pagina getPaginaPeloRRN(int);
Pagina inseriNaPagina(int, int, Pagina);

void inseriChavePaginaAuxiliar(int, int, PaginaAuxiliar *);
bool busca(int, int, int *, int *);
bool buscaNaPagina(int, Pagina, int *);
void escrevePagina(Pagina, int);
void exibePagina(Pagina);
int byteOffsetApartirDoRRN(int);

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

PaginaAuxiliar copiaPaginaParaAuxiliar(Pagina *pagina) {
    PaginaAuxiliar paux;

    for (int i = 0; i < ORDEM_ARVORE_B - 1; i++) {
        paux.chaves[i] = pagina->chaves[i];
    }
    for (int i = 0; i < ORDEM_ARVORE_B; i++) {
        paux.filhos[i] = pagina->filhos[i];
    }
    paux.numeroDeChaves = ORDEM_ARVORE_B - 1;

    return paux;
}

Pagina getPaginaPeloRRN(int rrn) {
    FILE *arquiArvoreB = abreArquivo(ARQUIVO_DADOS);
    Pagina pag;

    fseek(arquiArvoreB, byteOffsetApartirDoRRN(rrn), SEEK_SET);
    fread(&pag, sizeof(Pagina), 1, arquiArvoreB);

    fclose(arquiArvoreB);
    return pag;
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

bool busca(int rrn, int chave, int *rrn_encontrado, int *pos_encontrada) {
    if (rrn < 0) {
        return false;
    }
    Pagina pag;
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

bool buscaNaPagina(int chave, Pagina pag, int *pos) {
    int i = 0;
    while (i < pag.numeroDeChaves && chave >= pag.chaves[i]) {
        if (i < pag.numeroDeChaves && chave == pag.chaves[i]) {
            return true;
        }
        i++;
    }
    *pos = i;
    return false;
}

void escrevePagina(Pagina pag, int rrn) {
    FILE *arquiArvoreB = abreArquivo(ARQUIVO_DADOS);
    fseek(arquiArvoreB, byteOffsetApartirDoRRN(rrn), SEEK_SET);
    fwrite(&pag, sizeof(Pagina), 1, arquiArvoreB);
    fclose(arquiArvoreB);
}

void exibePagina(Pagina pag) {
    printf("chaves: ");
    for (int i = 0; i < pag.numeroDeChaves; i++) {
        printf("%d", pag.chaves[i]);
        if (i == pag.numeroDeChaves - 1) {
            printf("\n");
        } else {
            printf(" | ");
        }
    }

    printf("filhos: ");
    for (int i = 0; i < pag.numeroDeChaves + 1; i++) {
        printf("%d", pag.filhos[i]);
        if (i == pag.numeroDeChaves) {
            printf("\n");
        } else {
            printf(" | ");
        }
    }
    puts("\n\n");
}

void exibePaginaAuxiliar(PaginaAuxiliar pag) {
    puts("\n--------AUXILIAR----------");
    printf("Numero elementos: %d", pag.numeroDeChaves);
    printf("\nchaves: ");
    for (int i = 0; i < ORDEM_ARVORE_B; i++) {
        printf("%d |", pag.chaves[i]);
    }

    printf("\nfilhos: ");
    for (int i = 0; i < ORDEM_ARVORE_B + 1; i++) {
        printf("%d |", pag.filhos[i]);
    }
    puts("\n------------------\n ");
}

int byteOffsetApartirDoRRN(int rrn) {
    return sizeof(int) + (rrn * sizeof(Pagina));
}

#endif
#ifndef _PAGINA_UTIL_
#define _PAGINA_UTIL_

#include <stdio.h>
#include <stdlib.h>

#include "arvoreB.h"
#include "fileUtil.h"

Pagina criaPaginaVazia();
PaginaAuxiliar criaPaginaAuxiliarVazia();
Pagina lePaginaPeloRRN(int);
Pagina inseriNaPagina(int, int, Pagina);

void inseriChavePaginaAuxiliar(int, int, PaginaAuxiliar *);
bool busca(int, int, int *, int *);
bool buscaNaPagina(int, Pagina, int *);
void atualizaPagina(Pagina, int);
void adicionaNovaPagina(Pagina);
int rnnNovaPagina();
void escrevePagina(Pagina, int);
void exibePagina(Pagina);

void geraNovaArvore() {
    int raiz = 0, qtd = 1;
    Pagina pag = criaPaginaVazia();
    FILE *arvore = criaArquivoEscrita(ARQUIVO_DADOS);

    fwrite(&raiz, sizeof(int), 1, arvore);
    fwrite(&qtd, sizeof(int), 1, arvore);
    fwrite(&pag, sizeof(Pagina), 1, arvore);

    fclose(arvore);
}

Pagina criaPaginaVazia() {
    Pagina *pag = (Pagina *)malloc(sizeof(Pagina));
    pag->numeroDeChaves = 0;

    for (int i = 0; i < ORDEM_ARVORE_B - 1; i++) {
        pag->chaves[i] = 0;
    }

    for (int i = 0; i < ORDEM_ARVORE_B; i++) {
        pag->filhos[i] = -1;
    }

    return *pag;
}

PaginaAuxiliar criaPaginaAuxiliarVazia() {
    PaginaAuxiliar *pag = (PaginaAuxiliar *)malloc(sizeof(Pagina));
    pag->numeroDeChaves = 0;

    for (int i = 0; i < ORDEM_ARVORE_B; i++) {
        pag->chaves[i] = 0;
    }

    for (int i = 0; i < ORDEM_ARVORE_B + 1; i++) {
        pag->filhos[i] = -1;
    }

    return *pag;
}

void resetaPagina(Pagina *pag) {
    pag->numeroDeChaves = 0;

    for (int i = 0; i < ORDEM_ARVORE_B - 1; i++) {
        pag->chaves[i] = 0;
    }

    for (int i = 0; i < ORDEM_ARVORE_B; i++) {
        pag->filhos[i] = -1;
    }
}

PaginaAuxiliar copiaPaginaParaAuxiliar(Pagina *pagina) {
    PaginaAuxiliar paux;

    for (int i = 0; i < ORDEM_ARVORE_B - 1; i++) {
        paux.chaves[i] = pagina->chaves[i];
    }

    for (int i = 0; i < ORDEM_ARVORE_B; i++) {
        paux.filhos[i] = pagina->filhos[i];
    }

    paux.numeroDeChaves = pagina->numeroDeChaves;

    return paux;
}

Pagina lePaginaPeloRRN(int rrn) {
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

void atualizaPagina(Pagina pag, int rrn) {
    escrevePagina(pag, rrn);
}

void adicionaNovaPagina(Pagina pag) {
    int qtd = rnnNovaPagina();
    escrevePagina(pag, qtd);
    alteraQuantidade(qtd + 1);
}

int rnnNovaPagina() {
    return quantidadePagina();
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
        printf("%02d", pag.chaves[i]);

        if (i == pag.numeroDeChaves - 1) {
            printf("\n");
        } else {
            printf(" | ");
        }
    }

    printf("filhos: ");

    for (int i = 0; i < pag.numeroDeChaves + 1; i++) {
        printf("%02d", pag.filhos[i]);

        if (i == pag.numeroDeChaves) {
            printf("\n");
        } else {
            printf(" | ");
        }
    }
    puts("\n");
}

#endif
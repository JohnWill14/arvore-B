/*
 * Autores:
 *  - John William Vicente <ra118237@uem.br>
 *  - Gabriel Valentim de Oliveira Dacie <ra118419@uem.br>
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "arvoreB.h"
#include "fileUtil.h"
#include "paginaUtil.h"

typedef enum {
    ERRO,
    PROMOCAO,
    SEM_PROMOCAO,
    ENCONTRADO,
    NAO_ENCONTRADO
} Codigo;

void geraArvoreB(char *);
void imprimeArvoreB();
void mostraChavesOrdenadasArvoreB();

Codigo inseriChave(int, int, int *, int *);
void divide(int chave, int filhoDireita, Pagina *pag, int *chavePromovida, int *rrn, Pagina *novaPagina);
void imprimiEmOrdem(int);

void geraArvoreB(char *nomeArquivo) {
    geraNovaArvore();

    int valorEntrada;
    int raiz = raizArvoreB();

    FILE *arquivoDados = abreArquivo(nomeArquivo);

    int chavePromovida, filhoDireita;
    while (leiaIntDasChaves(arquivoDados, &valorEntrada) != EOF) {
        Codigo c = inseriChave(raiz, valorEntrada, &chavePromovida, &filhoDireita);

        if (c == PROMOCAO) {
            Pagina novapag = criaPaginaVazia();

            novapag.chaves[0] = chavePromovida;
            novapag.filhos[0] = raiz;
            novapag.filhos[1] = filhoDireita;
            novapag.numeroDeChaves = 1;

            adicionaNovaPagina(novapag);
            raiz = quantidadePagina() - 1;
            alteraRaizArvoreB(raiz);
        }
    }

    fclose(arquivoDados);
}

Codigo inseriChave(int rrn, int chave, int *chavePromovida, int *filhoDireita) {
    if (rrn < 0) {
        *chavePromovida = chave;
        *filhoDireita = -1;

        return PROMOCAO;
    }

    int pos = 0;
    Pagina pag = lePaginaPeloRRN(rrn);
    bool encontrada = buscaNaPagina(chave, pag, &pos);

    if (encontrada) {
        return ERRO;
    }

    Codigo codigo = inseriChave(pag.filhos[pos], chave, chavePromovida, filhoDireita);

    if (codigo == ERRO || codigo == SEM_PROMOCAO) {
        return codigo;
    }

    if (pag.numeroDeChaves < ORDEM_ARVORE_B - 1) {
        pag = inseriNaPagina(*chavePromovida, *filhoDireita, pag);
        atualizaPagina(pag, rrn);

        return SEM_PROMOCAO;
    } else {
        Pagina pagaux = criaPaginaVazia();

        divide(*chavePromovida, *filhoDireita, &pag, chavePromovida, filhoDireita, &pagaux);

        atualizaPagina(pag, rrn);
        adicionaNovaPagina(pagaux);
        return PROMOCAO;
    }
}

void divide(int chave, int filhoDireita, Pagina *pag, int *chavePromovida, int *rrn, Pagina *novaPagina) {
    PaginaAuxiliar paux = copiaPaginaParaAuxiliar(pag);
    int tam = ORDEM_ARVORE_B - 1;

    inseriChavePaginaAuxiliar(chave, filhoDireita, &paux);

    int meio = ORDEM_ARVORE_B / 2;

    resetaPagina(pag);
    resetaPagina(novaPagina);

    for (int i = 0; i < tam; i++) {
        if (i < meio) {
            pag->chaves[i] = paux.chaves[i];
        } else {
            pag->chaves[i] = -1;
        }
    }

    for (int i = 0; i < tam + 1; i++) {
        if (i <= meio) {
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

    *chavePromovida = paux.chaves[meio];

    pag->numeroDeChaves = meio;
    novaPagina->numeroDeChaves = ORDEM_ARVORE_B % 2 == 0 ? meio - 1 : meio;

    *rrn = rnnNovaPagina();
}

void imprimeArvoreB() {
    int raiz = raizArvoreB();

    for (int i = 0; i < quantidadePagina(); i++) {
        Pagina pag = lePaginaPeloRRN(i);
        if (raiz == i) {
            puts("- - - - Pagina Raiz - - - -\n");
        }
        printf("Pagina: %d\n", i);
        exibePagina(pag);
        if (raiz == i) {
            puts("- - - - - - - - - - - - - -\n");
        }
    }

    puts("= = = = = = = = = = = = = =\n");
    printf("raiz: %d\n", raiz);
    printf("quantidade de paginas: %d\n", quantidadePagina());
    puts("= = = = = = = = = = = = = =\n");
}

void mostraChavesOrdenadasArvoreB() {
    printf("Chaves em ordem => ");
    imprimiEmOrdem(raizArvoreB());
    printf("\n");
}

void imprimiEmOrdem(int rrn) {
    if (rrn == -1) return;
    Pagina pagina = lePaginaPeloRRN(rrn);
    int i;

    for (i = 0; i < pagina.numeroDeChaves; i++) {
        imprimiEmOrdem(pagina.filhos[i]);
        printf("%02d - ", pagina.chaves[i]);
    }

    imprimiEmOrdem(pagina.filhos[i]);
}

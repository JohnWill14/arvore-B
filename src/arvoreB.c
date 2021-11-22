/*
 * Autores: 
 *  - John William Vicente <ra118237@uem.br>
 *  - Gabriel Valentim de Oliveira Dacie <ra118419@uem.br>
*/

#include "arvoreB.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "fileUtil.h"
#include "paginaUtil.h"

typedef enum {
    ERRO,
    PROMOCAO,
    SEM_PROMOCAO,
    ENCONTRADO,
    NAO_ENCONTRADO
} Codigos;

Codigos inseriChave(int, int, int *, int *);
void divide(int chave, int filhoDireita, Pagina *pag, int *chavePromovida, int *rrn, Pagina *novaPagina);
void imprimiEmOrdem(int);

void geraArvoreB(char *nomeArquivo) {
    int valorEntrada;
    geraNovaArvore();
    int raiz = raizArvoreB();

    FILE *arquivoDados = abreArquivo(nomeArquivo);

    int chavePromovida, filhoDireita;
    while (leiaIntDasChaves(arquivoDados, &valorEntrada) != EOF) {
        Codigos c = inseriChave(raiz, valorEntrada, &chavePromovida, &filhoDireita);
        if (c == PROMOCAO) {
            Pagina novapag = criaPaginaVazia();
            novapag.chaves[0] = chavePromovida;
            novapag.filhos[0] = raiz;
            novapag.filhos[1] = filhoDireita;
            novapag.numeroDeChaves = 1;
            adicionaNovaPagina(novapag);
            raiz = quantidade() - 1;
            alteraRaizArvoreB(raiz);
        }
    }

    fclose(arquivoDados);
}

void imprimeArvoreB() {
    int raiz = raizArvoreB();

    for (int i = 0; i < quantidade(); i++) {
        Pagina pag = getPaginaPeloRRN(i);
        if (raiz == i) {
            puts("- - - - Pagina Raiz - - - -\n");
        }
        printf("Pagina: %d\n", i);
        exibePagina(pag);
        if (raiz == i) {
            puts("- - - - - - - - - - - - - -\n");
        }
    }
    printf("RAIZ %d\n", raiz);
    printf("quantidadePaginas: %d\n", quantidade());
}

void mostraChavesOrdenadasArvoreB() {
    abreArquivo(ARQUIVO_DADOS);
    imprimiEmOrdem(raizArvoreB());
}

void imprimiEmOrdem(int pag) {
    if (pag < 0) return;
    Pagina pagina = getPaginaPeloRRN(pag);
    int i;
    for (i = 0; i < pagina.numeroDeChaves; i++) {
        imprimiEmOrdem(pagina.filhos[i]);
        printf("%02d | ", pagina.chaves[i]);
    }

    imprimiEmOrdem(pagina.filhos[i]);
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

    if (encontrada) {
        return ERRO;
    }

    Codigos codigo = inseriChave(pag.filhos[pos], chave, chavePromovida, filhoDireita);

    if (codigo == ERRO || codigo == SEM_PROMOCAO) {
        return codigo;
    }

    if (pag.numeroDeChaves < ORDEM_ARVORE_B - 1) {
        pag = inseriNaPagina(*chavePromovida, *filhoDireita, pag);
        atualizaPagina(pag, rrn);

        return SEM_PROMOCAO;
    } else {
        Pagina pagaux = criaPaginaVazia();
        int beatriz;
        divide(*chavePromovida, *filhoDireita, &pag, &beatriz, filhoDireita, &pagaux);
        *chavePromovida = beatriz;

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

    *rrn = quantidade();
}
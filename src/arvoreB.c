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


void divide(int chave, int filhoDireita, Pagina *pag, int *chavePromovida, int *rrn, Pagina *novaPagina);
Codigos inseriChave(int, int, int *, int *);

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
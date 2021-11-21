/*
 * Autores: 
 *  - John William Vicente <ra118237@uem.br>
 *  - Gabriel Valentim de Oliveira Dacie <ra118419@uem.br>
*/

#include "arvoreB.h"

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
void selectionSort(PaginaAuxiliar *pag);
void divide(int chave, int filhoDireita, Pagina pag, int pos, int *RrnNovaPagina, Pagina *novaPag);

void geraArvoreB(char *nomeArquivo) {
    FILE *arquivoDados = abreArquivo(nomeArquivo);
    FILE *arvore = criaArquivoEscrita(ARQUIVO_DADOS);
    fclose(arvore);
    int numero;

    escrevePagina(criaPaginaVazia(), 0);
    Pagina pag = getPaginaPeloRRN(0);
    int a, b;
    while (leiaIntDasChaves(arquivoDados, &numero) != EOF) {
        inseriChave(0, numero, &a, &b);
    }

    fclose(arquivoDados);
}

void imprimeArvoreB() {
}
void mostraChavesOrdenadasArvoreB() {
}

int leiaIntDasChaves(FILE *file, int *num) {
    return fscanf(file, "%d|", num);
}

Codigos inseriChave(int rrn, int chave, int *chavePromovida, int *filhoPromovido) {
    if (rrn < 0) {
        *chavePromovida = chave;
        *filhoPromovido = -1;

        return PROMOCAO;
    }

    int pos = 0;
    Pagina pag = getPaginaPeloRRN(rrn);
    bool encontrada = buscaNaPagina(chave, pag, &pos);

    if (encontrada) {
        return ERRO;
    }

    Codigos codigo = inseriChave(pag.filhos[pos], chave, chavePromovida, filhoPromovido);

    if (codigo == ERRO || codigo == SEM_PROMOCAO) {
        return codigo;
    }

    if (pag.numeroDeChaves < ORDEM_ARVORE_B - 1) {
        pag = inseriNaPagina(chave, *filhoPromovido, pag);
        escrevePagina(pag, rrn);

        return SEM_PROMOCAO;
    } else {
        puts("EFETUA DIVISAO");
        printf("Chave: %d\n", chave);
        printf("chavePromovido: %d\n", *chavePromovida);
        printf("filhoPromovido: %d\n", *filhoPromovido);
        printf("Chave pai: %d\n", pag.chaves[pos]);

        puts("=======");
        printf("Numeros de chaves desse:  bgl %d\n", pag.numeroDeChaves);
        printf("chaves: ");
        for (int i = 0; i < ORDEM_ARVORE_B - 1; i++) {
            printf("%d, ", pag.chaves[i]);
        }
        printf("\nfilhos: ");
        for (int i = 0; i < ORDEM_ARVORE_B; i++) {
            printf("%d, ", pag.filhos[i]);
        }

        puts("\n=======\n");

        Pagina pagaux;
        int rrn;
        divide(chave, *filhoPromovido, pag, pos, &rrn, &pagaux);
    }
}

Pagina criaPaginaVazia() {
    Pagina *pag = (Pagina *)malloc(sizeof(Pagina));
    pag->numeroDeChaves = 0;

    for (int i = 0; i < ORDEM_ARVORE_B - 1; i++) {
        pag->chaves[i] = -1;
    }

    for (int i = 0; i < ORDEM_ARVORE_B ; i++) {
        pag->filhos[i] = -1;
    }

    return *pag;
}

PaginaAuxiliar criaPaginaAuxiliarVazia() {
    PaginaAuxiliar *pag = (PaginaAuxiliar *)malloc(sizeof(Pagina));
    memset(pag->chaves, -1, ORDEM_ARVORE_B - 1);
    memset(pag->filhos, -1, ORDEM_ARVORE_B);

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

Pagina getPaginaPeloRRN(int rrn) {
    FILE *arquiArvoreB = abreArquivo(ARQUIVO_DADOS);
    Pagina pag;

    fseek(arquiArvoreB, byteOffsetApartirDoRRN(rrn), SEEK_SET);
    fread(&pag, sizeof(Pagina), 1, arquiArvoreB);

    fclose(arquiArvoreB);
    return pag;
}

bool buscaNaPagina(int chave, Pagina pag, int *pos) {
    for (int i = 0; i < pag.numeroDeChaves && pag.chaves[i] <= chave; i++) {
        *pos = i;

        if (chave == pag.chaves[i]) {
            return true;
        }
    }

    return false;
}

void divide(int chave, int filhoDireita, Pagina pag, int pos, int *RrnNovaPagina, Pagina *novaPag) {
    PaginaAuxiliar paux;
    for (int i = 0; i < ORDEM_ARVORE_B - 1; i++) {
        printf("%d || %d\n", pag.chaves[i], pag.filhos[i]);
        paux.chaves[i] = pag.chaves[i];
        paux.filhos[i] = pag.filhos[i];
    }
    paux.chaves[ORDEM_ARVORE_B - 1] = chave;
    paux.filhos[ORDEM_ARVORE_B - 1] = filhoDireita;
    //selectionSort(&paux);

    for (int i = 0; i < ORDEM_ARVORE_B; i++) {
        printf("%d | ", paux.chaves[i]);
    }
    puts("\n");
    for (int i = 0; i < ORDEM_ARVORE_B + 1; i++) {
        printf("%d | ", paux.filhos[i]);
    }
    puts("");
}

void selectionSort(PaginaAuxiliar *pag) {
    int min, chaveAux, filhoAux;
    for (int i = 0; i < (ORDEM_ARVORE_B - 1); i++) {
        min = i;
        for (int j = (i + 1); j < ORDEM_ARVORE_B; j++) {
            if (pag->chaves[j] < pag->chaves[min])
                min = j;
        }
        if (i != min) {
            chaveAux = pag->chaves[i];
            filhoAux = pag->filhos[i];

            pag->chaves[i] = pag->chaves[min];
            pag->filhos[i] = pag->filhos[min];

            pag->chaves[min] = chaveAux;
            pag->filhos[min] = filhoAux;
        }
    }
}

FILE *criaArquivoEscrita(char *nomeArquivo) {
    /*
        Ambos r+e w+podem ler e gravar em um arquivo. No entanto, r+ não exclui o conteúdo do
        arquivo e não cria um novo arquivo se tal arquivo não existir, enquanto w+ exclui o
        conteúdo do arquivo e o cria se ele não existir.

        URL: https://stackoverflow.com/questions/21113919/difference-between-r-and-w-in-fopen
    */
    FILE *arquivo = fopen(nomeArquivo, "w+");

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
    return rrn * sizeof(Pagina);
}
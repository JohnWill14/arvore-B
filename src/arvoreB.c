/*
 * Autores: 
 *  - John William Vicente <ra118237@uem.br>
 *  - Gabriel Valentim de Oliveira Dacie <ra118419@uem.br>
*/

#include "arvoreB.h"

#include <stdbool.h>
#include <stdio.h>

typedef enum {
    ERRO,
    PROMOCAO,
    SEM_PROMOCAO,
    ENCONTRADO,
    NAO_ENCONTRADO
} Codigos;

bool busca(int, int, int *, int *);
bool buscaNaPagina(int, Pagina, int *);
Pagina getPag(int, FILE *);
int leiaIntDasChaves(FILE *, int *);
Pagina inseriNaPagina(int, int, Pagina);
void escrevePagina(Pagina, int);
FILE *criaArquivoEscrita(char *);
FILE *abreArquivo(char *);
int byteOffsetApartirDoRRN(int);

void geraArvoreB(char *nomeArquivo) {
    FILE *arquivoDados = abreArquivo(nomeArquivo);
    int numero;

    while (leiaIntDasChaves(arquivoDados, &numero) != EOF) {
        printf("*** %d\n", numero);
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

// Codigos inseriChave(int rrn, int chave, int *filhoDireito, int *chavePromovida) {
//     if (rrn < 0) {
//         *chavePromovida = chave;
//         *filhoDireito = -1;

//         return PROMOCAO;
//     }
//     Pagina pag;
//     FILE *arquiArvoreB = abreArquivo(ARQUIVO_DADOS);
//     int pos;

//     pag = getPag(rrn, arquiArvoreB);
//     fclose(arquiArvoreB);

//     bool encontrado = buscaNaPagina(chave, pag, &pos);

//     if (encontrado) {
//         fprintf(stderr, "Chave repetida !!!\n");
//         return ERRO;
//     }

//     int *rrnPromovido;

//     Codigos retorno = inseriChave(pag.filhos[pos], chave, rrnPromovido, chavePromovida);

//     if (retorno == SEM_PROMOCAO || retorno == ERRO) {
//         return retorno;
//     }
// // vou conseguir inserir
//     if (pag.numeroDeChaves < ORDEM_ARVORE_B - 1) {
//         pag = inseriNaPagina(chave, *rrnPromovido, pag);

//         escrevePagina(pag, rrn);

//         return SEM_PROMOCAO;
//     } else {
//        escrevePagina(pag, rrn);
//     }
// }

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
}

bool busca(int rrn, int chave, int *rrn_encontrado, int *pos_encontrada) {
    if (rrn < 0) {
        return false;
    }
    Pagina pag;
    FILE *arquiArvoreB = abreArquivo(ARQUIVO_DADOS);
    int pos;

    pag = getPag(rrn, arquiArvoreB);
    fclose(arquiArvoreB);

    bool encontrado = buscaNaPagina(chave, pag, &pos);

    if (encontrado) {
        *rrn_encontrado = rrn;
        *pos_encontrada = pos;

        return true;
    }

    return busca(pag.filhos[pos], chave, rrn_encontrado, pos_encontrada);
}

Pagina getPag(int rrn, FILE *file) {
    Pagina pag;

    fseek(file, byteOffsetApartirDoRRN(rrn), SEEK_SET);
    fread(&pag, sizeof(Pagina), 1, file);

    return pag;
}

bool buscaNaPagina(int chave, Pagina pag, int *pos) {
    for (int i = 0; i < pag.numeroDeChaves && pag.chaves[i] < chave; i++) {
        *pos = i;

        if (i < pag.numeroDeChaves && chave == pag.chaves[i]) {
            return true;
        }
    }

    return false;
}

void divide(){

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
    return sizeof(int) + rrn * sizeof(Pagina);
}
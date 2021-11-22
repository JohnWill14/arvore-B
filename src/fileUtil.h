#ifndef _FILE_UTIL_H
#define _FILE_UTIL_H

#include <stdio.h>
#include <stdlib.h>

FILE *criaArquivoEscrita(char *);
FILE *abreArquivo(char *);

int leiaIntDasChaves(FILE *, int *);

int leiaIntDasChaves(FILE *file, int *num) {
    return fscanf(file, "%d|", num);
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

#endif
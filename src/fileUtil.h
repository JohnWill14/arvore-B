#ifndef _FILE_UTIL_H
#define _FILE_UTIL_H

#include <stdio.h>
#include <stdlib.h>

FILE *criaArquivoEscrita(char *);
FILE *abreArquivo(char *);

int raizArvoreB();
int quantidadePagina();
int alteraRaizArvoreB(int);
int alteraQuantidade(int);
int leiaIntDasChaves(FILE *, int *);
int byteOffsetApartirDoRRN(int);

int raizArvoreB() {
    int raizArvoreB;
    FILE *file = abreArquivo(ARQUIVO_DADOS);
    fread(&raizArvoreB, sizeof(int), 1, file);
    fclose(file);
    return raizArvoreB;
}

int quantidadePagina() {
    int qtd;
    FILE *file = abreArquivo(ARQUIVO_DADOS);
    fseek(file, sizeof(int), SEEK_SET);
    fread(&qtd, sizeof(int), 1, file);
    fclose(file);
    return qtd;
}

int alteraRaizArvoreB(int valor) {
    FILE *file = abreArquivo(ARQUIVO_DADOS);
    fwrite(&valor, sizeof(int), 1, file);
    fclose(file);
    return valor;
}

int alteraQuantidade(int valor) {
    FILE *file = abreArquivo(ARQUIVO_DADOS);
    fseek(file, sizeof(int), SEEK_SET);
    fwrite(&valor, sizeof(int), 1, file);
    fclose(file);
    return valor;
}

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
    return 2 * sizeof(int) + (rrn * sizeof(Pagina));
}

#endif
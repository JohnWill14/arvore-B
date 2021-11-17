/*
 * Autores: 
 *  - JOHN WILLIAM Vicente <ra118237@uem.br
 *  - Gabriel Valientim de Oliveira Dacie
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool stringEquals(char*, char*);

int main(int argc, char* argv[]) {
    if (argc == 3 && stringEquals(argv[1], "-c")) {
        puts("Modo criacao");

    } else if (argc == 2 && stringEquals(argv[1], "-p")) {
        puts("Modo Informação");

    } else if (argc == 2 && stringEquals(argv[1], "-k")) {
        puts("Modo Impressao");

    } else {
        fprintf(stderr, "Argumentos incorretos!\n");
        fprintf(stderr, "Modo de uso:\n");
        fprintf(stderr, "$ %s (-c) nome_arquivo\n", argv[0]);
        fprintf(stderr, "$ %s (-p|-k)\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    return 0;
}

bool stringEquals(char* str, char* str2) {
    return strcmp(str, str2) == 0;
}
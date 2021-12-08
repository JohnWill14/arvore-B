/*
 * Autores:
 *  - John William Vicente <ra118237@uem.br>
 *  - Gabriel Valentim de Oliveira Dacie <ra118419@uem.br>
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operacoesArvoreB.c"

bool stringEquals(char*, char*);

/* Querida professora;
 Quando eu e o john escrevemos esse código,
 apenas nos e Deus sabíamos como ele funcionava.
 Agora, apenas Deus sabe!

 Então, se você encontrar alguma gambiarra e com certeza vai
 apenas confia nelas.
 total de gambiarras: no minimo 78
 Obs: perdemos a conta 😨😨😨
*/

int main(int argc, char* argv[]) {
    if (argc == 3 && stringEquals(argv[1], "-c")) {
        puts("Modo criacao");
        geraArvoreB(argv[2]);
        puts("Sucesso !!!");
    } else if (argc == 2 && stringEquals(argv[1], "-p")) {
        puts("Modo Impressao");
        imprimeArvoreB();
    } else if (argc == 2 && stringEquals(argv[1], "-k")) {
        puts("Modo chaves ordenadas");
        mostraChavesOrdenadasArvoreB();
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
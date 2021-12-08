/*
 * Autores: 
 *  - John William Vicente <ra118237@uem.br>
 *  - Gabriel Valentim de Oliveira Dacie <ra118419@uem.br>
*/

#ifndef _ARVORE_B_
#define _ARVORE_B_

#define ARQUIVO_DADOS "btree.dat"
#define ORDEM_ARVORE_B 5

typedef struct {
    int numeroDeChaves;             /* número de chaves na página */
    int chaves[ORDEM_ARVORE_B - 1]; /* vetor que armazena as chaves */
    int filhos[ORDEM_ARVORE_B];     /* RRNs dos filhos */
} Pagina;

typedef struct {
    int numeroDeChaves;             /* número de chaves na página */
    int chaves[ORDEM_ARVORE_B];     /* vetor que armazena as chaves */
    int filhos[ORDEM_ARVORE_B + 1]; /* RRNs dos filhos */
} PaginaAuxiliar;


#endif
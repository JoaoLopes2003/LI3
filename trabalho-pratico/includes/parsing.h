#ifndef _PARSING_H_
#define _PARSING_H_
#include <stdio.h>



// Funcoes relativas ao parsing dos dados


    // Guarda num array de void pointers o número de void pointers alocados (posição 0 do array),
    // o número de linhas do ficheiro sem contar com o cabeçalho (na posição 1 do array) e
    // todas as linhas do ficheiro (a partir da posição 2 do array)

void **parsing(FILE *fp);

    // Liberta todo o espaço alocado pela função parsing

void libertaParsing (void **arrayLinhas);


#endif
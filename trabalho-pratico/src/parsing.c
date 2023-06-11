#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// Funcoes relativas ao parsing dos dados


    // Guarda num array de void pointers o número de void pointers alocados (posição 0 do array),
    // o número de linhas do ficheiro sem contar com o cabeçalho (na posição 1 do array) e
    // todas as linhas do ficheiro (a partir da posição 2 do array)

void **parsing(FILE *fp)
{
    // Guarda inicialmente as strings a partrir da posição dois do array e no final da função vai ser o número de linhas guardadas no array

    int *sp = (int*) malloc(sizeof(int));
    (*sp) = 2;

    int *size = (int*) malloc(sizeof(int));
    *size = 10002;

    void **arrayLinhas = malloc(10002 * sizeof(void*));

    char* line = NULL;
    ssize_t read;
    size_t len;

    // Salta a primeira linha do ficheiro.
    
    read = getline(&line, &len, fp);

    // Guarda todas as linhas do ficheiro no array de void pointers

    while (((read = getline(&line, &len, fp)) != -1))
    {
        if ((*sp) == (*size))
        {
            (*size) *= 2;
            arrayLinhas = realloc (arrayLinhas, (*size) * sizeof(void*));
        }
        
        arrayLinhas[(*sp)] = strdup (line);
        (*sp)++;
    }

    free (line);

    arrayLinhas[0] = (void*) size;

    // Guarda o número de linhas lidas (sem contar com o cabeçalho)
    // na posição 1 do array de apontadores para void

    (*sp) -= 2;
    arrayLinhas[1] = (void*) sp;

    fclose(fp);

    return arrayLinhas;
}

// Liberta todo o espaço alocado pela função parsing

void libertaParsing (void **arrayLinhas)
{
    int i = (*((int*)arrayLinhas[1]))+1;

    for (;i>=0;i--)
    {
        free(arrayLinhas[i]);
    }

    free(arrayLinhas);
}

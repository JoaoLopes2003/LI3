#ifndef _OUTPUTS_H_
#define _OUTPUTS_H_
#include <stdio.h>
#include "catalogo.h"



// Funcoes ressponsaveis por executar a querie correta


    // Estrutura que guarda a linha de input

typedef struct query *QUERIE;

    // Executa a querie pedida

void processalinha (QUERIE querie, void *ficheiro,Catalogos c,bool batchMode);

    // Processa uma linha de input, organizando os dados

QUERIE parselinha(char *line);

    // Função que liberta o espaço alocado para uma querie

void libertaQuerie (QUERIE q);

    // Devolve o numero da querie

short int get_querie_num(QUERIE l);


#endif
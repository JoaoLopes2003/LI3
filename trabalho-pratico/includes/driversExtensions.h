#ifndef _DRIVERSEXTENSIONS_H_
#define _DRIVERSEXTENSIONS_H_
#include "drivers.h"
#include "rides.h"



// Estrutura para a querie 2


    // Estrutura auxiliar para guardar a informacao de um driver

typedef struct driversClass **DriversClass;

    // Funcao que percorre todos os drivers e cria um array ordenado por ordem decrescente de classificacao media de drivers ativos

DriversClass arrayDriversClassOrd (Drivers arrayD, Trips arrayT, int nDrivers, int nRides);

    // Liberta a memoria alocada para a estrutura da querie 2

void libertaArrayDriversOrd (DriversClass arrayOrdenado);

    // Devolve o id do driver numa determinada posicao do array

int getIdArrDClass (DriversClass arrDClass, int position);

    // Devolve a classificacao media do driver numa determinada posicao do array

double getClassArrDClass (DriversClass arrDClass, int position);


#endif
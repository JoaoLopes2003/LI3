#include "driversExtensions.h"
#include <stdio.h>




// Estrutura para a querie 2


    // Estrutura auxiliar para guardar a informacao de um driver

typedef struct driversClassAux
{
    int id;                             // Id do driver
    double class;                       // Classificacao media
    unsigned short int lastTrip;        // Data da ultima viagem
} *DriversClassAux;

    // Estrutura final para guardar a informacao de um driver

struct driversClass
{
    int id;                             // Id do driver
    double class;                       // Classificacao media
};

static int tamanhoArray;

    // Funcao que determina que o array de drivers vai ser ordenado por ordem decrescente de classificacao media

static int comparatorDrivers (const void *p, const void *q)
{
    DriversClassAux l = *(DriversClassAux *) p;
    DriversClassAux r = *(DriversClassAux *) q;

    if (l->class < r->class)
    {
        return 1;
    }
    else if (l->class > r->class)
    {
        return -1;
    }
    else
    {
        if (l->lastTrip > r->lastTrip)
        {
            return -1;
        }
        else if (l->lastTrip<r->lastTrip)
        {
            return 1;
        }
        else
        {
            if (l->id>r->id)
            {
                return 1;
            }
            return -1;
        }
    }
}

    // Funcao que copia apenas a informacao necessaria de um array para outro, que ocupa apenas o espaço de memória necessario

static void copyArray (DriversClassAux *arrOrigin,DriversClass arrDest, int nDrivers)
{
    for (int i = 0;i<nDrivers;i++)
    {
        arrDest[i]->id = arrOrigin[i]->id;
        arrDest[i]->class = arrOrigin[i]->class;
    }
}

    // Funcao que percorre todos os drivers e cria um array ordenado por ordem decrescente de classificacao media de drivers ativos

DriversClass arrayDriversClassOrd (Drivers arrayD, Trips arrayT, int nDrivers, int nRides)
{
    DriversClassAux *arrayDriversAux = malloc(nDrivers * sizeof(DriversClassAux));

    for (int i = 0;i<nDrivers;i++) arrayDriversAux[i] = malloc(sizeof(struct driversClassAux));

    int i, id, driversValidos;

    for (id = 1,driversValidos = 0;id<=nDrivers;id++)
    {
        if (existeDriver(arrayD,id) && getArrDriversStatus(arrayD,id))
        {
            arrayDriversAux[driversValidos]->id = id;
            arrayDriversAux[driversValidos]->class = classificationDrivers(arrayT,arrayD,id);
            arrayDriversAux[driversValidos]->lastTrip = getArrDriversLastTrip (arrayD, id);
            driversValidos++;

            // percorrer todas as viagens do driver
        }

    }
    
    tamanhoArray = driversValidos;

    // Ordenar os elementos por ordem decrescente (QSort).

    qsort ((void*)arrayDriversAux, driversValidos, sizeof(arrayDriversAux[0]), comparatorDrivers);

    DriversClass arrayDrivers = malloc(driversValidos * sizeof(DriversClass));

    for (i = 0;i<driversValidos;i++)
    {
        arrayDrivers[i] = malloc (sizeof(struct driversClass));
    }

    copyArray (arrayDriversAux, arrayDrivers, driversValidos);

    for (int i = 0;i<nDrivers;i++)
    {
        free (arrayDriversAux[i]);
    }

    free (arrayDriversAux);

    return arrayDrivers;
}

    // Liberta a memoria alocada para a estrutura da querie 2

void libertaArrayDriversOrd (DriversClass arrayOrdenado)
{
    for (int i =0;i < tamanhoArray;i++)
    {
        free (arrayOrdenado[i]);
    }
    free(arrayOrdenado);
}

    // Devolve o id do driver numa determinada posicao do array

int getIdArrDClass (DriversClass arrDClass, int position)
{
    return arrDClass[position]->id;
}

    // Devolve a classificacao media do driver numa determinada posicao do array

double getClassArrDClass (DriversClass arrDClass, int position)
{
    return arrDClass[position]->class;
}

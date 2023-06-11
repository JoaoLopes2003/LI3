#ifndef _CATALOGO_H_
#define _CATALOGO_H_
#include <string.h>
#include <glib.h>
#include <stdio.h>
#include "drivers.h"
#include "driversExtensions.h"
#include "usersExtensions.h"
#include "ridesExtensions.h"



// Funcoes relativas ao catalogo


    // Estrutura do catalogo que guarda todos os dados em varias estruturas

typedef struct catalogos *Catalogos;

    // Funcao que inicializa e preenche o catalogo todo

Catalogos initCatalogos (char *pastaCsv);

    // Liberta a memoria alocada para o catalogo e todas as suas estruturas

void libertaCatalogo(Catalogos catalogo);



// Funcoes para aceder aos elementos do catalogo


    // Devolve o numero de drivers (validos e invalidos)

int numberDrivers(Catalogos c);

    // Devolve o numero de viagens (validas e invalidas)

int numberTrips(Catalogos c);

    // Devolve o array dos drivers

Drivers getArrDrivers (Catalogos c);

    // Devolve a hashTable dos users

Users getHashUsers (Catalogos c); 

    // Devolve o array dos drivers ordenado por classificacao

DriversClass getArrDClass (Catalogos c);

    // Devolve o array dos users ordenado por distancia total percorrida

UsersDist getArrUDist (Catalogos c);

    // Devolve o array das viagens

Trips getArrTrips (Catalogos c);

    // Devolve a hashTable das cidades, que a cada cidade corresponde um array de drivers ordenado por classificacao

OrdScoreDriversByCity getOrdScoreDriversByCity (Catalogos c);

    // Devolve a estrutura com dois arrays de viagens, um apenas com viagens com driver e user do sexo masculino e
    // outro do feminino ordenados por data de criacao da conta

DriversUsersSameGender getDriversUsersSameGender (Catalogos c);

    // Devolve a hashTable das cidades, que a cada cidade corresponde um array de viagens ordenado por data da viagem

CityOrdByDate getCityOrdByDate (Catalogos c);


#endif
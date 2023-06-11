#ifndef _RIDES_H_
#define _RIDES_H_
#include "users.h"
#include "drivers.h"



// Funcoes relativas à estrutura base das viagens


    // Estrutura para guardar a informacao de uma viagem

typedef struct trips **Trips;

    // Funcao que recebe o array das linhas do ficheiro csv das rides e organiza os dados das viagens validas num array,
    // em que a posicao do array corresponde ao id da viagem -1

Trips pushTrips (void **arrayLines, Drivers arrayDrivers, Users users, int *NTrips);

    // Liberta a memória alocada para o array de viagens

void libertaARRTrips (Trips arrayT, int size);



// Funções para obter informacoes da estrutura das viagens


    // Devolve a classificacao do driver

double getClassTD (Trips arrayT, int id);

    // Devolve o id do driver

int getDriverT (Trips arrayT, int id);

    // Devolve o username do user

char *getUserT (Trips arrayT, int id);

    // Devolve a cidade

char *getCity (Trips arrayT, int id);

    // Devolve o genero do driver

char getGenderDriverTrips (Trips arrayT, Drivers drivers, int id);

    // Devolve o genero do user

char getGenderUserTrips (Trips arrayT, Users users, int id);

    // Devolve a data da viagem

unsigned short int getDateArrTrips (Trips arrayT, int id);

    // Devolve a distancia percorrida

int getDistTrips (Trips arrayT, int id);

    // Indica se o user deu gorjeta

int getArrTripsTipVerify (Trips arrayT, int id);

    // Devolve a gorjeta

double getArrTripsTip (Trips arrayT, int id);

    // Verifica se a viagem existe

bool verifyTripNULL (Trips arrayT, int id);



// Funções para calcular informacoes sobre as viagens


    // Funcao que calcula o preço de uma viagem

double preco (int distance, char classe);

    // Funcao que calcula o total ganho por um driver

double totalSpentDrivers (Trips arrayTrips, Drivers drivers, int id);

    // Função que calcula a classificação média de todas as viagens de um driver

double classificationDrivers (Trips arrayTrips, Drivers drivers, int id);

    // Funcao que calcula o total gasto por um user

double totalSpentUsers (Trips arrayTrips, Users users, Drivers drivers, char *username);

    // Funcao que calcula a distancia total percorrida por um user

int totalDistanceUsers (Trips arrayTrips, Users users, char *username);

    // Função que calcula a classificação média de todas as viagens de um user

double classificationUsers (Trips arrayTrips, Users users, char *username);


#endif
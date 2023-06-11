#ifndef _VIAGENSEXTENSIONS_H_
#define _VIAGENSEXTENSIONS_H_
#include "rides.h"
#include <stdio.h>
#include <string.h>



// Querie 7


    // Estrutura que armazena os dados da querie 7

typedef struct ordScoreDriversByCity *OrdScoreDriversByCity;

    // Liberta a memoria alocada para a estrutura da querie 7

void libertaOrdScoreDriversByCity (OrdScoreDriversByCity ordScoreDriversByCity);

    // Funcao que percorre o array das rides e cria uma hashTable em que as keys são as cidades e a cada cidade está associado
    // um array de drivers ordenado por ordem decrescente de classificacao

OrdScoreDriversByCity hashScoreDriversbyCity (Drivers arrayD, Trips arrayT, int nDrivers, int nRides);

    // Funcao que quando chamada a querie 7, percorre o array ordenado de drivers e imprime os N primeiros num ficheiro

void printTopDriversByCity (OrdScoreDriversByCity ordScoreDriversByCity, Drivers arrayrivers, void *output, int N, char *city,bool batchMode);



// Querie 8


    // Estrutura que armazena os dados da querie 8

typedef struct driversUsersSameGender *DriversUsersSameGender;

    // Liberta a memoria alocada para a estrutura da querie 8

void libertaDriversUsersSameGender (DriversUsersSameGender driversUsersSameGender);

    // Funcao que percorre o array das rides e cria uma estrutura com dois arrays, um correspondente apenas a elementos
    // do sexo masculino e outro apenas a elementos do sexo feminino, em que para pertencer ao array é necessário numa
    // viagem o driver e user serem do mesmo genero. Este array é ainda ordenado por ordem crescente de data de criacao
    // da conta, estando nas primeiras posicoes elementos com data de criacao mais antiga

DriversUsersSameGender arrayDriversUsersSameGender (Drivers arrayD, Trips arrayT, Users users, int nRides);

    // Funcao que quando chamada a querie 8, percorre o array ordenado de elementos do mesmo sexo e imprime todos os elementos com ano de
    // criacao inferior ao recebido como argumento num ficheiro

void printArrayOrdGender (void *output, DriversUsersSameGender driversUsersSameGender, Drivers drivers, Users users, char gender, int year ,bool batchMode);



// Estrutura para as queries 4, 5, 6 e 9

    // Estrutura

typedef struct cityOrdByDate *CityOrdByDate;

    // Liberta a memoria alocada para a estrutura da querie 4, 5, 6 e 9

void libertaCityOrdByDate (CityOrdByDate ordCitiesByDate);

    // Funcao que percorre o array das rides e cria uma hashTable em que as keys são as cidades e a cada cidade está associado
    // um array de ids de viagens ordenado da viagem vais antiga para a mais recente

CityOrdByDate ordCitiesByDate (Trips arrayT, int nTrips);



// Querie 4

    // Funcao que quando chamada a querie 4, percorre o array todo de viagens de uma cidade ordenado da viagem mais antiga para a mais recente e
    // imprime o preço médio das viagens nessa cidade num ficheiro

void printAveragePriceCity (Trips rides_arr, Drivers drivers_arr, CityOrdByDate cityOrdByDate, char *city_name, void *output ,bool batchMode);



// Querie 5


    // Funcao que quando chamada a querie 5, percorre os array de viagens de todas as cidades ordenados da viagem mais antiga para a mais recente e
    // imprime o preço médio das viagens num determinado intervalo de tempo num ficheiro

void printfTipsOrsByDistance (CityOrdByDate cityOrdByDate, Trips arrayT, unsigned short int startDate, unsigned short int limitDate, void *output ,bool batchMode);



// Querie 6


    // Funcao que quando chamada a querie 6, percorre o array de viagens de uma cidade ordenado da viagem mais antiga para a mais recente e
    // imprime a distancia media percorrida num determinado intervalo de tempo nessa cidade num ficheiro

void printAveragePriceByDate (Trips rides_arr, Drivers drivers_arr, CityOrdByDate cityOrdByDate, unsigned short int s_date, unsigned short int b_date, void *output ,bool batchMode);



// Querie 9


    // Funcao que quando chamada a querie 9, cria um array com todas as viagens realizadas no intervalo dado como argumento, ordena esse
    // array por ordem decrescente de distancias percorridas e em caso de empate a aparecer primeiro as viagens mais recentes e
    // imprime o array inteiro da primeira posicao até à ultima num ficheiro

void printAverageDistanceByDate (Trips rides_arr, CityOrdByDate cityOrdByDate, char *city_name, unsigned short int s_date, unsigned short int b_date, void *output ,bool batchMode);


#endif
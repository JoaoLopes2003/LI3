#include "catalogo.h"
#include "parsing.h"
#include "rides.h"
#include "users.h"
#include "ridesExtensions.h"




// Funcoes relativas ao catalogo


    // CSVS

#define CSVD "/drivers.csv"
#define CSVU "/users.csv"
#define CSVR "/rides.csv"

    // Estrutura do catalogo que guarda todos os dados em varias estruturas

struct catalogos
{
    int sizeArrDrivers;                                         // Numero de drivers (validos e invalidos)
    int sizeArrTrips;                                           // Numero de viagens (validas e invalidas)

    Users users;                                     // HashTable dos users
    Drivers arrDrivers;                                         // Array dos drivers
    Trips arrTrips;                                             // Array das viagens
    DriversClass arrDClass;                                     // Array dos drivers ordenado por classificacao
    UsersDist arrUDist;                                         // Array dos users ordenado por distancia total percorrida
    OrdScoreDriversByCity ordScoreDriversByCity;                // HashTable das cidades, que a cada cidade corresponde um array de drivers ordenado por classificacao
    DriversUsersSameGender driversUsersSameGender;              // Estrutura com dois arrays de viagens, um apenas com viagens com driver e user do sexo masculino e outro do feminino, ordenados por data de criacao da conta
    CityOrdByDate cityOrdByDate;                                // HashTable das cidades, que a cada cidade corresponde um array de viagens ordenado por data da viagem
};

    // Funcao que inicializa e preenche o catalogo todo

Catalogos initCatalogos (char *pastaCsv)
{
    
    int t = strlen(pastaCsv) + 1;
    char *csvDrivers, *csvUsers, *csvRides;
    csvDrivers = malloc(t + strlen(CSVD));
    csvUsers = malloc(t + strlen(CSVU));
    csvRides = malloc(t + strlen(CSVR));

    // Apartir da pasta dos csv, cria os caminhos para abrir cada ficheiro csv

    strcpy(csvDrivers, pastaCsv);
    strcpy(csvUsers, pastaCsv);
    strcpy(csvRides, pastaCsv);

    strcat(csvDrivers, CSVD);
    strcat(csvUsers, CSVU);
    strcat(csvRides, CSVR);

    FILE *usersFile,*driversFile,*ridesFile;
    usersFile = fopen(csvUsers,"r");
    driversFile = fopen(csvDrivers,"r");
    ridesFile = fopen(csvRides,"r");

    // Verifica se o caminho para os ficheiros de dados é valido, se não for liberta a memória e retorna NULL
    
    if (usersFile ==NULL || driversFile == NULL ||  ridesFile == NULL)
    {
        // fechar os ficheiros que foram abertos e libertar o nome dos ficheiros que se tentou abrir
        if (usersFile != NULL) fclose(usersFile);

        if (driversFile != NULL) fclose(driversFile);

        if (ridesFile != NULL) fclose(ridesFile);


        free(csvDrivers);
        free(csvUsers);
        free(csvRides);

        return NULL;

    }

    Catalogos r = malloc(sizeof(struct catalogos));

    int *numberDrivers = malloc(sizeof(int));
    int *numberRides = malloc(sizeof(int));

    // Guarda as linhas dos ficheiros e organiza os dados guardados

    void **arrayLinesDrivers = parsing (driversFile);
    r->arrDrivers = pushDrivers (arrayLinesDrivers, numberDrivers);

    void **arrayLinesUsers = parsing (usersFile);
    r->users = pushUsers (arrayLinesUsers);

    void **arrayLinesRides = parsing (ridesFile);
    r->arrTrips = pushTrips (arrayLinesRides, r->arrDrivers, r->users, numberRides);


    r->sizeArrDrivers = *numberDrivers;
    r->sizeArrTrips = *numberRides;


    r->arrUDist = arrayUsersDistOrd (r->users, r->arrTrips, *numberRides);
    r->arrDClass = arrayDriversClassOrd (r->arrDrivers, r->arrTrips, *numberDrivers, *numberRides);
    r->ordScoreDriversByCity = hashScoreDriversbyCity (r->arrDrivers, r->arrTrips, *numberDrivers, *numberRides);
    r->driversUsersSameGender = arrayDriversUsersSameGender (r->arrDrivers, r->arrTrips, r->users, *numberRides);
    r->cityOrdByDate = ordCitiesByDate (r->arrTrips, *numberRides);

    // Liberta o espaço para os caminhos dos ficheiros csv especificos e fecha os ficheiros

    free(csvDrivers);
    free(csvUsers);
    free(csvRides);
    free(numberDrivers);
    free(numberRides);

    return r;
}

    // Liberta a memoria alocada para o catalogo e todas as suas estruturas

void libertaCatalogo(Catalogos c)
{
    libertaArrDrivers (c->arrDrivers,c->sizeArrDrivers);
    libertaARRTrips (c->arrTrips,c->sizeArrTrips);
    libertaUsers (c->users);
    libertaArrayDriversOrd (c->arrDClass);
    libertaArrUsersOrd (c->arrUDist);
    libertaOrdScoreDriversByCity (c->ordScoreDriversByCity);
    libertaDriversUsersSameGender (c->driversUsersSameGender);
    libertaCityOrdByDate (c->cityOrdByDate);

    free(c);
}





// Funcoes para aceder aos elementos do catalogo


    // Devolve o numero de drivers (validos e invalidos)

int numberDrivers (Catalogos c)
{
    return c->sizeArrDrivers;
}

    // Devolve o numero de viagens (validas e invalidas)

int numberTrips (Catalogos c)
{
    return c->sizeArrTrips;
}

    // Devolve o array dos drivers

Drivers getArrDrivers (Catalogos c)
{
    return (c->arrDrivers);
}

    // Devolve o array dos drivers ordenado por classificacao

DriversClass getArrDClass (Catalogos c)
{
    return (c->arrDClass);
}

    // Devolve a hashTable dos users

Users getHashUsers (Catalogos c)
{
    return c->users;
}

    // Devolve o array dos users ordenado por distancia total percorrida

UsersDist getArrUDist (Catalogos c)
{
    return (c->arrUDist);
}

    // Devolve o array das viagens

Trips getArrTrips (Catalogos c)
{
    return (c->arrTrips);
}

    // Devolve a hashTable das cidades, que a cada cidade corresponde um array de drivers ordenado por classificacao

OrdScoreDriversByCity getOrdScoreDriversByCity (Catalogos c)
{
    return (c->ordScoreDriversByCity);
}

    // Devolve a estrutura com dois arrays de viagens, um apenas com viagens com driver e user do sexo masculino e
    // outro do feminino ordenados por data de criacao da conta

DriversUsersSameGender getDriversUsersSameGender (Catalogos c)
{
    return (c->driversUsersSameGender);
}

    // Devolve a hashTable das cidades, que a cada cidade corresponde um array de viagens ordenado por data da viagem

CityOrdByDate getCityOrdByDate (Catalogos c)
{
    return (c->cityOrdByDate);
}

#include "rides.h"
#include "auxiliares.h"
#include "parsing.h"
#include "drivers.h"
#include <stdio.h>




// Funcoes relativas à estrutura base das viagens


    // Tipos de automoveis

#define Basic 'b'
#define Green 'b'
#define Premium 'b'

    // Estrutura para guardar a informacao de uma viagem

struct trips
{
    unsigned short int date;                // Data
    int driver;                             // Id do driver
    char *user;                             // Username do user
    char *city;                             // Cidade
    unsigned int distance;                  // distancia percorrida
    unsigned char scoreUser;                // Avaliacao user
    unsigned char scoreDriver;              // Avaliacao Driver
    double tip;                             // Gorjeta
};

    // Verifica se uma viagem é valida

static bool tripCheck(char *id,char *idDriver, char *user,char *city,char *data,unsigned short int *tripDate,int distance ,char scoreUser, char scoreDriver,double tip)
{
    // verificar se existem campos vazios

    if (id == NULL || id[0] == '\0' || user == NULL || user[0] == '\0' || idDriver == NULL || idDriver[0] == '\0' ||  city == NULL || city[0] == '\0' ) return false;

    // verificar se os restantes campos são validos

    bool vality = (scoreUser && scoreDriver && distance  && tip != -1 && pushData(data,tripDate));

    return vality;
}

    // Funcao que recebe o array das linhas do ficheiro csv das rides e organiza os dados das viagens validas num array,
    // em que a posicao do array corresponde ao id da viagem -1

Trips pushTrips (void **arrayLines, Drivers arrayDrivers, Users users, int *NTrips)
{
    int sizeArray, lastId, positionAux = 1;

    // Número de viagens a serem guardadas
    
    int spArray = *((int *)arrayLines[1]);
    
    char *lastIdString = strdup ((char*)arrayLines[spArray+2-positionAux]);
    lastIdString = strsep(&lastIdString,";");

    while (!sscanf(lastIdString, "%d", &lastId))
    {
        positionAux++;
        free (lastIdString);
        lastIdString = strdup ((char*)arrayLines[spArray+2-positionAux]);
        lastIdString = strsep(&lastIdString,";");
    }

    sizeArray = lastId + positionAux - 1;
    free (lastIdString);

    *NTrips = sizeArray;

    Trips arrayTrips = malloc(sizeArray * sizeof(Trips));
    
    for (int i = 0; i < sizeArray; i++) arrayTrips[i] = NULL;

    unsigned short int data;
    int dist,idTrip;
    char scoreDriver,scoreUser;
    double tip;
    char *username,*id,*dataStr,*idDriver,*copy,*start,*token,*city;
    
    for (int i = 0;i<spArray; i++)
    {
        copy = strdup (arrayLines[i + 2]);

        // apontador para o inicio da string duplicada, para dps ser possível libertar a memóiria reservada
        start = copy;

        id = strsep(&copy,";");

        // Guarda a data da viagem
        dataStr = strsep(&copy,";");

        // Guarda o id do condutor da viagem
        idDriver = strsep(&copy,";");

        // Guarda o username do utilizador que realizou a viagem
        username = strsep(&copy,";");

        // Guarda a cidade onde a viagem foi realizada
        city = strsep(&copy,";");

        // Guarda a distância viajada na viagem
        token = strsep(&copy,";");
        dist = intMaiorQue0(token);

        // Guarda a avaliação do utilizador
        token = strsep(&copy,";");
        scoreUser = intMaiorQue0(token);

        // Guarda a avaliação do condutor
        token = strsep(&copy,";");
        scoreDriver = intMaiorQue0(token);
        
        // Guarda o valor da gorjeta
        token = strsep(&copy,";\n");
        tip = pushTip(token);

        // Não é necessário guardar a informação do comentário

        if (tripCheck(id,idDriver,username,city,dataStr,&data,dist,scoreUser,scoreDriver,tip))
        {

            idTrip = atoi(id)-1;
            arrayTrips[idTrip] = malloc(sizeof(struct trips));
            arrayTrips[idTrip]->city = strdup(city);
            arrayTrips[idTrip]->date = data;
            arrayTrips[idTrip]->distance = dist;
            int idD = atoi(idDriver);
            arrayTrips[idTrip]->driver = idD;
            arrayTrips[idTrip]->user = strdup(username);
            arrayTrips[idTrip]->scoreDriver = scoreDriver;
            arrayTrips[idTrip]->scoreUser = scoreUser;
            arrayTrips[idTrip]->tip = tip;
            pushTrips_User (users,username,idTrip+1,data);
            pushViagens_Driver(arrayDrivers,idTrip+1,idD,data);
        }
        else
        {
            //printf("Invalida %s ", (char *) arrayLines[i + 2]);
        }

        free (start);
    }

    libertaParsing (arrayLines);


    return arrayTrips;
}

// Liberta a memória alocada para o array de viagens

void libertaARRTrips (Trips arrayT, int size)
{
    for (int i =0;i<size;i++)
    {
        if (arrayT[i] != NULL)
        {
            free(arrayT[i]->city);
            free(arrayT[i]->user);
            free(arrayT[i]);
        }
    }
    free (arrayT);
}





// Funções para obter informacoes da estrutura das viagens


    // Devolve a classificacao do driver

double getClassTD (Trips arrayT, int id)
{
    return arrayT[id-1]->scoreDriver;
}

    // Devolve o id do driver

int getDriverT (Trips arrayT, int id)
{
    return arrayT[id-1]->driver;
}

    // Devolve o username do user

char *getUserT (Trips arrayT, int id)
{
    return strdup (arrayT[id-1]->user);
}

    // Devolve a cidade

char *getCity (Trips arrayT, int id)
{
    return strdup (arrayT[id-1]->city);
}

    // Devolve o genero do driver

char getGenderDriverTrips (Trips arrayT, Drivers drivers, int id)
{
    return getArrDriversGender (drivers, arrayT[id-1]->driver);
}

    // Devolve o genero do user

char getGenderUserTrips (Trips arrayT, Users users, int id)
{
    return getGenderUsers (users, arrayT[id-1]->user);
}

    // Devolve a data da viagem

unsigned short int getDateArrTrips (Trips arrayT, int id)
{
    return arrayT[id-1]->date;
}

    // Devolve a distancia percorrida

int getDistTrips (Trips arrayT, int id)
{
    return arrayT[id-1]->distance;
}

    // Indica se o user deu gorjeta

int getArrTripsTipVerify (Trips arrayT, int id)
{
    return arrayT[id-1]->tip!=0;
}

    // Devolve a gorjeta

double getArrTripsTip (Trips arrayT, int id)
{
    return arrayT[id-1]->tip;
}

    // Verifica se a viagem existe

bool verifyTripNULL (Trips arrayT, int id)
{
    return (arrayT[id-1]!=NULL);
}





// Funções para calcular informacoes sobre as viagens


    // Funcao que calcula o preço de uma viagem

double preco (int distance, char classe)
{
    double valor;
    if (classe == 'b')
        valor = 3.25 + 0.62 * (double) distance;
    else if (classe == 'g')
        valor = 4.00 + 0.79 * (double) distance;
    else
        valor = 5.20 + 0.94 * (double) distance;

    return valor;
}

    // Funcao que calcula o total ganho por um driver

double totalSpentDrivers (Trips arrayTrips, Drivers drivers, int id)
{
    double totalSpent = 0;
    int numTrips = getArrDriversNumberTrips (drivers, id);

    for (int i = 0;i<numTrips;i++)
    {
        int idTrip = getArrDriversTrip (drivers, id, i);

        int distance = arrayTrips[idTrip-1]->distance;
        char carClass = getArrDriversClass (drivers, id);

        totalSpent += preco (distance, carClass) + arrayTrips[idTrip-1]->tip;
    }

    return totalSpent;
}

    // Função que calcula a classificação média de todas as viagens de um driver

double classificationDrivers (Trips arrayTrips, Drivers drivers, int id)
{
    double classification = 0;
    int numTrips = getArrDriversNumberTrips (drivers, id);

    for (int i = 0;i<numTrips;i++)
    {
        int idTrip = getArrDriversTrip (drivers, id, i);

        classification += arrayTrips[idTrip-1]->scoreDriver;
    }

    classification /= (double) numTrips;

    return classification;
}

    // Funcao que calcula o total gasto por um user

double totalSpentUsers (Trips arrayTrips, Users users, Drivers drivers, char *username)
{
    double totalSpent = 0;
    int numTrips = getNumberTripsUsers (users, username);

    for (int i = 0;i<numTrips;i++)
    {
        int idTrip = getTripUsers (users, username, i);

        int distance = arrayTrips[idTrip-1]->distance;

        int idDriver = arrayTrips[idTrip-1]->driver;
        char carClass = getArrDriversClass (drivers, idDriver);

        totalSpent += preco (distance, carClass) + arrayTrips[idTrip-1]->tip;
    }

    return totalSpent;
}

    // Funcao que calcula a distancia total percorrida por um user

int totalDistanceUsers (Trips arrayTrips, Users users, char *username)
{
    int totalDist = 0;
    int numTrips = getNumberTripsUsers (users, username);

    for (int i = 0;i<numTrips;i++)
    {
        int idTrip = getTripUsers (users, username, i);

        totalDist += arrayTrips[idTrip-1]->distance;
    }

    return totalDist;
}

    // Função que calcula a classificação média de todas as viagens de um user

double classificationUsers (Trips arrayTrips, Users users, char *username)
{
    double classification = 0;
    int numTrips = getNumberTripsUsers (users, username);

    for (int i = 0;i<numTrips;i++)
    {
        int idTrip = getTripUsers (users, username, i);

        classification += arrayTrips[idTrip-1]->scoreUser;
    }

    classification /= (double) numTrips;

    return classification;
}

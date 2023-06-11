#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "drivers.h"
#include "auxiliares.h"
#include "parsing.h"




// Funcoes relativas à estrutura base dos drivers


    // Estrutura para guardar a informacao de um driver

struct drivers
{
    char *name;                                 // Nome
    unsigned char age;                          // Idade
    char gender;                                // Genero (1 -> M ; 0 -> F)
    char classe;                                // Classe do veiculo (b -> Basic ; g -> Green ; p -> Premium)
    bool status;                                // Status (0 -> inativo ; 1 -> ativo)
    unsigned short int creation;                // Data de criacao da conta
    unsigned short int lastTrip;                // Data última viagem
    StackIdTrips trips;                         // Ids das viagens realizadas pelo driver
};

    // Liberta a memoria alocada para guardar a informacao de um driver

static void freeDriver (struct drivers *c)
{
    if (c == NULL) return;
    free (c->name);

    freeStack (c->trips);
    free (c);
}

    // Liberta a memória alocada para o array de apontadores para drivers

void libertaArrDrivers (Drivers arr,int size)
{
    int i;
    for (i = 0;i<size;i++)
    {
        freeDriver (arr[i]);
    }
    free(arr);
}

    // Recebe informacoes sobre uma viagem e adiciona-as ao respetivo driver

void pushViagens_Driver (Drivers arrayDrivers, int id, int driver, unsigned short int date)
{
    int pos = driver-1;
    
    // Adiciona o id da viagem ao array de viagens do driver

    pushViagem(id,(arrayDrivers[pos]->trips));

    // Atualiza a data da última viagem do driver caso esta tenha sido mais recente que a anteriormente alocada
    
    if (arrayDrivers[pos]->lastTrip<date) arrayDrivers[pos]->lastTrip = date;
}

    // Verifica se um driver é valido

static bool validaDriver(char *id,char *name,char *date,char *gender,char class,char *license,char *city,char *activity,unsigned short int ageDriver,unsigned short int *creation,bool *status)
{
    // verificar se exitem campos vazios
    if (id== NULL || id[0] == '\0' || name[0] == '\0' || city== NULL || city[0] == '\0' || gender== NULL || gender[0] == '\0' || license== NULL || license[0] == '\0' ) return false;

    // verificar se as datas , a class e o estado de atividade são verdadeiros
    if (pushData(date,creation) && possivelStatus(activity,status) && ageDriver && class ) return true;
    else return false;
}

    // Funcao que recebe o array das linhas do ficheiro csv dos drivers e organiza os dados dos drivers validos num array
    // em que a posicao no array corresponde ao id do driver nessa posicao -1

Drivers pushDrivers (void **arrayLines, int *NDrivers)
{
    // Número de drivers a serem guardados

    int spArray = *((int *)arrayLines[1]);

    int sizeArray, lastId, positionAux = 1;
    
    char *lastIdString = strdup ((char*)arrayLines[spArray+2-positionAux]);
    lastIdString = strsep(&lastIdString,";");

    while (!sscanf(lastIdString, "%d", &lastId))
    {
        free(lastIdString);
        positionAux++;
        free (lastIdString);
        lastIdString = strdup ((char*)arrayLines[spArray+2-positionAux]);
        lastIdString = strsep(&lastIdString,";");
    }

    sizeArray = lastId + positionAux - 1;
    free (lastIdString);

    *NDrivers = sizeArray;

    Drivers arrayDrivers = malloc(sizeArray * sizeof(Drivers));

    for (int i = 0; i < sizeArray; i++) arrayDrivers[i] = NULL;

    int idDriver;
    char *name,*id,*token,*copia,*inicio,*data,*gender,class,*license,*city,*activity;
    unsigned short int accountCreation;
    unsigned char driverAge;
    bool status;


    for (int i = 0;i<spArray;i++)
    {
       
        copia = strdup((char*) arrayLines[i+2]);

        inicio = copia;

        // Guarda o id do condutor
        id = strsep(&copia,";");

        // Guarda o nome do condutor
        name = strsep(&copia,";");

        // Guarda a idade do conduto
        token = strsep(&copia,";");
        driverAge = age(token);

        // Guarda o género
        gender= strsep(&copia,";");

        // Guarda a classe do veículo do condutor
        token = strsep(&copia,";");
        class = pushClass(token);

        // Salta a matrícula do veículo do condutor
        license = strsep(&copia,";");

        // Guarda a cidade do condutor
        city = strsep(&copia,";");

        // Guarda a data de criação da conta
        data = strsep(&copia,";");

        // Guarda a atividade do condutor
        activity = strsep(&copia,"\n;");

        if (validaDriver(id,name,data,gender,class,license,city,activity,driverAge,&accountCreation,&status))
        {

            idDriver = atoi(id)-1;
            arrayDrivers[idDriver] = malloc(sizeof(struct drivers));
            arrayDrivers[idDriver]->name = strdup(name);
            arrayDrivers[idDriver]->gender = toupper(gender[0]);
            arrayDrivers[idDriver]->age = driverAge;
            arrayDrivers[idDriver]->classe = class;
            arrayDrivers[idDriver]->creation = accountCreation;
            arrayDrivers[idDriver]->status = status;
            arrayDrivers[idDriver]->lastTrip = 0;
            arrayDrivers[idDriver]->trips = initStack();

        }
        else
        {
            //printf("Invalido : %s" , (char *) arrayLines[i+2]);
        }

        free (inicio);

    }

    libertaParsing (arrayLines);
    
    return arrayDrivers;
}





// Funções para obter informacoes da estrutura dos drivers


    // Indica se o driver existe

bool existeDriver (Drivers drivers,int id)
{
    return (drivers[id-1] != NULL);
}

    // Devolve o status de um driver

bool getArrDriversStatus (Drivers drivers, int id)
{
    return drivers[id-1]->status;
}

    // Devolve a data da ultima viagem de um driver

unsigned short int getArrDriversLastTrip (Drivers drivers, int id)
{
    return drivers[id-1]->lastTrip;
}

    // Devolve o numero de um viagens de um driver

int getArrDriversNumberTrips (Drivers drivers, int id)
{
    return getSp(drivers[id-1]->trips);
}

    // Devolve o nome de um driver

char *getArrDriversName (Drivers drivers, int id)
{
    return strdup (drivers[id-1]->name);
}

    // Devolve o genero de um driver

char getArrDriversGender (Drivers drivers, int id)
{
    return drivers[id-1]->gender;
}

    // Devolve a idade de um driver

unsigned char getArrDriversAge (Drivers drivers, int id)
{
    return drivers[id-1]->age;
}

    // Devolve o id de uma viagem de um driver

int getArrDriversTrip (Drivers drivers, int idDriver, int tripPosition)
{
    return getIdTStack(drivers[idDriver-1]->trips,tripPosition);
}

    // Devolve a classe do automovel de um driver

char getArrDriversClass (Drivers drivers, int id)
{
    return drivers[id-1]->classe;
}

    // Devolve a data de criacao da conta de um driver

unsigned short int getArrDriversCreationDate (Drivers drivers, int id)
{
    return drivers[id-1]->creation;
}

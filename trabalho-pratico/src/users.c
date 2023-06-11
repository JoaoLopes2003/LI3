#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "auxiliares.h"
#include "users.h"
#include "rides.h"
#include "parsing.h"




// Funcoes relativas à estrutura base dos users


    // Estrutura para guardar a informacao de um user

struct user
{
    char *name;                                 // Nome
    char gender;                                // Genero (1 -> M ; 0 -> F)
    unsigned char age;                          // Idade
    unsigned short int creation;                // Data de criacao da conta
    bool status;                                // Status (0 -> inativo ; 1 -> ativo)
    unsigned short int lastTrip;                // Data última viagem
    StackIdTrips trips;                         // Ids das viagens realizadas pelo user
};

    // Estrutura final

struct users
{
    GHashTable *users;                          // HashTable dos users
};

    // Liberta a memoria alocada para guardar a informacao de um user

void freeUser (void *user)
{
    User u = user;
    free(u->name);
    freeStack(u->trips);
    free (u);
}

    // Liberta a informação dos users

void libertaUsers (Users users)
{
    g_hash_table_destroy (users->users);
    free (users);
}

    // Verifica se um user é valido

static bool validaUser(char *username,char *name,char *date,char *gender,char *method,char *activity,unsigned short int ageDriver,unsigned short int *creation,bool *status)
{
    // Verificar se exitem campos vazios

    if (username== NULL || username[0] == '\0' || name== NULL || name[0] == '\0' || gender== NULL || gender[0] == '\0' || method== NULL || method[0] == '\0' ) return false;

    // Verificar se as datas , a class e o estado de atividade são verdadeiros

    if (pushData(date,creation) && possivelStatus(activity,status) && ageDriver) return true;
    else return false;
}

    // Funcao que recebe o array das linhas do ficheiro csv dos users e organiza os dados dos users validos numa hashTable

Users pushUsers (void **arrayLines)
{
    // Número de users a serem guardados

    int sizeArray = *((int*) arrayLines[1]);

    GHashTable* htable = g_hash_table_new_full(g_str_hash,g_str_equal,free,freeUser);


    char *username,*name,*key,*token,*copia,*inicio,*data,*gender,*method,*activity;

    unsigned short int userAge,accountCreation;
    bool status;


    for (int i = 0;i<sizeArray;i++)
    {
        copia = strdup((char*) arrayLines[i+2]);

        inicio = copia;

        // Guarda o username/key do use
        username = strsep(&copia,";");

        // Guarda o nome do user
        name = strsep(&copia,";");
        

        // Guarda o género do user
        gender = strsep(&copia,";");

        // Guarda a idade do user
        token = strsep(&copia,";");
        userAge = age(token);

        // Guarda a data de criação da conta do user
        data = strsep(&copia,";");

        // Guarda o método de pagamento
        method = strsep(&copia,";");      

        // Guarda a atividade do user
        activity = strsep(&copia,";\n");

        if (validaUser(username,name,data,gender,method,activity,userAge,&accountCreation,&status))
        {
            User user = malloc(sizeof(struct user));

            key  = strdup(username);
            user->name = strdup(name);
            user->age = userAge;
            user->creation=accountCreation;
            user->status = status;
            user->gender = toupper(gender[0]);
            user->trips = initStack();
            user->lastTrip =0;

            g_hash_table_insert(htable,key,user);
        }
        else
        {
            // printf("Invalida: %s",(char *) arrayLines[i+2]);
        }
        free (inicio);
    }

    libertaParsing (arrayLines);

    Users users = malloc (sizeof(struct users));

    users->users = htable;

    return users;
}

    // Recebe informacoes sobre uma viagem e adiciona-as ao respetivo user

void pushTrips_User (Users users, char *username, int id, unsigned short int date)
{
    GHashTable *htable = users->users;

    // Ir buscar o respetivo user

    User user = g_hash_table_lookup (htable, username);

    // Adiciona o id da viagem ao array de viagens do user

    pushViagem(id,user->trips);

    // Atualiza a data da última viagem do user caso esta tenha sido mais recente que a anteriormente alocada
    
    if (user->lastTrip<date) user->lastTrip = date;
}





// Funções para obter informacoes da estrutura dos users


    // Devolve o status de um user

bool getstatusUser (User u)
{
    return u->status;
}

    // Devolve a data da ultima viagem de um user

unsigned short int getLastTripUser (User u)
{
    return u->lastTrip;
}

    // Devolve o numero de users

int numeroUsers (Users users)
{
    return g_hash_table_size (users->users);
}

    // Devolve o nome de um user

char *getNameUsers (Users users, char *username)
{
    GHashTable *hashUsers = users->users;
    User user = g_hash_table_lookup (hashUsers, username);
    return strdup (user->name);
}

    // Devolve o genero de um user

char getGenderUsers (Users users, char *username)
{
    GHashTable *hashUsers = users->users;
    User user = g_hash_table_lookup (hashUsers, username);
    return user->gender;
}

    // Devolve a idade de um user

unsigned char getAgeUsers (Users users, char *username)
{
    GHashTable *hashUsers = users->users;
    User user = g_hash_table_lookup (hashUsers, username);
    return user->age;
}

    // Devolve o numero de um viagens de um user

int getNumberTripsUsers (Users users, char *username)
{
    GHashTable *hashUsers = users->users;
    User user = g_hash_table_lookup (hashUsers, username);
    return getSp(user->trips);
}

    // Devolve o id de uma viagem de um user

int getTripUsers (Users users, char *username, int tripPosition)
{
    GHashTable *hashUsers = users->users;
    User user = g_hash_table_lookup (hashUsers, username);
    return getIdTStack(user->trips,tripPosition);
}

    // Devolve a hashTable dos users

GHashTable *hashUsers (Users users)
{
    return users->users;
}

    // Devolve o status de um user

bool getStatusUsers (Users users, char *username)
{
    GHashTable *hashUsers = users->users;
    User user = g_hash_table_lookup (hashUsers, username);
    return (user!= NULL && user->status);
}

    // Devolve a data de criacao da conta de um user

unsigned short int getCreationDateUsers (Users users, char *username)
{
    GHashTable *hashUsers = users->users;
    User user = g_hash_table_lookup (hashUsers, username);
    return user->creation;
}

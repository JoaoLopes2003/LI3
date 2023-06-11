#include <stdio.h>
#include "usersExtensions.h"




// Estrutura para a querie 3


    // Estrutura auxiliar para guardar a informacao de um user

typedef struct usersDistAux
{
    int distance;                           // Distancia percorrida
    char *username;                         // Username
    unsigned short int lastTrip;            // Data da ultima viagem
} UsersDistAux;

    // Estrutura final para guardar a informacao de um user

struct usersDist
{
    char *username;                         // Username
    int distance;                           // Distancia percorrida
};

    // Estrutura usada para passar dados para a funcao iteratorU que vai ordenar o array de users por distancia total percorrida

typedef struct arg
{
    int i;                                  // Numero de elementos no array
    UsersDistAux **array;                   // Array auxiliar
    Users hashUsers;                  // HashTable dos users
    Trips arrayT;                           // Array das viagens
} Arg;


    // Itera sobre todas as keys da hashTable dos users, criando um array desordenado com as informacoes dos users ativos

static void iteratorU (gpointer key, gpointer value, gpointer user_data)
{
    UsersDistAux **arrayUsersAux = ((Arg*) user_data)->array;
    
    Arg *resultado = (Arg*) user_data;
    int i = resultado->i;

    User user = (User) value;

    // experimentar sem (Utilizador)
    if (getstatusUser (user))
    {
        arrayUsersAux[i]->lastTrip = getLastTripUser (user);
        arrayUsersAux[i]->distance = totalDistanceUsers (resultado->arrayT,resultado->hashUsers,key);
        arrayUsersAux[i]->username = strdup ((char*) key);
        resultado->i++;
    }
}

static int usersAtivos;

    // Funcao que determina que o array de users vai ser ordenado por ordem decrescente de distancia total percorrida

static int comparatorUsers (const void *p, const void *q)
{
    UsersDistAux l = **((UsersDistAux**) p);
    UsersDistAux r = **((UsersDistAux**) q);


    if (l.distance<r.distance)
    {
        return 1;
    }
    else if (l.distance>r.distance)
    {
        return -1;
    }
    else
    {
        if (l.lastTrip>r.lastTrip)
        {
            return -1;
        }
        else if (l.lastTrip<r.lastTrip)
        {
            return 1;
        }
        else
        {
            if (strcmp(l.username,r.username)<0)
            {
                return -1;
            }
            return 1;
        }
    }
}

    // Funcao que copia apenas a informacao necessaria de um array para outro, que ocupa apenas o espaço de memória necessario

static void copyArray (UsersDistAux **arrOrigin, UsersDist arrDest, int nUsers)
{
    for (int i = 0;i<nUsers;i++)
    {
        arrDest[i]->username = arrOrigin[i]->username;
        arrDest[i]->distance = arrOrigin[i]->distance;
    }
}

    // Funcao que percorre todos os users e cria um array ordenado por ordem decrescente de distancia total percorrida de users ativos

UsersDist arrayUsersDistOrd (Users users, Trips arrayT, int nRides)
{
    // Guarda temporariamente a distancia total percorrida pelos users

    int nUsers = numeroUsers (users);

    UsersDistAux **arrayUsersAux = malloc (nUsers * (sizeof(UsersDistAux**)));

    for (int i = 0;i<nUsers;i++) arrayUsersAux[i] = malloc (sizeof(struct usersDistAux));

    Arg *arg = malloc(sizeof(Arg));
    arg->array = arrayUsersAux;
    arg->i = 0;
    arg->arrayT = arrayT;
    arg->hashUsers = users;

    // É para puder passar o inteiro i e o array de users como 
    // sendo apenas um argumento para a função g_hash_table_foreach

    g_hash_table_foreach (hashUsers (users), iteratorU, arg);

    // Ordena o array dos users por ordem decrescente (QSort)
    int nUsersAtivos = arg->i;

    qsort ((void*)arrayUsersAux, nUsersAtivos, sizeof(arrayUsersAux[0]), comparatorUsers);

    UsersDist arrayUsers = malloc(nUsersAtivos * sizeof(UsersDist));

    for (int i = 0;i<nUsersAtivos;i++) arrayUsers[i] = malloc (sizeof(struct usersDist));

    copyArray (arrayUsersAux, arrayUsers, nUsersAtivos);

    for (int i = 0;i<nUsers;i++) free(arrayUsersAux[i]);

    free (arrayUsersAux);

    free(arg);

    usersAtivos = nUsersAtivos;

    return arrayUsers;
}

    // Liberta a memoria alocada para a estrutura da querie 3

void libertaArrUsersOrd (UsersDist arrayOrd)
{
    for (int i =0; i<usersAtivos;i++)
    {
        free(arrayOrd[i]->username);
        free(arrayOrd[i]);
    }
    free (arrayOrd);
}

    // Devolve o username do user numa determinada posicao do array

char *getUsernameArrUDist (UsersDist arrayUsers, int position)
{
    return strdup (arrayUsers[position]->username);
}

    // Devolve a distancia total percorrida por um user numa determinada posicao do array

int getDistArrUDist (UsersDist arrayUsers, int position)
{
    return arrayUsers[position]->distance;
}

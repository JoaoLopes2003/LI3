#ifndef _USERS_H_
#define _USERS_H_
#include <stdbool.h>
#include <glib.h>



// Funcoes relativas à estrutura base dos users


    // Estrutura para guardar a informacao de um user

typedef struct user *User;

    // Estrutura final para guardar a hashTable dos users

typedef struct users *Users;

    // Liberta a memoria alocada para guardar a informacao de um user

void freeUser (void *user);

    // Liberta a informação dos users

void libertaUsers (Users users);

    // Funcao que recebe o array das linhas do ficheiro csv dos users e organiza os dados dos users validos numa hashTable

Users pushUsers (void **arrayLines);

    // Recebe insformacoes sobre uma viagem e adiciona-as ao respetivo user

void pushTrips_User (Users users, char *username, int id, unsigned short int date);



// Funções para obter informacoes da estrutura dos users


    // Devolve o status de um user

bool getstatusUser (User u);

    // Devolve a data da ultima viagem de um user

unsigned short int getLastTripUser (User u);

    // Devolve o numero de users

int numeroUsers (Users users);

    // Devolve o nome de um user

char *getNameUsers (Users users, char *username);

    // Devolve o genero de um user

char getGenderUsers (Users users, char *username);

    // Devolve a idade de um user

unsigned char getAgeUsers (Users users, char *username);

    // Devolve o numero de um viagens de um user

int getNumberTripsUsers (Users users, char *username);

    // Devolve o id de uma viagem de um user

int getTripUsers (Users users, char *username, int tripPosition);

    // Devolve a hashTable dos users

GHashTable *hashUsers (Users users);

    // Devolve o status de um user

bool getStatusUsers (Users users, char *username);

    // Devolve a data de criacao da conta de um user

unsigned short int getCreationDateUsers (Users users, char *username);


#endif
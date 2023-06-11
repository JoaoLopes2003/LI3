#ifndef _USERSEXTENSIONS_H_
#define _USERSEXTENSIONS_H_
#include "users.h"
#include "rides.h"



// Estrutura para a querie 3


    // Estrutura para guardar a informacao de um user

typedef struct usersDist **UsersDist;

    // Funcao que percorre todos os users e cria um array ordenado por ordem decrescente de distancia total percorrida de users ativos

UsersDist arrayUsersDistOrd (Users users, Trips arrayT, int nRides);

    // Liberta a memoria alocada para a estrutura da querie 3

void libertaArrUsersOrd (UsersDist arrayOrd);

    // Devolve o username do user numa determinada posicao do array

char *getUsernameArrUDist (UsersDist arrayUsers, int position);

     // Devolve a distancia total percorrida por um user numa determinada posicao do array

int getDistArrUDist (UsersDist arrayUsers, int position);


#endif
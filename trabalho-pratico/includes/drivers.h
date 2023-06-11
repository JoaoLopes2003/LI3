#ifndef _DRIVERS_H_
#define _DRIVERS_H_
#include <stdbool.h>



// Funcoes relativas à estrutura base dos drivers


    // Estrutura para guardar a informacao de um driver

typedef struct drivers **Drivers;


    // Liberta a memória alocada para o array de apontadores para drivers

void libertaArrDrivers (Drivers arr,int size);

    // Recebe informacoes sobre uma viagem e adiciona-as ao respetivo driver

void pushViagens_Driver (Drivers arrayDrivers, int id, int driver, unsigned short int date);

    // Funcao que recebe o array das linhas do ficheiro csv dos drivers e organiza os dados dos drivers validos num array
    // em que a posicao no array corresponde ao id do driver nessa posicao -1

Drivers pushDrivers (void **arrayLines, int *NDrivers);



// Funções para obter informacoes da estrutura dos drivers


    // Indica se o driver existe

bool existeDriver(Drivers drivers,int id);

    // Devolve o status de um driver

bool getArrDriversStatus (Drivers drivers, int id);

    // Devolve a data da ultima viagem de um driver

unsigned short int getArrDriversLastTrip (Drivers drivers, int id);

    // Devolve o numero de um viagens de um driver

int getArrDriversNumberTrips (Drivers drivers, int id);

    // Devolve o nome de um driver

char *getArrDriversName (Drivers drivers, int id);

    // Devolve o genero de um driver

char getArrDriversGender (Drivers drivers, int id);

    // Devolve a idade de um driver

unsigned char getArrDriversAge (Drivers drivers, int id);

    // Devolve o id de uma viagem de um driver

int getArrDriversTrip (Drivers drivers, int idDriver, int tripPosition);

    // Devolve a classe do automovel de um driver

char getArrDriversClass (Drivers drivers, int id);

    // Devolve a data de criacao da conta de um driver

unsigned short int getArrDriversCreationDate (Drivers drivers, int id);


#endif
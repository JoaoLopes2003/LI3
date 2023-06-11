#ifndef _AUXILIARES_H_
#define _AUXILIARES_H_
#include "stdbool.h"
#include <ncurses.h>



// Funcoes uteis para outros ficheiros


    // Estrutura que guarda os ids das viagens realizadas por um driver ou user

typedef struct stackIds *StackIdTrips;

    // Inicializa um array de ids de viagens

StackIdTrips initStack ();

    // Adiciona uma viagem a um array de ids de viagens

void pushViagem (int id, StackIdTrips st);

    // Liberta a memoria alocada para um array de ids de viagens

void freeStack (StackIdTrips st);

    // Devolve o id de uma viagem num array de ids de viagens

int getIdTStack (StackIdTrips st,int position);

    // Devolve o numero de viagens num array de ids de viagens

int getSp (StackIdTrips st);



// Funcoes relacionadas com verificacoes e conversao e calculo de dados


    // Função que testa validade de uma classe e devolve o valor correspondente

char pushClass (char *class);

    // Testa se o status existe (active ou inactive)

bool possivelStatus (char *str,bool *acumulador);

    // Função que converte uma string para o valor da avalição ou grojeta se esta for válida, caso contrário retorna -1

double pushTip (char *str);

    // Verifica se a string corresponde a um inteiro maior que zero, senão retorna zero

int intMaiorQue0 (char *str);

    // Verifica se a data é valida e se preenche os valores do day, month e year

bool checkData (char *data, int *day,int *month,int *year);

    // Calcula o preco de uma viagem

double price (double distance, char class);



// Funcoes relacionadas com o modo iterativo


    // Função que remove tudo que tiver sido escrito numa janela dentro da sua caixa

void clearBox (WINDOW *win);

    // Funçao que limpa o que foi escrito numa linha da janela numa certa
    // linha a partir de uma coordenada passada como argumento, inserindo ‘ ‘ de
    // forma a substituir eventuais carateres que possam ter sido impressos antes

void cleanLine (WINDOW *win,int y,int xStart);

    // Função que da print na janela centrado na linha

void printCenter (WINDOW *win,char *str,int linha);



// Funcoes relacionadas com datas


    // Transforma uma string de uma data num unsigned short int, para anos a comecar no ano 2000

unsigned short int pushData1 (char *data);

    // Transforma uma string de uma data num unsigned short int, para anos a comecar no ano 2000,
    // se a data for valida

bool pushData (char *data, unsigned short int *acumula);

    // Transforma um unsigned short int em uma string de uma data, contando a partir do ano 2000

char *decodeData (unsigned short int data);

    // Calcula a idade se for valida a data de nascimento

int age (char *aniversary);


#endif
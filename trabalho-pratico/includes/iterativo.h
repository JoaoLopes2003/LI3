#ifndef _ITERATIVO_H_
#define _ITERATIVO_H_
#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <outputs.h>
#include "auxiliares.h"



// Modo Iterativo


    // Estrutura dos menus

typedef struct menu *MENU;

    // Ações no menu principal, mudar de opção ou executar uma certa opção.

void keyUPMenu (MENU m);

void keyDownMenu (MENU m);

bool pressEnter (MENU m);

    // Função que responde as keys pressionadas pelo utilizador

bool runMenu (MENU m);

    // Função que sinaliza o erro de quando um utilizador insere um caminho errado para os ficheiros csv

void errorPath (int xCenter,int yCenter);

    // Função que cria uma janela de uma certa dimensão e ajusta as coordenadas iniciais para estar centrada em xCenter e yCenter

WINDOW *newWindow (int height, int length, int xCenter, int yCenter);

    // Função responsável por desenhar o menu

void desenharMenu (MENU m);

    // Função que cria e inicializa o menu

MENU initMenu (int xCenter,int yCenter,void *c);


#endif
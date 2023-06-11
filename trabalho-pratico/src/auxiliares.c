#include "auxiliares.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>





// Funcoes uteis para outros ficheiros



    // Data de hoje e tipos de automoveis

#define TODAY 20221009
#define Basic 'b'
#define Premium 'p'
#define Green 'g'

    // Estrutura que guarda os ids das viagens realizadas por um driver ou user

struct stackIds
{
    int *ids;                                   // Array de ids de viagens
    int sp;                                     // Número de ids
    int size;                                   // Espaco alocado para o array
};

    // Inicializa um array de ids de viagens

StackIdTrips initStack ()
{
    StackIdTrips trips = malloc(sizeof(struct stackIds));
    trips->sp = 0;
    trips->size = 4;
    trips->ids = malloc(4 * sizeof(int));
    return trips;
}

    // Adiciona uma viagem a um array de ids de viagens

void pushViagem (int id, StackIdTrips st)
{
    if (st->size == st->sp)
    {
        st->size *= 2;
        st->ids = realloc(st->ids,st->size * sizeof(int));
    }
    st->ids[st->sp] = id;
    st->sp++;
}

    // Liberta a memoria alocada para um array de ids de viagens

void freeStack (StackIdTrips st)
{
    free (st->ids);
    free (st);
}

    // Devolve o id de uma viagem num array de ids de viagens

int getIdTStack(StackIdTrips st,int position)
{
    if (position < 0 || position >= st->sp) return -1 ;
    return st->ids[position];
}

    // Devolve o numero de viagens num array de ids de viagens

int getSp(StackIdTrips st)
{
    return st->sp;
}





// Funcoes relacionadas com verificacoes e conversao e calculo de dados


    // Função que passa as letras maiúsculas de uma string inicial para minúsculas

static void strToLower (char *str)
{
    int i;
    for (i=0;str[i] != '\0';i++)
        str[i] = tolower(str[i]);
}

    // Função que testa validade de uma classe e devolve o valor correspondente

char pushClass (char *class)
{
    char res;
    strToLower(class);
    if (strcmp (class, "basic") == 0) res = Basic;
    else if (strcmp (class, "premium") == 0) res = Premium;
    else if (strcmp (class, "green") == 0) res = Green;
    else res =0;
    return res;
}

    // Testa se o status existe (active ou inactive)

bool possivelStatus (char *str,bool *acumulador)
{
    strToLower (str);
    if (strcmp(str,"active") == 0 )
    {
        *acumulador = true;
        return true;

    }
    else if (strcmp(str,"inactive") == 0)
    {
        *acumulador = false;
        return true;
    }
    return false;
}

    // Função que converte uma string para o valor da avalição ou grojeta se esta for válida, caso contrário retorna -1

double pushTip (char *str)
{
    double s =-1;
    char *resto = malloc(strlen(str) +1);

    if (sscanf (str,"%lf%s",&s,resto) != 1 || s < 0 || !(strcmp (str,"nan")))
        s = -1;

    //else printf ("Valido:  %s\n",str);*/
    free (resto);
    return s;
} 

    // Verifica se a string corresponde a um inteiro maior que zero, senão retorna zero

int intMaiorQue0 (char *str)
{
    int d =0;
    char *resto = malloc(strlen(str) +1);

    if (sscanf (str,"%d%s", &d,resto) != 1 || d < 0) d =0; // verificar que a string tem o formato certo e, não é negativa


    free(resto);
    return d;
}

    // Verifica se a data é valida e se preenche os valores do day, month e year

bool checkData(char *data, int *day,int *month,int *year)
{
    bool vality;
    
    // Verifica a validade da data por formato da string e, posteriormente, por valores possiveis

    if (sscanf(data,"%d/%d/%d",day,month,year) != 3) vality =false;
    else
    {
        vality = (*day >0 && *day < 32 && *month >0 && *month <13 &&  *year > 0);
    }
    return vality;
}

    // Calcula o preco de uma viagem

double price (double distance, char class)
{
    double value;
    if (class == Basic)
        value = 3.25 + 0.62 * distance;
    else if (class == Green)
        value = 4.00 + 0.79 * distance;
    else
        value = 5.20 + 0.94 * distance;

    return value;
}





// Funcoes relacionadas com o modo iterativo


    // Função que remove tudo que tiver sido escrito numa janela dentro da sua caixa

void clearBox (WINDOW *win)
{
    int i, j;
    for (i = 1; i< win->_maxy;i++)
    {
        for (j = 1; j< win->_maxx;j++)
        {
            mvwprintw(win,i,j," ");
        }
    }
    wrefresh(win);
}

    // Funçao que limpa o que foi escrito numa linha da janela numa certa
    // linha a partir de uma coordenada passada como argumento, inserindo ‘ ‘ de
    // forma a substituir eventuais carateres que possam ter sido impressos antes

void cleanLine (WINDOW *win,int y,int xStart)
{
    for (;xStart < win->_maxx;xStart++)
    {
        mvwprintw(win,y,xStart," ");
    }
}

    // Função que da print na janela centrado na linha

void printCenter (WINDOW *win,char *str,int linha)
{
    int startX = (win->_maxx - strlen(str)) / 2;

    mvwprintw(win,linha,startX,"%s",str);
}





// Funcoes relacionadas com datas


    // Transforma uma string de uma data num unsigned short int, para anos a comecar no ano 2000

unsigned short int pushData1 (char *data)
{

    unsigned short int resultado = 0;

    unsigned short int day, month, year;

    sscanf (data, "%hu/%hu/%hu", &day, &month, &year);

    year -= 2000;

    int yearAux = 0;

    while (yearAux<year)
    {
        if (yearAux%4!=0)
        {
            resultado += 365;
        }
        else
        {
            resultado += 366;
        }
        yearAux++;
    }

    int monthdays[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    resultado += monthdays[month-1];

    if (year%4==0 && month>=3) resultado++;

    resultado += day;

    return resultado;
}

    // Transforma uma string de uma data num unsigned short int, para anos a comecar no ano 2000,
    // se a data for valida

bool pushData (char *data, unsigned short int *acumula)
{
    int day,month,year;


    if (!checkData(data,&day,&month,&year)) return false;

    unsigned short int resultado = 0;

    year -= 2000;

    int yearAux = 0;

    while (yearAux<year)
    {
        if (yearAux%4!=0)
        {
            resultado += 365;
        }
        else
        {
            resultado += 366;
        }
        yearAux++;
    }

    int monthdays[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    resultado += monthdays[month-1];

    if (year%4==0 && month>=3) resultado++;

    resultado += day;
    *acumula = resultado;
    return true;
}

    // Transforma um unsigned short int em uma string de uma data, contando a partir do ano 2000

char *decodeData (unsigned short int data)
{
    int year = 0, month = 1, day = 1;

    char prov[29];

    // Passar o ano e o mes para inteiro

    for (int i = 0;(data>366 && i%4==0) || (data>365 && i%4!=0);i++)
    {
        if (i%4!=0)
        {
            data -= 365;
            year++;
        }
        else
        {
            data -= 366;
            year++;
        }
    }

    year += 2000;

    // Calcular o numero de meses

    if (data-31>0)
    {
        data -= 31;
        month++;
        if (data-28>0 && year%4!=0)
        {
            data -= 28;
            month++;
        }
        else if (data-29>0 && year%4==0)
        {
            data -= 29;
            month++;
        }
        if (month==3 && data-31>0)
        {
            data -= 31;
            month++;
            if (data-30>0)
            {
                data -= 30;
                month++;
                if (data-31>0)
                {
                    data -= 31;
                    month++;
                    if (data-30>0)
                    {
                        data -= 30;
                        month++;
                        if (data-31>0)
                        {
                            data -= 31;
                            month++;
                            if (data-31>0)
                            {
                                data-= 31;
                                month++;
                                if (data-30>0)
                                {
                                    data -= 30;
                                    month++;
                                    if (data-31>0)
                                    {
                                        data -= 31;
                                        month++;
                                        if (data-30>0)
                                        {
                                            data -= 30;
                                            month++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Calcular o numero de dias

    day = data;

    // Converter outra vez para string

    sprintf (prov, "%02d/%02d/%d", day, month, year);
    char *res = strdup(prov);

    return res;
}


    // Calcula a idade se for valida a data de nascimento

int age (char *aniversary)
{
    int day, month, year;
    int result;
    if (checkData(aniversary,&day,&month,&year))
    {
        // Transforma a data para a forma AAAAMMDD

        int aux = day + 100 * (month+100*year);
        aux = TODAY - aux;
        result = aux /10000;
    }
    else result = 0;
    return result;
};
